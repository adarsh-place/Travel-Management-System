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
std::vector<User *> allUsersList;

FlightAdmin *flightAdmin = new FlightAdmin(flight_admin_email, flight_admin_password);
TrainAdmin *trainAdmin = new TrainAdmin(train_admin_email, train_admin_password);
UserManager *userManager = new UserManager();
CSVManager *csvManager = new CSVManager();

// Helper Functions
int cityCodeToTerminalPosition(std::vector<Terminal *> &terminalList, std::string &cityCode)
{
    int pos = 0;
    for (Terminal *t : terminalList)
    {
        if (t->code == cityCode)
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
std::pair<ListNode *, ListNode *> terminalsInTransportRoute(ListNode *coveringCities, std::string &boardingTerminalCode, std::string &destinationTerminalCode)
{
    while (coveringCities != NULL)
    {
        if (coveringCities->currentTerminal.code == boardingTerminalCode)
        {
            ListNode *temp = coveringCities;
            coveringCities = coveringCities->next;
            while (coveringCities != NULL)
            {
                if (coveringCities->currentTerminal.code == destinationTerminalCode)
                    return {temp, coveringCities};
                coveringCities = coveringCities->next;
            }
            return {NULL, NULL};
        }
        coveringCities = coveringCities->next;
    }
    return {NULL, NULL};
}
std::tuple<std::vector<int>, std::vector<std::pair<ListNode *, ListNode *>>> filterTransportsForUser(std::string transport, std::string &boardingTerminalCode, std::string &destinationTerminalCode, DateTime journeyDate)
{
    std::vector<int> positions;
    std::vector<std::pair<ListNode *, ListNode *>> citiesList;

    int position = 0;
    if (transport == "Train")
    {
        for (auto t : allTrainsList)
        {
            auto cities = terminalsInTransportRoute(t->coveringCities, boardingTerminalCode, destinationTerminalCode);
            if (cities.first != NULL)
            {
                positions.push_back(position);
                citiesList.push_back(cities);
            }
            position++;
        }
    }
    else
    {
        for (auto t : allFlightsList)
        {
            auto cities = terminalsInTransportRoute(t->coveringCities, boardingTerminalCode, destinationTerminalCode);
            if (cities.first != NULL)
            {
                positions.push_back(position);
                citiesList.push_back(cities);
            }
            position++;
        }
    }
    return {positions, citiesList};
}

std::vector<std::string> splitAsStrings(std::string &s, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter))
    {
        tokens.push_back(item);
    }
    return tokens;
}
std::vector<int> splitAsNums(std::string &s, char delimiter = ' ')
{
    std::vector<int> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter))
    {
        tokens.push_back(stoi(item));
    }
    return tokens;
}

