#include "classes.h"
#include "frontend.cpp"

#include <fstream>
#include <sstream>
#include <iostream>

const std::string flight_admin_email = "flightadmin@gmail.com";
const std::string flight_admin_password = "flightadmin@123";
const std::string train_admin_email = "trainadmin@gmail.com";
const std::string train_admin_password = "trainadmin@123";

std::vector<Train *> allTrainsList;
std::vector<Flight *> allFlightsList;
std::vector<Terminal *> allStationsList;
std::vector<Terminal *> allAirportsList;

FlightAdmin *flightAdmin = new FlightAdmin(flight_admin_email, flight_admin_password);
TrainAdmin *trainAdmin = new TrainAdmin(train_admin_email, train_admin_password);

int codeToTerminalPosition(std::vector<Terminal *> &terminalList, std::string &code)
{
    int pos = 0;
    for (Terminal *t : terminalList)
    {
        if (t->code == code)
            return pos;
        pos++;
    }
    return -1;
}
int numberToTransportPosition(std::string transport, std::string &number)
{
    int pos = 0;
    if (transport == "Train")
    {
        for (auto *t : allTrainsList)
        {
            if (t->number == number)
                return pos;
            pos++;
        }
    }
    else
    {
        for (auto *f : allFlightsList)
        {
            if (f->number == number)
                return pos;
            pos++;
        }
    }
    return -1;
}
bool isTerminalsExistsInTransport(ListNode *coveringCities, std::string &boardingTerminalCode, std::string &destinationTerminalCode)
{
    while (coveringCities != NULL)
    {
        if (coveringCities->currentTerminal.code == boardingTerminalCode)
        {
            coveringCities = coveringCities->next;
            while (coveringCities != NULL)
            {
                if (coveringCities->currentTerminal.code == destinationTerminalCode)
                    return true;
                coveringCities = coveringCities->next;
            }
            return false;
        }
        coveringCities = coveringCities->next;
    }
    return false;
}
std::vector<int> filterTransportsForUser(std::string transport, std::string &boardingTerminalCode, std::string &destinationTerminalCode, DateTime journeyDate)
{
    std::vector<int> filteredList;
    int position = 0;
    if (transport == "Train")
    {
        for (auto t : allTrainsList)
        {
            if (isTerminalsExistsInTransport(t->coveringCities, boardingTerminalCode, destinationTerminalCode))
            {
                filteredList.push_back(position);
            }
            position++;
        }
    }
    else
    {
        for (auto t : allFlightsList)
        {
            if (isTerminalsExistsInTransport(t->coveringCities, boardingTerminalCode, destinationTerminalCode))
            {
                filteredList.push_back(position);
            }
            position++;
        }
    }
    return filteredList;
}

std::vector<std::string> stringToVectorString(std::string &str)
{
    std::vector<std::string> words;
    std::string word = "";
    str += '\n';
    for (char ch : str)
    {
        if (ch == ' ' || ch == '\n' || ch == '\0')
        {
            if (word.size() > 0)
            {
                words.push_back(word);
                word = "";
            }
        }
        else
        {
            word += ch;
        }
    }
    return words;
}

std::vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
bool isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
DateTime *stringToDateTime(std::string &str, int sz = 14)
{
    int day, month, year, hour, minute;
    if (sz == 8)
    {
        if (str.size() != 8 || str[2] != '/' || str[5] != '/')
            return NULL;

        for (int i = 0; i < 8; i++)
        {
            if (i != 2 && i != 5)
            {
                if (str[i] < '0' || str[i] > '9')
                    return NULL;
            }
        }

        day = stoi(str.substr(0, 2));
        month = stoi(str.substr(3, 2));
        year = stoi(str.substr(6, 2));
        hour = 0;
        minute = 0;
    }
    else
    {
        if (str.size() != 14 || str[2] != '/' || str[5] != '/' || str[8] != '-' || str[11] != ':')
            return NULL;

        for (int i = 0; i < 14; i++)
        {
            if (i != 2 && i != 5 && i != 8 && i != 11)
            {
                if (str[i] < '0' || str[i] > '9')
                    return NULL;
            }
        }

        day = stoi(str.substr(0, 2));
        month = stoi(str.substr(3, 2));
        year = stoi(str.substr(6, 2));
        hour = stoi(str.substr(9, 2));
        minute = stoi(str.substr(12, 2));
    }

    if (day < 0 || month < 0 || year < 0 || month > 12 || hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return NULL;

    if (month == 2)
    {
        if (isLeapYear(2000 + year))
        {
            if (day > 29)
                return NULL;
        }
        else if (day > 28)
            return NULL;
    }
    else if (daysInMonth[month - 1] < day)
        return NULL;

    return new DateTime(year, month, day, hour, minute);
}
bool isOnlyDigits(std::string &str)
{
    for (char ch : str)
    {
        if (!isdigit(ch))
        {
            return false;
        }
    }
    return true;
}
bool isOnlyAlphas(std::string &str)
{
    for (char ch : str)
    {
        if (!isalpha(ch))
        {
            return false;
        }
    }
    return true;
}
bool isOnlyDigitOrAlpha(std::string &str)
{
    for (char ch : str)
    {
        if (!isalnum(ch))
        {
            return false;
        }
    }
    return true;
}

std::string generateTrainTicketPNR()
{
    std::string pnr = "";
    for (int i = 0; i < 10; i++)
    {
        pnr += ('0' + (std::rand() % 10));
    }
    return pnr;
}
std::string generateFlightTicketPNR()
{
    std::string pnr = "";
    for (int i = 0; i < 6; i++)
    {
        int r = std::rand();
        if ((r % 2) == 0)
        {
            pnr += ('0' + (r % 10));
        }
        else
        {
            pnr += ('A' + (r % 26));
        }
    }
    return pnr;
}
DateTime *getCurrentDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%y - %H:%M", localTime);
    std::string formattedDateTime = buffer;

    return stringToDateTime(formattedDateTime);
}

// Admin Class
Admin::Admin(std::string email, std::string passsword)
{
    this->email = email;
    this->password = passsword;
}
void Admin::adminLoginPanel()
{
    std::vector<std::string> inputs = printLoginPanel("Admin");
    printAlert(inputs[0] + " " + inputs[1] + " " + this->email + " " + this->password);
    if (inputs[0] == this->email && inputs[1] == this->password)
    {
        printAlert("Logged In Successfully");
        this->adminDashboard();
    }
    else
    {
        printAlert("Login Failed.\nInvalid Credentials");
    }
}