std::vector<int> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
bool isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
DateTime *stringToDateTime(std::string &str)
{
    int day, month, year, hour, minute;

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

    return new DateTime(str, year, month, day, hour, minute);
}
bool isOnlyDigits(std::string &str, std::string extraChars = "")
{
    for (char ch : str)
    {
        if (!isdigit(ch))
        {
            bool find = false;
            for (char extra : extraChars)
            {
                if (extra == ch)
                {
                    find = true;
                    break;
                }
            }
            if (!find)
                return false;
        }
    }
    return true;
}
bool isOnlyAlphas(std::string &str, std::string extraChars = "")
{
    for (char ch : str)
    {
        if (!isalpha(ch))
        {
            bool find = false;
            for (char extra : extraChars)
            {
                if (extra == ch)
                {
                    find = true;
                    break;
                }
            }
            if (!find)
                return false;
        }
    }
    return true;
}
bool isOnlyDigitOrAlpha(std::string &str, std::string extraChars = "")
{
    for (char ch : str)
    {
        if (!isalnum(ch))
        {
            bool find = false;
            for (char extra : extraChars)
            {
                if (extra == ch)
                {
                    find = true;
                    break;
                }
            }
            if (!find)
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
    std::strftime(buffer, sizeof(buffer), "%d/%m/%y-%H:%M", localTime);
    std::string formattedDateTime = buffer;

    return stringToDateTime(formattedDateTime);
}

// DateTime struct
bool DateTime::operator==(const DateTime &other) const
{
    return day == other.day && month == other.month && year == other.year &&
           hour == other.hour && minute == other.minute;
}
bool DateTime::operator<(const DateTime &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    if (day != other.day)
        return day < other.day;
    if (hour != other.hour)
        return hour < other.hour;
    return minute < other.minute;
}
bool DateTime::operator>(const DateTime &other) const
{
    return other < *this;
}
bool DateTime::operator<=(const DateTime &other) const
{
    return !(*this > other);
}
bool DateTime::operator>=(const DateTime &other) const
{
    return !(*this < other);
}
bool DateTime::operator!=(const DateTime &other) const
{
    return !(*this == other);
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
    if (inputs[0] == this->email && inputs[1] == this->password)
    {
        printAlert("Logged In Successfully");
        this->adminDashboard();
    }
    else
    {
        printAlert("Login Failed.\n  Invalid Credentials");
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
    transform(code.begin(), code.end(), code.begin(), ::toupper);

    std::string message;
    bool success = false;

    if (name.size() == 0)
    {
        message = "Station name is required.";
    }
    else if (city.size() == 0)
    {
        message = "Station city is required.";
    }
    else if (code.size() == 0)
    {
        message = "Station Code is required.";
    }
    else if (code.size() > 4)
    {
        message = "Station Code size is more than 4: " + code;
    }
    else if (!isOnlyAlphas(name, " "))
    {
        message = "Station Name can only have alphabets and underscores: " + name;
    }
    else if (!isOnlyAlphas(city, " "))
    {
        message = "Station City can only have alphabets and underscores: " + city;
    }
    else if (!isOnlyAlphas(code))
    {
        message = "Invalid characters in station code: " + code;
    }
    else if (cityCodeToTerminalPosition(allStationsList, code) != -1)
    {
        message = "Station Code already exists.";
    }
    else
    {
        allStationsList.push_back(new Terminal(name, city, code));
        // writing into file
        csvManager->saveAllStations();
        success = true;
    }

    if (success)
    {
        printAlert("Station Added successfully");
    }
    else
    {
        printAlert("Failed to add station.\n" + message);
    }
}
void TrainAdmin::addNewTransport()
{
    int citiesCount;
    std::string name, number, coveringCitiesCode, departureTimes, totalSeats;
    printAddNewTransportPanel("Train", citiesCount, name, number, coveringCitiesCode, departureTimes, totalSeats);

    transform(coveringCitiesCode.begin(), coveringCitiesCode.end(), coveringCitiesCode.begin(), ::toupper);

    if (citiesCount == -1)
    {
        printAlert("Failed to add Train.\nCities count should be integer");
    }
    else if (name.size() == 0)
    {
        printAlert("Failed to add Train.\nTrain Name is Required");
    }
    else if (number.size() == 0)
    {
        printAlert("Failed to add Train.\nTrain Number is Required");
    }
    else if (!isOnlyAlphas(name, " "))
    {
        printAlert("Failed to add Train.\nTrain Name can only have alphabets and underscores: " + name);
    }
    else if (number.size() != 5 || !isOnlyDigits(number))
    {
        printAlert("Failed to add Train.\nTrain Number is Invalid");
    }
    else if (numberToTransportPosition("Train", number) != -1)
    {
        printAlert("Failed to add Train.\nTrain Number already exists.");
    }
    else if (!isOnlyDigits(totalSeats, " "))
    {
        printAlert("Failed to add Train.\nNumber of seats is invalid");
    }
    else
    {
        // separating cities
        std::vector<std::string> cityCodes = splitAsStrings(coveringCitiesCode);
        std::vector<Terminal *> coveringStations;
        if (cityCodes.size() != citiesCount)
        {
            printAlert("Failed to add Train.\nNo. of cities provided does not match cities count");
            return;
        }
        for (std::string &cityCode : cityCodes)
        {
            int stationPosition = cityCodeToTerminalPosition(allStationsList, cityCode);
            if (stationPosition == -1)
            {
                printAlert("Failed to add train\nA station does not exists: " + cityCode);
                return;
            }
            coveringStations.push_back(allStationsList[stationPosition]);
        }

        // separating destination times of stations
        std::vector<std::string> cityTimes = splitAsStrings(departureTimes);
        std::vector<DateTime *> cityDTs;
        if (cityTimes.size() != citiesCount)
        {
            printAlert("Failed to add Train.\nNo. of departure times provided does not match cities count");
            return;
        }
        // checking and storing destination times for each city
        for (int i = 0; i < citiesCount; i++)
        {
            DateTime *DT = stringToDateTime(cityTimes[i]);
            if (DT == NULL)
            {
                printAlert("Failed to add Train.\nA date time format is wrong : " + cityTimes[i]);
                return;
            }
            if (i > 0 && (*DT) <= (*cityDTs.back()))
            {
                printAlert("Failed to add Train.\nA date time difference is equal or negative: " + cityTimes[i - 1] + " " + cityTimes[i]);
                return;
            }
            cityDTs.push_back(DT);
        }

        // checking and storing no. of seats in each seat class
        std::vector<int> seatsInInt = splitAsNums(totalSeats);
        if (seatsInInt.size() != 4)
        {
            printAlert("Failed to add Train.\nSeats provided does not match.");
            return;
        }

        // adding station to the covering cities list
        ListNode *coveringCities = new ListNode();
        ListNode *movingPtr = coveringCities;
        for (int i = 0; i < citiesCount; i++)
        {
            movingPtr->next = new ListNode;
            movingPtr = movingPtr->next;
            movingPtr->currentTerminal = *coveringStations[i];
            movingPtr->departureTime = *cityDTs[i];
        }

        allTrainsList.push_back(new Train(name, number, coveringCities->next, seatsInInt));
        // writing in file
        csvManager->saveAllTrains();
        printAlert("Train added successfully");
    }
}
void TrainAdmin::removeTerminal()
{
    std::string stationCode;
    printRemoveTerminalPanel("Station", stationCode);

    transform(stationCode.begin(), stationCode.end(), stationCode.begin(), ::toupper);

    bool success = false;
    std::string message;

    if (!isOnlyAlphas(stationCode))
    {
        message = "Invalid Station Code";
    }
    else
    {
        int stationPosition = cityCodeToTerminalPosition(allStationsList, stationCode);
        if (stationPosition != -1)
        {
            allStationsList.erase(allStationsList.begin() + stationPosition);
            // delete from file
            csvManager->saveAllStations();
            success = true;
        }
        else
        {
            message = "Station does not exists.";
        }
    }

    if (success)
    {
        printAlert("Station removed successfully");
    }
    else
    {
        printAlert("Failed to remove Station.\n" + message);
    }
}
void TrainAdmin::removeTransport()
{
    std::string trainNumber;
    printRemoveTransportPanel("Train", trainNumber);

    bool success = false;
    std::string message;

    if (!isOnlyDigits(trainNumber))
    {
        message = "Invalid Train Number: " + trainNumber;
    }
    else
    {
        int trainPosition = numberToTransportPosition("Train", trainNumber);
        if (trainPosition != -1)
        {
            allTrainsList.erase(allTrainsList.begin() + trainPosition);
            // writing into file
            csvManager->saveAllTrains();
            success = true;
        }
        else
        {
            message = "Train does not exists.";
        }
    }

    if (success)
    {
        printAlert("Train removed successfully");
    }
    else
    {
        printAlert("Failed to remove Train.\n" + message);
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
    transform(code.begin(), code.end(), code.begin(), ::toupper);

    bool success = false;
    std::string message;

    if (name.size() == 0)
    {
        message = "Airport name is required. ";
    }
    else if (city.size() == 0)
    {
        message = "Airport city is required.";
    }
    else if (code.size() == 0)
    {
        message = "Airport code is required. ";
    }
    else if (code.size() > 4)
    {
        message = "Airport Code length is more than 4: " + code;
    }
    else if (!isOnlyAlphas(name, " "))
    {
        message = "Airport Name can only have alphabets and underscores: " + name;
    }
    else if (!isOnlyAlphas(city, " "))
    {
        message = "Airport City can only have alphabets and underscores: " + city;
    }
    else if (!isOnlyDigitOrAlpha(code))
    {
        message = "Invalid characters in airport code: " + code;
    }
    else if (cityCodeToTerminalPosition(allAirportsList, code) != -1)
    {
        message = "Airport Code already exists.";
    }
    else
    {
        allAirportsList.push_back(new Terminal(name, city, code));
        // writing into file
        csvManager->saveAllAirports();
        success = true;
    }

    if (success)
    {
        printAlert("Airport Added successfully");
    }
    else
    {
        printAlert("Failed to add airport.\n" + message);
    }
}
void FlightAdmin::addNewTransport()
{
    int citiesCount;
    std::string name, number, coveringCitiesCode, departureTimes, totalSeats;
    printAddNewTransportPanel("Flight", citiesCount, name, number, coveringCitiesCode, departureTimes, totalSeats);

    transform(number.begin(), number.end(), number.begin(), ::toupper);
    transform(coveringCitiesCode.begin(), coveringCitiesCode.end(), coveringCitiesCode.begin(), ::toupper);

    if (citiesCount == -1)
    {
        printAlert("Failed to add Flight.\nCities count should be integer");
    }
    else if (name.size() == 0)
    {
        printAlert("Failed to add Flight.\nFlight Name is Required");
    }
    else if (number.size() != 5 || !isOnlyDigitOrAlpha(number))
    {
        printAlert("Failed to add Flight.\nFlight Number is Invalid");
    }
    else if (numberToTransportPosition("Flight", number) != -1)
    {
        printAlert("Failed to add Flight.\nFlight Number already exists.");
    }
    else if (!isOnlyDigits(totalSeats, " "))
    {
        printAlert("Failed to add Flight.\nNumber of seat is invalid.");
    }
    else
    {
        // separating cities
        std::vector<std::string> cityCodes = splitAsStrings(coveringCitiesCode);
        std::vector<Terminal> coveringAirports;
        if (cityCodes.size() != citiesCount)
        {
            printAlert("Failed to add Flight.\nNo. of cities provided does not match cities count.");
            return;
        }
        for (std::string &cityCode : cityCodes)
        {
            int airportPosition = cityCodeToTerminalPosition(allAirportsList, cityCode);
            if (airportPosition == -1)
            {
                printAlert("Failed to add Flight\nAn airport does not exists: " + cityCode);
                return;
            }
            coveringAirports.push_back(*allAirportsList[airportPosition]);
        }

        // separating destination times of stations
        std::vector<std::string> cityTimes = splitAsStrings(departureTimes);
        std::vector<DateTime> cityDTs;
        if (cityTimes.size() != citiesCount)
        {
            printAlert("Failed to add Train.\nNo. of departure times provided does not match cities count.");
            return;
        }
        // checking and storing destination times for each city
        for (int i = 0; i < citiesCount; i++)
        {
            DateTime *DT = stringToDateTime(cityTimes[i]);
            if (DT == NULL)
            {
                printAlert("Failed to add Train.\nA date time format is wrong : " + cityTimes[i]);
                return;
            }
            if (i > 0 && (*DT) <= cityDTs.back())
            {
                printAlert("Failed to add Train.\nA date time difference is equal or negative: " + cityTimes[i - 1] + " " + cityTimes[i]);
                return;
            }
            cityDTs.push_back(*DT);
        }

        // checking and adding no. of seats in each class
        std::vector<int> seatsInInt = splitAsNums(totalSeats);
        if (seatsInInt.size() != citiesCount)
        {
            printAlert("Failed to add Flight.\nSeats provided does not match.");
            return;
        }

        // validating and adding airports to the list
        ListNode *coveringCities = new ListNode();
        ListNode *movingPtr = coveringCities;
        for (int i = 0; i < citiesCount; i++)
        {
            std::cout << "A" << i << " ";
            movingPtr->next = new ListNode();
            std::cout << "A" << i << " ";
            movingPtr = movingPtr->next;
            std::cout << "A" << i << " ";
            movingPtr->currentTerminal = coveringAirports[i];
            std::cout << "A" << i << " ";
            movingPtr->departureTime = cityDTs[i];
            std::cout << "A" << i << " ";
        }

        allFlightsList.push_back(new Flight(name, number, coveringCities->next, seatsInInt));
        // write in file
        csvManager->saveAllFlights();
        printAlert("Flight added successfully");
    }
}
void FlightAdmin::removeTerminal()
{
    std::string airportCode;
    printRemoveTerminalPanel("Airport", airportCode);

    transform(airportCode.begin(), airportCode.end(), airportCode.begin(), ::toupper);

    bool success = false;
    std::string message;

    if (!isOnlyAlphas(airportCode))
    {
        message = "Invalid Airport Code.";
    }
    else
    {
        int airportPosition = cityCodeToTerminalPosition(allAirportsList, airportCode);
        if (airportPosition != -1)
        {
            allAirportsList.erase(allAirportsList.begin() + airportPosition);
            // delete from file
            csvManager->saveAllAirports();
            success = true;
        }
        else
        {
            message = "Airport does not exists.";
        }
    }

    if (success)
    {
        printAlert("Airport removed successfully");
    }
    else
    {
        printAlert("Failed to remove Airport.\n" + message);
    }
}
void FlightAdmin::removeTransport()
{
    std::string flightNumber;
    printRemoveTransportPanel("Flight", flightNumber);

    transform(flightNumber.begin(), flightNumber.end(), flightNumber.begin(), ::toupper);

    bool success = false;
    std::string message;

    if (!isOnlyDigitOrAlpha(flightNumber))
    {
        message = "Invalid Flight Number: " + flightNumber;
    }
    else
    {
        int flightPosition = numberToTransportPosition("Flight", flightNumber);
        if (flightPosition != -1)
        {
            allFlightsList.erase(allFlightsList.begin() + flightPosition);
            // writing into file
            csvManager->saveAllFlights();
            success = true;
        }
        else
        {
            message = "Flight Does not exists.";
        }
    }

    if (success)
    {
        printAlert("Flight removed successfully");
    }
    else
    {
        printAlert("Failed to remove Flight.\n" + message);
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
    for (User *user : allUsersList)
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
    for (User *user : allUsersList)
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
            // writing into file is left
            allUsersList.push_back(new User(inputs[0], inputs[1], inputs[2]));
            message = "You are successfully registered.";
            csvManager->saveAllUsers();
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
    case 'a':
    {
        this->changeUserName();
        break;
    }
    case 'b':
    {
        this->changeUserPassword();
        break;
    }
    case 'c':
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
    inputs[2] += "-00:00";
    DateTime *journeyDate = stringToDateTime(inputs[2]);
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
            auto [filteredTrainsPosition, fromToList] = filterTransportsForUser("Train", inputs[0], inputs[1], *journeyDate);
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
                        DateTime *bookingDate = getCurrentDateTime();
                        loggedInUser->trainTickets.push_back(new TrainTicket(seatChoice, choosenTrain, fromToList[trainChoice].first, fromToList[trainChoice].second, pnr, bookingDate));
                        choosenTrain->bookedSeats[seatChoice]++;
                        // write in file;
                        csvManager->saveAllTrainTickets();
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
            auto [filteredFlightsPosition, fromToList] = filterTransportsForUser("Flight", inputs[0], inputs[1], *journeyDate);
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
                        DateTime *bookingDate = getCurrentDateTime();
                        loggedInUser->flightTickets.push_back(new FlightTicket(seatChoice, choosenFlight, fromToList[flightChoice].first, fromToList[flightChoice].second, pnr, bookingDate));
                        choosenFlight->bookedSeats[seatChoice]++;
                        // write in file;
                        csvManager->saveAllFlightTickets();
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
                TrainTicket *tT = (*allTrainTickets)[ticketChoice];
                (*allTrainTickets).erase((*allTrainTickets).begin() + ticketChoice);
                csvManager->saveAllTrainTickets();
                delete (tT);
            }
            else
            {
                auto allFlightTickets = &this->loggedInUser->flightTickets;
                int seatChoice = (*allFlightTickets)[ticketChoice]->seatChoice;
                // increment seats in flight
                (*allFlightTickets)[ticketChoice]->flightPtr->bookedSeats[seatChoice]--;
                // delete ticket from allFlightTickets;
                FlightTicket *fT = (*allFlightTickets)[ticketChoice];
                (*allFlightTickets).erase((*allFlightTickets).begin() + ticketChoice);
                csvManager->saveAllFlightTickets();
                delete (fT);
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
        csvManager->saveAllUsers();
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
        csvManager->saveAllUsers();
        message = "Name Updated Successfully";
    }
    printAlert(message);
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
FlightTicket::FlightTicket(int seatChoice, Flight *flightPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr, DateTime *bookingDate)
    : flightPtr(flightPtr)
{
    this->seatChoice = seatChoice;
    this->pnr = pnr;
    this->boardingListNode = boardingListNode;
    this->destinationListNode = destinationListNode;
    this->bookingDate = bookingDate;
}

// TrainTicket Class
TrainTicket::TrainTicket(int seatChoice, Train *trainPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr, DateTime *bookingDate)
    : trainPtr(trainPtr)
{
    this->seatChoice = seatChoice;
    this->pnr = pnr;
    this->boardingListNode = boardingListNode;
    this->destinationListNode = destinationListNode;
    this->bookingDate = bookingDate;
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

// CSVManager Class
std::string CSVManager ::join(std::vector<int> &items, char delimiter)
{
    std::string comb = "";
    for (int i = 0; i < items.size(); i++)
    {
        comb += std::to_string(items[i]);
        if (i < items.size() - 1)
            comb += delimiter;
    }
    return comb;
}
std::pair<std::string, std::string> CSVManager ::join(ListNode *items, char delimiter)
{
    std::string cityCodes = "";
    std::string departureTimes = "";

    ListNode *currCity = items;
    while (currCity != NULL)
    {
        cityCodes += currCity->currentTerminal.code;
        departureTimes += currCity->departureTime.rawDT;

        currCity = currCity->next;
        if (currCity != NULL)
        {
            cityCodes += delimiter;
            departureTimes += delimiter;
        }
    }

    return {cityCodes, departureTimes};
}

void CSVManager ::loadAllUsers()
{
    std::ifstream file("data/users.csv");

    if (!file.is_open())
    {
        printAlert("Error opening users file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string fullName, email, password;

        if (std::getline(ss, fullName, ',') &&
            std::getline(ss, email, ',') &&
            std::getline(ss, password, ','))
        {
            allUsersList.push_back(new User(fullName, email, password));
        }
    }

    file.close();
}
void CSVManager ::loadAllStations()
{
    std::ifstream file("data/stations.csv");

    if (!file.is_open())
    {
        printAlert("Error opening stations file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string code, name, city;

        if (std::getline(ss, code, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, city, ','))
        {
            allStationsList.push_back(new Terminal(name, city, code));
        }
    }

    file.close();
}
void CSVManager ::loadAllAirports()
{
    std::ifstream file("data/airports.csv");

    if (!file.is_open())
    {
        printAlert("Error opening airports file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string code, name, city;

        if (std::getline(ss, code, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, city, ','))
        {
            allAirportsList.push_back(new Terminal(name, city, code));
        }
    }

    file.close();
}
void CSVManager ::loadAllFlights()
{
    std::ifstream file("data/flights.csv");
    if (!file.is_open())
    {
        printAlert("Error opening flights file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue; // skip header
        }

        std::stringstream ss(line);
        std::string number, name, totalSeatsStr, citiesStr, timesStr;

        if (std::getline(ss, number, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, totalSeatsStr, ',') &&
            std::getline(ss, citiesStr, ',') &&
            std::getline(ss, timesStr, ','))
        {
            std::vector<int> totalSeats = splitAsNums(totalSeatsStr, '|');
            std::vector<std::string> cityCodes = splitAsStrings(citiesStr, '|');
            std::vector<std::string> cityTimes = splitAsStrings(timesStr, '|');

            ListNode *citiesList = new ListNode();
            ListNode *movingPtr = citiesList;
            for (int i = 0; i < cityCodes.size(); i++)
            {
                int flightPosition = cityCodeToTerminalPosition(allAirportsList, cityCodes[i]);

                movingPtr->next = new ListNode();
                movingPtr = movingPtr->next;
                movingPtr->currentTerminal = *allAirportsList[flightPosition];
                movingPtr->departureTime = *stringToDateTime(cityTimes[i]);
            }

            allFlightsList.push_back(new Flight(name, number, citiesList->next, totalSeats));
        }
    }

    file.close();
}
void CSVManager ::loadAllTrains()
{
    std::ifstream file("data/trains.csv");
    if (!file.is_open())
    {
        printAlert("Error opening trains file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue; // skip header
        }

        std::stringstream ss(line);
        std::string number, name, totalSeatsStr, citiesStr, timesStr;

        if (std::getline(ss, number, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, totalSeatsStr, ',') &&
            std::getline(ss, citiesStr, ',') &&
            std::getline(ss, timesStr, ','))
        {
            std::vector<int> totalSeats = splitAsNums(totalSeatsStr, '|');
            std::vector<std::string> cityCodes = splitAsStrings(citiesStr, '|');
            std::vector<std::string> cityTimes = splitAsStrings(timesStr, '|');

            ListNode *citiesList = new ListNode();
            ListNode *movingPtr = citiesList;
            for (int i = 0; i < cityCodes.size(); i++)
            {
                int stationPosition = cityCodeToTerminalPosition(allStationsList, cityCodes[i]);

                movingPtr->next = new ListNode();
                movingPtr = movingPtr->next;
                movingPtr->currentTerminal = *allStationsList[stationPosition];
                movingPtr->departureTime = *stringToDateTime(cityTimes[i]);
            }

            allTrainsList.push_back(new Train(name, number, citiesList->next, totalSeats));
        }
    }

    file.close();
}
void CSVManager ::loadAllFlightTickets()
{
    std::ifstream file("data/flight_tickets.csv");

    if (!file.is_open())
    {
        printAlert("Error opening flight tickets file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string email, pnr, flightNumber, boarding, destination, seatChoice, bookigDate;

        if (std::getline(ss, email, ',') &&
            std::getline(ss, pnr, ',') &&
            std::getline(ss, flightNumber, ',') &&
            std::getline(ss, boarding, ',') &&
            std::getline(ss, destination, ',') &&
            std::getline(ss, seatChoice, ',') &&
            std::getline(ss, bookigDate, ','))
        {
            User *user = userManager->findUser(email);
            int flightPosition = numberToTransportPosition("Flight", flightNumber);
            Flight *flight = allFlightsList[flightPosition];
            int seatType = stoi(seatChoice);
            DateTime *bookingDateTime = stringToDateTime(bookigDate);
            auto terminals = terminalsInTransportRoute(flight->coveringCities, boarding, destination);

            flight->bookedSeats[seatType]++;

            user->flightTickets.push_back(new FlightTicket(seatType, flight, terminals.first, terminals.second, pnr, bookingDateTime));
        }
    }

    file.close();
}
void CSVManager ::loadAllTrainTickets()
{
    std::ifstream file("data/train_tickets.csv");

    if (!file.is_open())
    {
        printAlert("Error opening train tickets file for reading.");
        return;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string email, pnr, trainNumber, boarding, destination, seatChoice, bookigDate;

        if (std::getline(ss, email, ',') &&
            std::getline(ss, pnr, ',') &&
            std::getline(ss, trainNumber, ',') &&
            std::getline(ss, boarding, ',') &&
            std::getline(ss, destination, ',') &&
            std::getline(ss, seatChoice, ',') &&
            std::getline(ss, bookigDate, ','))
        {
            User *user = userManager->findUser(email);
            int trainPosition = numberToTransportPosition("Train", trainNumber);
            Train *train = allTrainsList[trainPosition];
            int seatType = stoi(seatChoice);
            DateTime *bookingDateTime = stringToDateTime(bookigDate);
            auto terminals = terminalsInTransportRoute(train->coveringCities, boarding, destination);

            train->bookedSeats[seatType]++;

            user->trainTickets.push_back(new TrainTicket(seatType, train, terminals.first, terminals.second, pnr, bookingDateTime));
        }
    }

    file.close();
}

void CSVManager ::saveAllUsers()
{
    std::ofstream file("data/users.csv");

    if (!file.is_open())
    {
        printAlert("Error opening users file for writing.");
        return;
    }

    // Optional header
    file << "Full Name,Email,Password\n";

    for (const auto &user : allUsersList)
    {
        file << user->fullName << "," << user->email << "," << user->getPassword() << "\n";
    }

    file.close();
}
void CSVManager ::saveAllStations()
{
    std::ofstream file("data/stations.csv");

    if (!file.is_open())
    {
        printAlert("Error opening stations file for writing.");
        return;
    }

    // Optional header
    file << "Code,Name,City\n";

    for (const auto &station : allStationsList)
    {
        file << station->code << "," << station->name << "," << station->city << "\n";
    }

    file.close();
}
void CSVManager ::saveAllAirports()
{
    std::ofstream file("data/airports.csv");

    if (!file.is_open())
    {
        printAlert("Error opening airports file for writing.");
        return;
    }

    // Optional header
    file << "Code,Name,City\n";

    for (const auto &airport : allAirportsList)
    {
        file << airport->code << "," << airport->name << "," << airport->city << "\n";
    }

    file.close();
}
void CSVManager ::saveAllFlights()
{
    std::ofstream file("data/flights.csv");
    if (!file.is_open())
    {
        printAlert("Error opening flights file for writing.");
        return;
    }

    // Write header
    file << "Number,Name,Total Seats,Covering City Codes,Departure Times\n";

    for (const auto &flight : allFlightsList)
    {
        std::string totalSeats = join(flight->totalSeats, '|');
        auto coveringCities = join(flight->coveringCities, '|');

        file << flight->number << ","
             << flight->name << ","
             << totalSeats << ","
             << coveringCities.first << ","
             << coveringCities.second << "\n";
    }

    file.close();
}
void CSVManager ::saveAllTrains()
{
    std::ofstream file("data/trains.csv");
    if (!file.is_open())
    {
        printAlert("Error opening trains file for writing.");
        return;
    }

    // Write header
    file << "Number,Name,Total Seats,Covering City Codes,Departure Times\n";

    for (const auto &train : allTrainsList)
    {
        std::string totalSeats = join(train->totalSeats, '|');
        auto coveringCities = join(train->coveringCities, '|');

        file << train->number << ","
             << train->name << ","
             << totalSeats << ","
             << coveringCities.first << ","
             << coveringCities.second << "\n";
    }

    file.close();
}
void CSVManager ::saveAllFlightTickets()
{
    std::ofstream file("data/flight_tickets.csv");
    if (!file.is_open())
    {
        printAlert("Error opening flight tickets file for writing.");
        return;
    }

    // Write header
    file << "User Email,PNR,Flight Number,From,To,Seat Choice,Booking Date\n";

    for (const auto &user : allUsersList)
    {
        for (const auto &ticket : user->flightTickets)
        {
            file << user->email << ","
                 << ticket->pnr << ","
                 << ticket->flightPtr->number << ","
                 << ticket->boardingListNode->currentTerminal.code << ","
                 << ticket->destinationListNode->currentTerminal.code << ","
                 << std::to_string(ticket->seatChoice) << ","
                 << ticket->bookingDate->rawDT << "\n";
        }
    }

    file.close();
}
void CSVManager ::saveAllTrainTickets()
{
    std::ofstream file("data/train_tickets.csv");
    if (!file.is_open())
    {
        printAlert("Error opening train tickets file for writing.");
        return;
    }

    // Write header
    file << "User Email,PNR,Train Number,From,To,Seat Choice,Booking Date\n";

    for (const auto &user : allUsersList)
    {
        for (const auto &ticket : user->trainTickets)
        {
            file << user->email << ","
                 << ticket->pnr << ","
                 << ticket->trainPtr->number << ","
                 << ticket->boardingListNode->currentTerminal.code << ","
                 << ticket->destinationListNode->currentTerminal.code << ","
                 << std::to_string(ticket->seatChoice) << ","
                 << ticket->bookingDate->rawDT << "\n";
        }
    }

    file.close();
}

void loadDataFromFiles()
{
    csvManager->loadAllUsers();

    csvManager->loadAllStations();
    csvManager->loadAllAirports();

    csvManager->loadAllTrains();
    csvManager->loadAllFlights();

    csvManager->loadAllFlightTickets();
    csvManager->loadAllTrainTickets();
}

// Landing Screen Manager
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
        userManager->userRegistrationPanel();
        break;
    }
    case '4':
    {
        userManager->userLoginPanel();
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

// free the allocated space in memory
void cleanMemory()
{
    for (auto t : allStationsList)
        delete (t);
    for (auto t : allAirportsList)
        delete (t);
    for (auto t : allTrainsList)
        delete (t);
    for (auto t : allFlightsList)
        delete (t);

    // delete all listnode pointers in train
    // delete all users pointers in unsermanager
    // delete all tickets pointers of each user

    delete (flightAdmin);
    delete (trainAdmin);
    delete (userManager);
}