// TrainAdmin Class
void TrainAdmin::adminDashboard()
{
    printAdminDashboard("Train", "Station");
    char choice = inputUserChoice();
    switch (choice)
    {
    case '1':
    {
        printTransportOrTerminalSelectionPanel("Trains", "Stations", allTrainsList, allFlightsList, allStationsList);
        break;
    }
    case '2':
    {
        this->addNewTransport();
        break;
    }
    case '3':
    {
        this->addNewTerminal();
        break;
    }
    case '4':
    {
        this->removeTransport();
        break;
    }
    case '5':
    {
        this->removeTerminal();
        break;
    }
    case '6':
    {
        printAlert("Logged Out Successfully");
        return;
    }
    default:
    {
        printAlert("Wrong Choice!");
        break;
    }
    }
    this->adminDashboard();
}
void TrainAdmin::addNewTerminal()
{
    std::string name, city, code;
    printAddNewTerminalPanel("Station", name, city, code);

    std::string message;
    if (name.size() == 0)
    {
        message = "Failed to add station.\nStation name is required.";
    }
    else if (city.size() == 0)
    {
        message = "Failed to add station.\nStation city is required.";
    }
    else if (code.size() == 0)
    {
        message = "Failed to add station.\nStation city is required.";
    }
    else
    {
        int validChar = isOnlyAlphas(code);
        if (!validChar)
        {
            message = "Failed to add airport.\nInvalid characters in code: " + code;
        }
        else if (code.size() > 4)
        {
            message = "Failed to add airport.\nCode size is more than 4: " + code;
        }
        else
        {
            transform(code.begin(), code.end(), code.begin(), ::toupper);
            int stationPosition = codeToTerminalPosition(allStationsList, code);
            if (stationPosition == -1)
            {
                allStationsList.push_back(new Terminal(name, city, code));
                // writing into file
                message = "Station Added successfully";
            }
            else
                message = "Failed to add station.\nStation Code already exists.";
        }
    }
    printAlert(message);
}
void TrainAdmin::addNewTransport()
{
    int citiesCount;
    std::string name, number, coveringCitiesCode, departureTimes, totalSeats;
    printAddNewTransportPanel("Train", citiesCount, name, number, coveringCitiesCode, departureTimes, totalSeats);

    if (citiesCount == -1)
    {
        printAlert("Failed to add Train.\nCities count should be integer");
        return;
    }

    if (name.size() == 0)
    {
        printAlert("Failed to add Train.\nTrain Name is Required");
    }
    else if (number.size() != 5 || !isOnlyDigits(number))
    {
        printAlert("Failed to add Train.\nTrain Number is Invalid");
    }
    else
    {
        // checking if train number is unique
        int trainPosition = numberToTransportPosition("Train", number);
        if (trainPosition != -1)
        {
            printAlert("Failed to add Train.\nTrain Number already exists.");
            return;
        }

        // separating cities
        std::vector<std::string> cityCodes = stringToVectorString(coveringCitiesCode);
        if (cityCodes.size() != citiesCount)
        {
            printAlert("Failed to add Train.\nNo. of cities provided does not match cities count");
            return;
        }
        for (std::string &cityCode : cityCodes)
        {
            transform(cityCode.begin(), cityCode.end(), cityCode.begin(), ::toupper);
        }

        // separating times
        std::vector<std::string> cityTimes = stringToVectorString(departureTimes);
        if (cityTimes.size() != citiesCount)
        {
            printAlert("Failed to add Train.\nNo. of departure times provided does not match cities count");
            return;
        }

        ListNode *coveringCities;
        ListNode *movingPtr = coveringCities;

        // validating and adding stations to the list
        for (int i = 0; i < citiesCount; i++)
        {
            int stationPosition = codeToTerminalPosition(allStationsList, cityCodes[i]);
            if (stationPosition == -1)
            {
                printAlert("Failed to add Train.\nA station does not exists : " + cityCodes[i]);
                return;
            }
            DateTime *DT = stringToDateTime(cityTimes[i]);
            if (DT == NULL)
            {
                printAlert("Failed to add Train.\nA date time format is wrong : " + cityTimes[i]);
                return;
            }
            if (i > 0 && *DT <= movingPtr->departureTime)
            {
                printAlert("Failed to add Train.\nA date time difference is equal or negative: " + cityTimes[i - 1] + " " + cityTimes[i]);
                return;
            }
            movingPtr->next = new ListNode;
            movingPtr = movingPtr->next;
            movingPtr->currentTerminal = *allStationsList[stationPosition];
            movingPtr->departureTime = *DT;
        }

        std::vector<std::string> seatsCt = stringToVectorString(totalSeats);
        std::vector<int> seatsInInt;

        // checking no. of seats in each class
        if (seatsCt.size() != 5)
        {
            printAlert("Failed to add Train.\nSeats provided does not match.");
            return;
        }
        for (std::string seat : seatsCt)
        {
            if (isOnlyDigits(seat))
            {
                seatsInInt.push_back(stoi(seat));
            }
            else
            {
                printAlert("Failed to add Train.\nNumber of seat is invalid: " + seat);
                return;
            }
        }

        allTrainsList.push_back(new Train(name, number, coveringCities->next, seatsInInt));
        // writing in file
        printAlert("Train added successfully");
    }
}
void TrainAdmin::removeTerminal()
{
    std::string stationCode;
    printRemoveTerminalPanel("Station", stationCode);

    if (!isOnlyAlphas(stationCode))
    {
        printAlert("Failed to remove Station.\nInvalid Station Code");
    }
    else
    {
        transform(stationCode.begin(), stationCode.end(), stationCode.begin(), ::toupper);
        int stationPosition = codeToTerminalPosition(allStationsList, stationCode);
        if (stationPosition != -1)
        {
            allStationsList.erase(allStationsList.begin() + stationPosition);
            // delete from file
            printAlert("Station removed successfully");
        }
        else
        {
            printAlert("Failed to remove Station.\nStation does not exists.");
        }
    }
}
void TrainAdmin::removeTransport()
{
    std::string trainNumber;
    printRemoveTransportPanel("Train", trainNumber);
    if (!isOnlyDigits(trainNumber))
    {
        printAlert("Failed to remove Train.\nInvalid Train Number: " + trainNumber);
    }
    else
    {
        int trainPosition = numberToTransportPosition("Train", trainNumber);
        if (trainPosition != -1)
        {
            allTrainsList.erase(allTrainsList.begin() + trainPosition);
            // writing into file
            printAlert("Train removed successfully");
        }
        else
        {
            printAlert("Failed to remove Train.\nTrain does not exists.");
        }
    }
}

// FlightAdmin Class
void FlightAdmin::adminDashboard()
{
    printAdminDashboard("Flight", "Airport");
    char choice = inputUserChoice();
    switch (choice)
    {
    case '1':
    {
        printTransportOrTerminalSelectionPanel("Flights", "Airports", allTrainsList, allFlightsList, allAirportsList);
        break;
    }
    case '2':
    {
        this->addNewTransport();
        break;
    }
    case '3':
    {
        this->addNewTerminal();
        break;
    }
    case '4':
    {
        this->removeTransport();
        break;
    }
    case '5':
    {
        this->removeTerminal();
        break;
    }
    case '6':
    {
        printAlert("Logged Out Successfully");
        return;
    }
    default:
    {
        printAlert("Wrong Choice");
        break;
    }
    }
    this->adminDashboard();
}
void FlightAdmin::addNewTerminal()
{
    std::string name, city, code;
    printAddNewTerminalPanel("Airport", name, city, code);

    std::string message;
    if (name.size() == 0)
    {
        message = "Failed to add airport.\nAirport name is required. ";
    }
    else if (city.size() == 0)
    {
        message = "Failed to add airport.\nAirport city is required.";
    }
    else if (code.size() == 0)
    {
        message = "Failed to add airport.\nAirport city is required. ";
    }
    else
    {
        int validChar = isOnlyDigitOrAlpha(code);
        if (!validChar)
        {
            message = "Failed to add airport.\nInvalid characters in code: " + code;
        }
        else if (code.size() > 4)
        {
            message = "Failed to add airport.\nCode size is more than 4: " + code;
        }
        else
        {
            transform(code.begin(), code.end(), code.begin(), ::toupper);
            int airportPosition = codeToTerminalPosition(allAirportsList, code);
            if (airportPosition == -1)
            {
                allAirportsList.push_back(new Terminal(name, city, code));
                // writing into file
                message = "Airport Added successfully";
            }
            else
                message = "Failed to add airport.\nAirport Code already exists.";
        }
    }
    printAlert(message);
}
void FlightAdmin::addNewTransport()
{
    int citiesCount;
    std::string name, number, coveringCitiesCode, departureTimes, totalSeats;
    printAddNewTransportPanel("Flight", citiesCount, name, number, coveringCitiesCode, departureTimes, totalSeats);

    if (citiesCount == -1)
    {
        printAlert("Failed to add Flight.\nCities count should be integer");
        return;
    }

    if (name.size() == 0)
    {
        printAlert("Failed to add Flight.\nFlight Name is Required");
    }
    else if (number.size() != 5 || !isOnlyDigitOrAlpha(number))
    {
        printAlert("Failed to add Flight.\nFlight Number is Invalid");
    }
    else
    {
        transform(number.begin(), number.end(), number.begin(), ::toupper);

        // checking if flight number is unique
        int flightPosition = numberToTransportPosition("Flight", number);
        if (flightPosition != -1)
        {
            printAlert("Failed to add Flight.\nFlight Number already exists.");
            return;
        }

        // separating cities
        std::vector<std::string> cityCodes = stringToVectorString(coveringCitiesCode);
        if (cityCodes.size() != citiesCount)
        {
            printAlert("Failed to add Flight.\nNo. of cities provided does not match cities count");
            return;
        }
        for (std::string &cityCode : cityCodes)
        {
            transform(cityCode.begin(), cityCode.end(), cityCode.begin(), ::toupper);
        }

        // separating times
        std::vector<std::string> cityTimes = stringToVectorString(departureTimes);
        if (cityTimes.size() != citiesCount)
        {
            printAlert("Failed to add Flight.\nNo. of departure times provided does not match cities count.");
            return;
        }

        ListNode *coveringCities;
        ListNode *movingPtr = coveringCities;

        // validating and adding airports to the list
        for (int i = 0; i < citiesCount; i++)
        {
            int airportPosition = codeToTerminalPosition(allAirportsList, cityCodes[i]);
            if (airportPosition == -1)
            {
                printAlert("Failed to add Flight.\nAn airport does not exists : " + cityCodes[i]);
                return;
            }
            DateTime *DT = stringToDateTime(cityTimes[i]);
            if (DT == NULL)
            {
                printAlert("Failed to add Flight.\nA date time format is wrong : " + cityTimes[i]);
                return;
            }
            if (i > 0 && *DT <= movingPtr->departureTime)
            {
                printAlert("Failed to add Flight.\nA date time difference is equal or negative: " + cityTimes[i - 1] + " " + cityTimes[i]);
                return;
            }
            movingPtr->next = new ListNode;
            movingPtr = movingPtr->next;
            movingPtr->currentTerminal = *allAirportsList[airportPosition];
            movingPtr->departureTime = *DT;
        }

        std::vector<std::string> seatsCt = stringToVectorString(totalSeats);
        std::vector<int> seatsInInt;

        // checking and adding no. of seats in each class
        if (seatsCt.size() != citiesCount)
        {
            printAlert("Failed to add Flight.\nSeats provided does not match.");
            return;
        }
        for (std::string seat : seatsCt)
        {
            if (isOnlyDigits(seat))
            {
                seatsInInt.push_back(stoi(seat));
            }
            else
            {
                printAlert("Failed to add Flight.\nNumber of seat is invalid: " + seat);
                return;
            }
        }

        allFlightsList.push_back(new Flight(name, number, coveringCities->next, seatsInInt));
        // write in file
        printAlert("Flight added successfully");
    }
}
void FlightAdmin::removeTerminal()
{
    std::string airportCode;
    printRemoveTerminalPanel("Airport", airportCode);

    if (!isOnlyAlphas(airportCode))
    {
        printAlert("Failed to remove Airport.\nInvalid Airport Code.");
    }
    else
    {
        transform(airportCode.begin(), airportCode.end(), airportCode.begin(), ::toupper);
        int airportPosition = codeToTerminalPosition(allAirportsList, airportCode);
        if (airportPosition != -1)
        {
            allAirportsList.erase(allAirportsList.begin() + airportPosition);
            // delete from file
            printAlert("Airport removed successfully");
        }
        else
        {
            printAlert("Failed to remove Airport.\nAirport does not exists.");
        }
    }
}
void FlightAdmin::removeTransport()
{
    std::string flightNumber;
    printRemoveTransportPanel("Flight", flightNumber);

    if (!isOnlyDigitOrAlpha(flightNumber))
    {
        printAlert("Failed to remove Flight.\nInvalid Flight Number: " + flightNumber);
    }
    else
    {
        transform(flightNumber.begin(), flightNumber.end(), flightNumber.begin(), ::toupper);
        int flightPosition = numberToTransportPosition("Flight", flightNumber);
        if (flightPosition != -1)
        {
            allFlightsList.erase(allFlightsList.begin() + flightPosition);
            // writing into file
            printAlert("Flight removed successfully");
        }
        else
        {
            printAlert("Failed to remove Flight.\nFlight Does not exists.");
        }
    }
}

// User Class
std::string User::getPassword()
{
    return this->password;
}
void User::setPassword(std::string newPassword)
{
    this->password = newPassword;
}

// UserManager Class
UserManager::UserManager()
{
    this->loggedInUser = NULL;
    // load all users data from file;
}
bool UserManager::isEmailRegistered(std::string email)
{
    bool flag = false;
    for (User *user : this->usersList)
    {
        if (user->email == email)
        {
            flag = true;
            break;
        }
    }
    return flag;
}
User *UserManager::findUser(std::string email)
{
    for (User *user : this->usersList)
    {
        if (email == user->email)
        {
            return user;
        }
    }
    return NULL;
}
void UserManager::userRegistrationPanel()
{
    std::vector<std::string> inputs = printUserRegistrationPanel();
    std::string message;
    if (inputs[0].size() == 0)
    {
        message = "Registration Failed.\nName is required.";
    }
    else if (inputs[1].size() == 0)
    {
        message = "Registration Failed.\nEmail is required";
    }
    else if (inputs[2].size() == 0)
    {
        message = "Registration Failed.\nPassword is required";
    }
    else if (inputs[2] != inputs[3])
    {
        message = "Registration Failed.\nPassword and confirm password do no match";
    }
    else
    {
        if (this->isEmailRegistered(inputs[1]))
        {
            message = "Registration Failed.\nUser Email already exists.";
        }
        else
        {
            User *newUser = new User(inputs[0], inputs[1], inputs[2]);
            // writing into file is left
            usersList.push_back(newUser);
            message = "You are successfully registered.";
        }
    }

    printAlert(message);
}
void UserManager::userLoginPanel()
{
    std::vector<std::string> inputs = printLoginPanel("User");

    std::string message;
    if (inputs[0].size() == 0)
    {
        printAlert("Login Failed.\nEmail is required.");
    }
    else if (inputs[1].size() == 0)
    {
        printAlert("Login Failed.\nPassword is required.");
    }
    else
    {
        User *tempPtr = findUser(inputs[0]);
        if (tempPtr != NULL && inputs[1] == tempPtr->getPassword())
        {
            loggedInUser = tempPtr;
            printAlert("Logged In successfully");
            this->userDashboard();
        }
        else
        {
            printAlert("Login Failed\nInvalid Credentials");
        }
    }
}
void UserManager::userDashboard()
{
    printUserDashboard(this->loggedInUser->fullName);
    char choice = inputUserChoice();
    switch (choice)
    {
    case '1':
    {
        printTransportOrTerminalSelectionPanel("Trains", "Stations", allTrainsList, allFlightsList, allStationsList);
        break;
    }
    case '2':
    {
        printTransportOrTerminalSelectionPanel("Flights", "Airports", allTrainsList, allFlightsList, allAirportsList);
        break;
    }
    case '3':
    {
        printBookingTransportSelection();
        char choice2 = inputUserChoice();
        switch (choice2)
        {
        case '1':
        {
            this->ticketBookingPanel("Train");
            break;
        }
        case '2':
        {
            this->ticketBookingPanel("Flight");
            break;
        }
        case '3':
        {
            break;
        }
        deafult:
        {
            printAlert("Wrong Choice");
            break;
        }
        }
        break;
    }
    case '4':
    {
        printTicketTransportSelection("View");
        char choice2 = inputUserChoice();
        switch (choice2)
        {
        case '1':
        {
            printAllTrainTickets(this->loggedInUser->trainTickets);
            printContinue();
            break;
        }
        case '2':
        {
            printAllFlightTickets(this->loggedInUser->flightTickets);
            printContinue();
            break;
        }
        case '3':
        {
            break;
        }
        default:
        {
            printAlert("Wrong Choice");
            break;
        }
        }
        break;
    }
    case '5':
    {
        printTicketTransportSelection("Cancel");
        char choice2 = inputUserChoice();
        switch (choice2)
        {
        case '1':
        {
            this->cancelTicketPanel("Train");
            break;
        }
        case '2':
        {
            this->cancelTicketPanel("Flight");
            break;
        }
        case '3':
        {
            break;
        }
        default:
        {
            printAlert("Wrong Choice");
            break;
        }
        }
        break;
    }
    case '6':
    {
        this->changeUserPassword();
        break;
    }
    case '7':
    {
        this->changeUserName();
        break;
    }
    case '8':
    {
        loggedInUser = NULL;
        printAlert("Logged Out Successfully");
        return;
    }
    default:
    {
        printAlert("Wrong Choice");
        break;
    }
    }
    this->userDashboard();
}
void UserManager::ticketBookingPanel(std::string transport)
{
    std::vector<std::string> inputs = printTicketBookingPanel(transport);
    DateTime *journeyDate = stringToDateTime(inputs[2], 8);
    if (journeyDate == NULL)
    {
        printAlert("Booking Failed\nInvalid Journey Date");
    }
    else if (!isOnlyAlphas(inputs[0]) || inputs[0].size() > 4 || !isOnlyAlphas(inputs[1]) || inputs[1].size() > 4)
    {
        printAlert("Booking Failed\nInvalid Code.");
    }
    else
    {
        // we can validate that journeyDate time should be greater than or equal to current DateTime

        transform(inputs[0].begin(), inputs[0].end(), inputs[0].begin(), ::toupper);
        transform(inputs[1].begin(), inputs[1].end(), inputs[1].begin(), ::toupper);

        if (transport == "Train")
        {
            // search for Train;
            std::vector<int> filteredTrainsPosition = filterTransportsForUser("Train", inputs[0], inputs[1], *journeyDate);
            if (filteredTrainsPosition.size() == 0)
            {
                printAlert("No Train scheduled around that Date");
                return;
            }

            // showing all available trains
            printAllTrains(allTrainsList, filteredTrainsPosition);

            // taking user's train choice
            int trainChoice = inputNumber("Enter Train Choice: ");
            if (trainChoice < 0 || trainChoice > filteredTrainsPosition.size())
            {
                printAlert("Booking Failed.\nInvalid Choice.");
            }
            else
            {
                trainChoice--;
                Train *choosenTrain = allTrainsList[filteredTrainsPosition[trainChoice]];
                std::string trainName = choosenTrain->name;

            trainSeatSelectionCheckPoint:
                printSeatTypeSelectionPanel("Train", trainName);
                int seatChoice = inputNumber();
                if (seatChoice < 0 || seatChoice > 5)
                {
                    printContinue("Invalid Choice");
                    goto trainSeatSelectionCheckPoint;
                }
                else if (seatChoice == 5)
                {
                    printAlert("Booking Canceled");
                }
                else
                {
                    seatChoice--;
                    if (choosenTrain->totalSeats[seatChoice] > choosenTrain->bookedSeats[seatChoice])
                    {
                        std::string pnr = generateTrainTicketPNR();
                        loggedInUser->trainTickets.push_back(new TrainTicket(seatChoice, choosenTrain, inputs[0], inputs[1]));
                        choosenTrain->bookedSeats[seatChoice]++;
                        // write in file;
                        printAlert("Seat Booked Successfully.");
                    }
                    else
                    {
                        printAlert("Booking Failed.\nNo seats Available");
                    }
                }
            }
        }
        else
        {
            // search for Flight
            std::vector<int> filteredFlightsPosition = filterTransportsForUser("Flight", inputs[0], inputs[1], *journeyDate);
            if (filteredFlightsPosition.size() == 0)
            {
                printAlert("No Flight scheduled around that Date");
                return;
            }

            // showing all available flights
            printAllFlights(allFlightsList, filteredFlightsPosition);

            // taking user's flight choice
            int flightChoice = inputNumber("Enter Flight Choice: ");
            if (flightChoice < 0 || flightChoice > filteredFlightsPosition.size())
            {
                printAlert("Booking Failed.\nInvalid Choice.");
            }
            else
            {
                flightChoice--;
                Flight *choosenFlight = allFlightsList[filteredFlightsPosition[flightChoice]];
                std::string flightName = choosenFlight->name;

            flightSeatSelectionCheckPoint:
                printSeatTypeSelectionPanel("Flight", flightName);
                int seatChoice = inputNumber();
                if (seatChoice < 0 || seatChoice > 3)
                {
                    printContinue("Invalid Choice");
                    goto flightSeatSelectionCheckPoint;
                }
                else if (seatChoice == 3)
                {
                    printAlert("Booking Canceled");
                }
                else
                {
                    seatChoice--;
                    if (choosenFlight->totalSeats[seatChoice] > choosenFlight->bookedSeats[seatChoice])
                    {
                        std::string pnr = generateFlightTicketPNR();
                        loggedInUser->flightTickets.push_back(new FlightTicket(seatChoice, choosenFlight, inputs[0], inputs[1]));
                        choosenFlight->bookedSeats[seatChoice]++;
                        // write in file;
                        printAlert("Seat Booked Successfully.");
                    }
                    else
                    {
                        printAlert("Booking Failed.\nNo seats Available");
                    }
                }
            }
        }
    }
}
void UserManager::cancelTicketPanel(std::string transport)
{
    int ticketsCt;
    if (transport == "Train")
    {
        printAllTrainTickets(this->loggedInUser->trainTickets);
        ticketsCt = this->loggedInUser->trainTickets.size();
    }
    else
    {
        printAllFlightTickets(this->loggedInUser->flightTickets);
        ticketsCt = this->loggedInUser->flightTickets.size();
    }

    if (ticketsCt == 0)
    {
        printContinue();
    }
    else
    {
        int ticketChoice = inputNumber("Enter choice number to cancel ticket: ");
        if (ticketChoice < 0 || ticketChoice > ticketsCt)
        {
            printAlert("Failed to cancel Ticket.\nInvalid Choice.");
        }
        else
        {
            ticketChoice--;
            if (transport == "Train")
            {
                auto allTrainTickets = &this->loggedInUser->trainTickets;
                int seatChoice = (*allTrainTickets)[ticketChoice]->seatChoice;
                // increment seats in train
                (*allTrainTickets)[ticketChoice]->trainPtr->bookedSeats[seatChoice]--;
                // delete ticket from allTrainTickets;
                (*allTrainTickets).erase((*allTrainTickets).begin() + ticketChoice);
            }
            else
            {
                auto allFlightTickets = &this->loggedInUser->flightTickets;
                int seatChoice = (*allFlightTickets)[ticketChoice]->seatChoice;
                // increment seats in flight
                (*allFlightTickets)[ticketChoice]->flightPtr->bookedSeats[seatChoice]--;
                // delete ticket from allFlightTickets;
                (*allFlightTickets).erase((*allFlightTickets).begin() + ticketChoice);
            }
            printAlert("Ticket Canceled Successfully.");
        }
    }
}
void UserManager::changeUserPassword()
{
    std::vector<std::string> info = printChangePasswordPanel();
    std::string message;
    if (info[1].size() == 0)
    {
        message = "Password is required";
    }
    else if (info[1] != info[2])
    {
        message = "New password and Confirm password does not match";
    }
    else if (info[0] != this->loggedInUser->getPassword())
    {
        message = "Invalid Old Password";
    }
    else
    {
        loggedInUser->setPassword(info[1]);
        // writing in file
        message = "Password Changed Successfully";
    }
    printAlert(message);
}
void UserManager::changeUserName()
{
    std::string newName = printChangeNamePanel(this->loggedInUser->fullName);
    std::string message;
    if (newName.size() == 0)
        message = "Name is required";
    else
    {
        this->loggedInUser->fullName = newName;
        // writing in file
        message = "Name Updated Successfully";
    }
    printAlert(message);
}

// CSVManager Class
std::vector<std::vector<std::string>> CSVManager::readCSV(const std::string &filename)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}
void CSVManager::writeCSV(const std::string &filename, const std::vector<std::vector<std::string>> &data)
{
    std::ofstream file(filename);
    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            file << row[i];
            if (i < row.size() - 1)
                file << ",";
        }
        file << "\n";
    }
}

// Terminal Class
Terminal::Terminal(std::string name, std::string city, std::string code)
{
    this->name = name;
    this->city = city;
    this->code = code;
}

// Ticket Class

// FlightTicket Class
FlightTicket::FlightTicket(int seatChoice, Flight *flightPtr, std::string boardingTerminalCode, std::string destinationTerminalCode)
    : flightPtr(flightPtr)
{
    this->seatChoice = seatChoice;
    int board = codeToTerminalPosition(allAirportsList, boardingTerminalCode);
    int dest = codeToTerminalPosition(allAirportsList, destinationTerminalCode);
    this->boardingTerminal = allAirportsList[board];
    this->destinationTerminal = allAirportsList[dest];
    this->bookingDate = getCurrentDateTime();
}

// TrainTicket Class
TrainTicket::TrainTicket(int seatChoice, Train *trainPtr, std::string boardingTerminalCode, std::string destinationTerminalCode)
    : trainPtr(trainPtr)
{
    this->seatChoice = seatChoice;
    int board = codeToTerminalPosition(allStationsList, boardingTerminalCode);
    int dest = codeToTerminalPosition(allStationsList, destinationTerminalCode);
    this->boardingTerminal = allStationsList[board];
    this->destinationTerminal = allStationsList[dest];
    this->bookingDate = getCurrentDateTime();
}

// Transport Class
Transport::Transport(std::string name, std::string number, ListNode *coveringCities, std::vector<int> totalSeats)
{
    this->name = name;
    this->number = number;
    this->coveringCities = coveringCities;
    this->totalSeats = totalSeats;
    this->bookedSeats.assign(totalSeats.size(), 0);
}

// FlightAdmin flightAdmin->flight_admin_email, flight_admin_password);
// TrainAdmin trainAdmin->train_admin_email, train_admin_password);
UserManager userManager;

void loadDataFromFiles() {}
// void saveData(){}

void ControlPanel()
{
    printLandingPanel();
    char choice = inputUserChoice();
    switch (choice)
    {
    case '1':
    {
        printTransportOrTerminalSelectionPanel("Trains", "Stations", allTrainsList, allFlightsList, allStationsList);
        break;
    }
    case '2':
    {
        printTransportOrTerminalSelectionPanel("Flights", "Airports", allTrainsList, allFlightsList, allAirportsList);
        break;
    }
    case '3':
    {
        userManager.userRegistrationPanel();
        break;
    }
    case '4':
    {
        userManager.userLoginPanel();
        break;
    }
    case '5':
    {
        printAdminLoginOptions();
        char choice2 = inputUserChoice();
        switch (choice2)
        {
        case '1':
        {
            flightAdmin->adminLoginPanel();
            break;
        }
        case '2':
        {
            trainAdmin->adminLoginPanel();
            break;
        }
        case '3':
        {
            break;
        }
        default:
        {
            printAlert("Wrong Choice");
            break;
        }
        }
        break;
    }
    case '6':
    {
        printExit();
        return;
    }
    default:
    {
        printAlert("Wrong Choice");
        break;
    }
    }
    ControlPanel();
}
