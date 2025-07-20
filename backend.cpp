#include "classes.h"
#include "frontend.cpp"

const std::string flight_admin_email = "flightadmin@gmail.com";
const std::string flight_admin_password = "flightadmin@123";
const std::string train_admin_email = "trainadmin@gmail.com";
const std::string train_admin_password = "trainadmin@123";

std::unordered_map<std::string, Train *> codeToTrain;
std::unordered_map<std::string, Flight *> codeToFlight;
std::unordered_map<std::string, Terminal *> codeToStation;
std::unordered_map<std::string, Terminal *> codeToAirport;
std::vector<User *> allUsersList;

FlightAdmin *flightAdmin = new FlightAdmin(flight_admin_email, flight_admin_password);
TrainAdmin *trainAdmin = new TrainAdmin(train_admin_email, train_admin_password);
UserManager *userManager = new UserManager();
CSVManager *csvManager = new CSVManager();

// Helper Functions
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

bool isStationCodeExists(std::string stationCode)
{
    if (codeToStation.count(stationCode) == 0)
        return false;
    return true;
}
bool isAirportCodeExists(std::string airportCode)
{
    if (codeToAirport.count(airportCode) == 0)
        return false;
    return true;
}
bool isTrainCodeExists(std::string trainCode)
{
    if (codeToTrain.count(trainCode) == 0)
        return false;
    return true;
}
bool isFlightCodeExists(std::string flightCode)
{
    if (codeToFlight.count(flightCode) == 0)
        return false;
    return true;
}

ListNode *terminalInTransportRoute(ListNode *coveringCities, std::string &terminalCode)
{
    while (coveringCities != NULL)
    {
        if (coveringCities->currentTerminal.code == terminalCode)
        {
            return coveringCities;
        }
        coveringCities = coveringCities->next;
    }
    return NULL;
}
std::tuple<std::vector<std::string>, std::vector<std::pair<ListNode *, ListNode *>>> filterTransportsForUser(std::string transport, std::string &boardingTerminalCode, std::string &destinationTerminalCode, DateTime journeyDate)
{
    std::vector<std::string> positions;
    std::vector<std::pair<ListNode *, ListNode *>> citiesList;

    std::string oneAndHalfDay = "01/00/00-12:00";
    DateTime *diff = stringToDateTime(oneAndHalfDay);
    DateTime validLowDate = journeyDate - (*diff);
    DateTime validHighDate = journeyDate + (*diff);

    auto temp = [&](ListNode *node, std::string number)
    {
        auto boardingTerminal = terminalInTransportRoute(node, boardingTerminalCode);
        if (boardingTerminal != NULL && validLowDate <= boardingTerminal->departureTime && validHighDate >= boardingTerminal->departureTime)
        {
            auto destinationTerminal = terminalInTransportRoute(boardingTerminal, destinationTerminalCode);
            if (destinationTerminal != NULL)
            {
                positions.push_back(number);
                citiesList.push_back({boardingTerminal, destinationTerminal});
            }
        }
    };

    if (transport == "Train")
    {
        for (auto &[_, t] : codeToTrain)
        {
            temp(t->coveringCities, t->number);
        }
    }
    else
    {
        for (auto &[_, f] : codeToFlight)
        {
            temp(f->coveringCities, f->number);
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
DateTime DateTime::operator+(const DateTime &other) const
{
    int y = year + other.year;
    int mo = month + other.month;
    int d = day + other.day;
    int h = hour + other.hour;
    int mi = minute + other.minute;

    // Normalize minutes
    if (mi >= 60)
    {
        h += mi / 60;
        mi %= 60;
    }

    // Normalize hours
    if (h >= 24)
    {
        d += h / 24;
        h %= 24;
    }

    // Normalize months
    if (mo > 12)
    {
        y += (mo - 1) / 12;
        mo = (mo - 1) % 12 + 1;
    }

    // Normalize days
    while (true)
    {
        int dim = daysInMonth[mo - 1] + (isLeapYear(y + 2000) && mo == 2 ? 1 : 0);
        if (d <= dim)
            break;
        d -= dim;
        mo++;
        if (mo > 12)
        {
            mo = 1;
            y++;
        }
    }

    return DateTime(" ", y, mo, d, h, mi);
}
DateTime DateTime::operator-(const DateTime &other) const
{
    int y1 = year, m1 = month, d1 = day, h1 = hour, min1 = minute;
    int y2 = other.year, m2 = other.month, d2 = other.day, h2 = other.hour, min2 = other.minute;

    int diffMin = min1 - min2;
    int diffHour = h1 - h2;
    int diffDay = d1 - d2;
    int diffMonth = m1 - m2;
    int diffYear = y1 - y2;

    // Normalize minutes
    if (diffMin < 0)
    {
        diffMin += 60;
        diffHour--;
    }

    // Normalize hours
    if (diffHour < 0)
    {
        diffHour += 24;
        diffDay--;
    }

    // Normalize days
    if (diffDay < 0)
    {
        // Borrow days from previous month
        m1--;
        if (m1 == 0)
        {
            m1 = 12;
            y1--;
        }
        diffDay += daysInMonth[m1 - 1] + (isLeapYear(y1 + 2000) && m1 == 2 ? 1 : 0);
        diffMonth--;
    }

    // Normalize months
    if (diffMonth < 0)
    {
        diffMonth += 12;
        diffYear--;
    }

    return DateTime(" ", diffYear, diffMonth, diffDay, diffHour, diffMin);
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
        printAlert("Logged In Successfully.");
        this->adminDashboard();
    }
    else
    {
        printAlert("Login Failed!\nInvalid Credentials.");
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
        printTransportOrTerminalSelectionPanel("Trains", "Stations", codeToTrain, codeToFlight, codeToStation);
        break;
    }
    case '2':
    {
        this->manageAddNewTransport();
        break;
    }
    case '3':
    {
        this->manageAddNewTerminal();
        break;
    }
    case '4':
    {
        this->manageRemoveTransport();
        break;
    }
    case '5':
    {
        this->manageRemoveTerminal();
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
void TrainAdmin::manageAddNewTerminal()
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
    else if (isStationCodeExists(code))
    {
        message = "Station Code already exists.";
    }
    else
    {
        Terminal *newStation = new Terminal(name, city, code);
        codeToStation[code] = newStation;
        // writing into file
        csvManager->saveNewStation(newStation);
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
void TrainAdmin::manageAddNewTransport()
{
    int stationsCount;
    std::string name, number, coveringCitiesCode, departureTimes, totalSeats;
    printAddNewTransportPanel("Train", stationsCount, name, number, coveringCitiesCode, departureTimes, totalSeats);

    transform(coveringCitiesCode.begin(), coveringCitiesCode.end(), coveringCitiesCode.begin(), ::toupper);

    std::string failedMessage = "Failed to add Train!\n";
    if (stationsCount == -1)
    {
        printAlert(failedMessage + "Cities count should be integer");
    }
    else if (name.size() == 0)
    {
        printAlert(failedMessage + "Train Name is Required");
    }
    else if (number.size() == 0)
    {
        printAlert(failedMessage + "Train Number is Required");
    }
    else if (!isOnlyAlphas(name, " "))
    {
        printAlert(failedMessage + "Train Name can only have alphabets and underscores: " + name);
    }
    else if (number.size() != 5 || !isOnlyDigits(number))
    {
        printAlert(failedMessage + "Train Number is Invalid");
    }
    else if (isTrainCodeExists(number))
    {
        printAlert(failedMessage + "Train Number already exists.");
    }
    else if (!isOnlyDigits(totalSeats, " "))
    {
        printAlert(failedMessage + "Number of seats is invalid");
    }
    else
    {
        // separating cities
        std::vector<std::string> stationCodes = splitAsStrings(coveringCitiesCode);
        std::vector<Terminal *> coveringStations;
        if (stationCodes.size() != stationsCount)
        {
            printAlert(failedMessage + "No. of cities provided does not match cities count");
            return;
        }
        for (std::string &stationCode : stationCodes)
        {
            if (!isStationCodeExists(stationCode))
            {
                printAlert(failedMessage + "A station does not exists: " + stationCode);
                return;
            }
            coveringStations.push_back(codeToStation[stationCode]);
        }

        // separating destination times of stations
        std::vector<std::string> stationTimes = splitAsStrings(departureTimes);
        std::vector<DateTime *> stationDTs;
        if (stationTimes.size() != stationsCount)
        {
            printAlert(failedMessage + "No. of departure times provided does not match cities count");
            return;
        }
        // checking and storing destination times for each city
        for (int i = 0; i < stationsCount; i++)
        {
            DateTime *DT = stringToDateTime(stationTimes[i]);
            if (DT == NULL)
            {
                printAlert(failedMessage + "A date time format is wrong : " + stationTimes[i]);
                return;
            }
            if (i > 0 && (*DT) <= (*stationDTs.back()))
            {
                printAlert(failedMessage + "A date time difference is equal or negative: " + stationTimes[i - 1] + " " + stationTimes[i]);
                return;
            }
            stationDTs.push_back(DT);
        }

        // checking and storing no. of seats in each seat class
        std::vector<int> seatsInInt = splitAsNums(totalSeats);
        if (seatsInInt.size() != 4)
        {
            printAlert(failedMessage + "Seats provided does not match.");
            return;
        }

        // adding station to the covering cities list
        ListNode *coveringCities = new ListNode();
        ListNode *movingPtr = coveringCities;
        for (int i = 0; i < stationsCount; i++)
        {
            movingPtr->next = new ListNode;
            movingPtr = movingPtr->next;
            movingPtr->currentTerminal = *coveringStations[i];
            movingPtr->departureTime = *stationDTs[i];
        }

        Train *newTrain = new Train(name, number, coveringCities->next, seatsInInt);
        codeToTrain[number] = newTrain;
        // writing in file
        csvManager->saveNewTrain(newTrain);
        printAlert("Train added successfully");
    }
}
void TrainAdmin::manageRemoveTerminal()
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
        if (isStationCodeExists(stationCode))
        {
            codeToStation.erase(stationCode);
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
        printAlert("Station removed successfully.");
    }
    else
    {
        printAlert("Failed to remove Station.\n" + message);
    }
}
void TrainAdmin::manageRemoveTransport()
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
        if (isTrainCodeExists(trainNumber))
        {
            codeToTrain.erase(trainNumber);
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
        printTransportOrTerminalSelectionPanel("Flights", "Airports", codeToTrain, codeToFlight, codeToAirport);
        break;
    }
    case '2':
    {
        this->manageAddNewTransport();
        break;
    }
    case '3':
    {
        this->manageAddNewTerminal();
        break;
    }
    case '4':
    {
        this->manageRemoveTransport();
        break;
    }
    case '5':
    {
        this->manageRemoveTerminal();
        break;
    }
    case '6':
    {
        printAlert("Logged Out Successfully.");
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
void FlightAdmin::manageAddNewTerminal()
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
    else if (isAirportCodeExists(code))
    {
        message = "Airport Code already exists.";
    }
    else
    {
        Terminal *newAirport = new Terminal(name, city, code);
        codeToAirport[code] = newAirport;
        // writing into file
        csvManager->saveNewAirport(newAirport);
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
void FlightAdmin::manageAddNewTransport()
{
    int airportsCount;
    std::string name, number, coveringCitiesCode, departureTimes, totalSeats;
    printAddNewTransportPanel("Flight", airportsCount, name, number, coveringCitiesCode, departureTimes, totalSeats);

    transform(number.begin(), number.end(), number.begin(), ::toupper);
    transform(coveringCitiesCode.begin(), coveringCitiesCode.end(), coveringCitiesCode.begin(), ::toupper);

    std::string failedMessage = "Failed to add Flight!\n";
    if (airportsCount == -1)
    {
        printAlert(failedMessage + "Cities count should be integer");
    }
    else if (name.size() == 0)
    {
        printAlert(failedMessage + "Flight Name is Required");
    }
    else if (number.size() != 5 || !isOnlyDigitOrAlpha(number))
    {
        printAlert(failedMessage + "Flight Number is Invalid");
    }
    else if (isFlightCodeExists(number))
    {
        printAlert(failedMessage + "Flight Number already exists.");
    }
    else if (!isOnlyDigits(totalSeats, " "))
    {
        printAlert(failedMessage + "Number of seat is invalid.");
    }
    else
    {
        // separating cities
        std::vector<std::string> airportCodes = splitAsStrings(coveringCitiesCode);
        std::vector<Terminal *> coveringAirports;
        if (airportCodes.size() != airportsCount)
        {
            printAlert(failedMessage + "No. of cities provided does not match cities count.");
            return;
        }
        for (std::string &airportCode : airportCodes)
        {
            if (!isAirportCodeExists(airportCode))
            {
                printAlert(failedMessage + "An airport does not exists: " + airportCode);
                return;
            }
            coveringAirports.push_back(codeToAirport[airportCode]);
        }

        // separating destination times of airports
        std::vector<std::string> airportTimes = splitAsStrings(departureTimes);
        std::vector<DateTime *> airportDTs;
        if (airportTimes.size() != airportsCount)
        {
            printAlert(failedMessage + "No. of departure times provided does not match cities count.");
            return;
        }
        // checking and storing destination times for each city
        for (int i = 0; i < airportsCount; i++)
        {
            DateTime *DT = stringToDateTime(airportTimes[i]);
            if (DT == NULL)
            {
                printAlert(failedMessage + "A date time format is wrong : " + airportTimes[i]);
                return;
            }
            if (i > 0 && (*DT) <= (*airportDTs.back()))
            {
                printAlert(failedMessage + "A date time difference is equal or negative: " + airportTimes[i - 1] + " " + airportTimes[i]);
                return;
            }
            airportDTs.push_back(DT);
        }

        // checking and adding no. of seats in each class
        std::vector<int> seatsInInt = splitAsNums(totalSeats);
        if (seatsInInt.size() != airportsCount)
        {
            printAlert(failedMessage + "Seats provided does not match.");
            return;
        }

        // validating and adding airports to the list
        ListNode *coveringCities = new ListNode();
        ListNode *movingPtr = coveringCities;
        for (int i = 0; i < airportsCount; i++)
        {
            movingPtr->next = new ListNode();
            movingPtr = movingPtr->next;
            movingPtr->currentTerminal = *coveringAirports[i];
            movingPtr->departureTime = *airportDTs[i];
        }

        Flight *newFlight = new Flight(name, number, coveringCities->next, seatsInInt);
        codeToFlight[number] = newFlight;
        // write in file
        csvManager->saveNewFlight(newFlight);
        printAlert("Flight added successfully");
    }
}
void FlightAdmin::manageRemoveTerminal()
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
        if (isAirportCodeExists(airportCode))
        {
            codeToAirport.erase(airportCode);
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
        printAlert("Airport removed successfully.");
    }
    else
    {
        printAlert("Failed to remove Airport!\n" + message);
    }
}
void FlightAdmin::manageRemoveTransport()
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
        if (isFlightCodeExists(flightNumber))
        {
            codeToFlight.erase(flightNumber);
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
        printAlert("Failed to remove Flight!\n" + message);
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
UserManager::UserManager() : TicketManager()
{
    this->loggedInUser = NULL;
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
    bool success = false;

    if (inputs[0].size() == 0)
    {
        message = "Name is required.";
    }
    else if (inputs[1].size() == 0)
    {
        message = "Email is required.";
    }
    else if (inputs[2].size() == 0)
    {
        message = "Password is required.";
    }
    else if (inputs[2] != inputs[3])
    {
        message = "Password and confirm password does not match.";
    }
    else
    {
        if (this->isEmailRegistered(inputs[1]))
        {
            message = "User Email already exists.";
        }
        else
        {
            User *newUser = new User(inputs[0], inputs[1], inputs[2]);
            allUsersList.push_back(newUser);
            // writing into file
            csvManager->saveNewUser(newUser);
            success = true;
        }
    }

    if (success)
    {
        printAlert("You are successfully registered.");
    }
    else
    {
        printAlert("Registration Failed!\n" + message);
    }
}
void UserManager::userLoginPanel()
{
    std::vector<std::string> inputs = printLoginPanel("User");

    std::string message;
    bool success = false;

    if (inputs[0].size() == 0)
    {
        message = "Email is required.";
    }
    else if (inputs[1].size() == 0)
    {
        message = "Password is required.";
    }
    else
    {
        User *tempPtr = findUser(inputs[0]);
        if (tempPtr != NULL && inputs[1] == tempPtr->getPassword())
        {
            loggedInUser = tempPtr;
            success = true;
        }
        else
        {
            message = "Invalid Credentials";
        }
    }

    if (success)
    {
        printAlert("Logged In successfully.");
        this->userDashboard();
    }
    else
    {
        printAlert("Login Failed!\n" + message);
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
        printTransportOrTerminalSelectionPanel("Trains", "Stations", codeToTrain, codeToFlight, codeToStation);
        break;
    }
    case '2':
    {
        printTransportOrTerminalSelectionPanel("Flights", "Airports", codeToTrain, codeToFlight, codeToAirport);
        break;
    }
    case '3':
    {
        this->ticketBookingPanel(this->loggedInUser);
        break;
    }
    case '4':
    {
        this->ticketViewingPanel(this->loggedInUser);
        break;
    }
    case '5':
    {
        this->ticketCancellingPanel(this->loggedInUser);
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
        printAlert("Logged Out Successfully.");
        return;
    }
    default:
    {
        printAlert("Wrong Choice!");
        break;
    }
    }
    this->userDashboard();
}
void UserManager::changeUserPassword()
{
    std::vector<std::string> info = printChangePasswordPanel();
    std::string message;
    bool success = false;
    if (info[1].size() == 0)
    {
        message = "Password is required.";
    }
    else if (info[1] != info[2])
    {
        message = "New password and Confirm password does not match.";
    }
    else if (info[0] != this->loggedInUser->getPassword())
    {
        message = "Wrong Old Password.";
    }
    else
    {
        loggedInUser->setPassword(info[1]);
        // writing in file
        csvManager->saveAllUsers();
        success = true;
    }

    if (success)
    {
        printAlert("Password Changed Successfully");
    }
    else
    {
        printAlert("Failed to change password!\n" + message);
    }
}
void UserManager::changeUserName()
{
    std::string newName = printChangeNamePanel(this->loggedInUser->fullName);
    std::string message;
    if (newName.size() == 0)
        message = "Failed to change name!\nName is required.";
    else
    {
        this->loggedInUser->fullName = newName;
        // writing in file
        csvManager->saveAllUsers();
        message = "Name Updated Successfully";
    }
    printAlert(message);
}

// TicketManager Class
void TicketManager::ticketViewingPanel(User *user)
{
    printTicketTransportSelection("View");
    char choice2 = inputUserChoice();
    switch (choice2)
    {
    case '1':
    {
        printAllTrainTickets(user->trainTickets);
        printContinue();
        break;
    }
    case '2':
    {
        printAllFlightTickets(user->flightTickets);
        printContinue();
        break;
    }
    case '3':
    {
        break;
    }
    default:
    {
        printAlert("Wrong Choice!");
        break;
    }
    }
}
void TicketManager::ticketBookingPanel(User *user)
{
    printBookingTransportSelection();
    char choice2 = inputUserChoice();
    switch (choice2)
    {
    case '1':
    {
        this->trainTicketBooking(user);
        break;
    }
    case '2':
    {
        this->flightTicketBooking(user);
        break;
    }
    case '3':
    {
        break;
    }
    deafult:
    {
        printAlert("Wrong Choice!");
        this->ticketBookingPanel(user);
    }
    }
}
void TicketManager::ticketCancellingPanel(User *user)
{
    printTicketTransportSelection("Cancel");
    char choice2 = inputUserChoice();
    switch (choice2)
    {
    case '1':
    {
        this->trainTicketCancelling(user->trainTickets);
        break;
    }
    case '2':
    {
        this->flightTicketCancelling(user->flightTickets);
        break;
    }
    case '3':
    {
        break;
    }
    default:
    {
        printAlert("Wrong Choice!");
        this->ticketCancellingPanel(user);
        break;
    }
    }
}

void TicketManager::trainTicketBooking(User *user)
{
    std::vector<std::string> inputs = printTicketBookingPanel("Train");
    inputs[2] += "-00:00";
    DateTime *journeyDate = stringToDateTime(inputs[2]);

    std::string message;
    bool success = false;

    if (journeyDate == NULL)
    {
        message = "Invalid Journey Date";
    }
    else if (!isOnlyAlphas(inputs[0]) || inputs[0].size() > 4 || !isOnlyAlphas(inputs[1]) || inputs[1].size() > 4)
    {
        message = "Invalid Code.";
    }
    else
    {
        transform(inputs[0].begin(), inputs[0].end(), inputs[0].begin(), ::toupper);
        transform(inputs[1].begin(), inputs[1].end(), inputs[1].begin(), ::toupper);

        // search for Train;
        auto [filteredTrainsCode, fromToList] = filterTransportsForUser("Train", inputs[0], inputs[1], *journeyDate);

        // showing all available trains
        printAllTrains(codeToTrain, true, filteredTrainsCode);

        // If no trains are available then return
        if (filteredTrainsCode.size() == 0)
        {
            printContinue();
            return;
        }

        // taking user's train choice
        int trainChoice = inputNumber("Enter Train Choice: ");
        if (trainChoice < 0 || trainChoice > filteredTrainsCode.size())
        {
            message = "Invalid Choice.";
        }
        else
        {
            trainChoice--;
            Train *choosenTrain = codeToTrain[filteredTrainsCode[trainChoice]];
            std::string trainName = choosenTrain->name;

        trainSeatSelectionCheckPoint:
            printSeatTypeSelectionPanel("Train", trainName);
            int seatChoice = inputNumber();
            if (seatChoice < 0 || seatChoice > 5)
            {
                printContinue("Invalid Choice!");
                goto trainSeatSelectionCheckPoint;
            }
            else if (seatChoice == 5)
            {
                message = "Booking Canceled.";
            }
            else
            {
                seatChoice--;
                if (choosenTrain->totalSeats[seatChoice] > choosenTrain->bookedSeats[seatChoice])
                {
                    std::string pnr = generateTrainTicketPNR();
                    DateTime *bookingDate = getCurrentDateTime();

                    TrainTicket *newTicket = new TrainTicket(seatChoice, choosenTrain, fromToList[trainChoice].first, fromToList[trainChoice].second, pnr, bookingDate, user->fullName, user->email);
                    user->trainTickets.push_back(newTicket);
                    choosenTrain->bookedSeats[seatChoice]++;
                    // write in file;
                    csvManager->saveNewTrainTicket(newTicket, user->email);
                    success = true;
                }
                else
                {
                    message = "All seats of this class were booked.";
                }
            }
        }
    }

    if (success)
    {
        printAlert("Seat Booked Successfully.");
    }
    else
    {
        printAlert("Booking Failed!\n" + message);
    }
}
void TicketManager::trainTicketCancelling(std::vector<TrainTicket *> &trainTickets)
{

    printAllTrainTickets(trainTickets);

    bool success = false;
    std::string message;

    int ticketsCt = trainTickets.size();
    if (ticketsCt == 0)
    {
        printContinue();
        return;
    }
    else
    {
        int ticketChoice = inputNumber("Enter choice number to cancel ticket: ");
        if (ticketChoice < 0 || ticketChoice > ticketsCt)
        {
            message = "Invalid Choice!";
        }
        else
        {
            ticketChoice--;

            // increment seats in train
            int seatChoice = trainTickets[ticketChoice]->seatChoice;
            trainTickets[ticketChoice]->trainPtr->bookedSeats[seatChoice]--;

            // delete ticket from trainTickets
            TrainTicket *tT = trainTickets[ticketChoice];
            trainTickets.erase(trainTickets.begin() + ticketChoice);

            // updating file data
            csvManager->saveAllTrainTickets();

            success = true;
            delete tT;
        }
    }

    if (success)
    {
        printAlert("Ticket Canceled Successfully.");
    }
    else
    {
        printAlert("Failed to cancel Ticket!\n" + message);
    }
}
void TicketManager::flightTicketBooking(User *user)
{
    std::vector<std::string> inputs = printTicketBookingPanel("Flight");
    inputs[2] += "-00:00";
    DateTime *journeyDate = stringToDateTime(inputs[2]);

    std::string message;
    bool success = false;

    if (journeyDate == NULL)
    {
        message = "Invalid Journey Date";
    }
    else if (!isOnlyAlphas(inputs[0]) || inputs[0].size() > 4 || !isOnlyAlphas(inputs[1]) || inputs[1].size() > 4)
    {
        message = "Invalid Code.";
    }
    else
    {
        transform(inputs[0].begin(), inputs[0].end(), inputs[0].begin(), ::toupper);
        transform(inputs[1].begin(), inputs[1].end(), inputs[1].begin(), ::toupper);

        // search for Flight
        auto [filteredFlightsCode, fromToList] = filterTransportsForUser("Flight", inputs[0], inputs[1], *journeyDate);

        // showing all available flights
        printAllFlights(codeToFlight, true, filteredFlightsCode);

        // If no flights are available then return
        if (filteredFlightsCode.size() == 0)
        {
            printContinue();
            return;
        }

        // taking user's flight choice
        int flightChoice = inputNumber("Enter Flight Choice: ");
        if (flightChoice < 0 || flightChoice > filteredFlightsCode.size())
        {
            message = "Invalid Choice.";
        }
        else
        {
            flightChoice--;
            Flight *choosenFlight = codeToFlight[filteredFlightsCode[flightChoice]];
            std::string flightName = choosenFlight->name;

        flightSeatSelectionCheckPoint:
            printSeatTypeSelectionPanel("Flight", flightName);
            int seatChoice = inputNumber();
            if (seatChoice < 0 || seatChoice > 3)
            {
                printContinue("Invalid Choice!");
                goto flightSeatSelectionCheckPoint;
            }
            else if (seatChoice == 3)
            {
                message = "Booking Canceled!";
            }
            else
            {
                seatChoice--;
                if (choosenFlight->totalSeats[seatChoice] > choosenFlight->bookedSeats[seatChoice])
                {
                    std::string pnr = generateFlightTicketPNR();
                    DateTime *bookingDate = getCurrentDateTime();

                    FlightTicket *newTicket = new FlightTicket(seatChoice, choosenFlight, fromToList[flightChoice].first, fromToList[flightChoice].second, pnr, bookingDate, user->fullName, user->email);
                    user->flightTickets.push_back(newTicket);
                    choosenFlight->bookedSeats[seatChoice]++;
                    // write in file;
                    csvManager->saveNewFlightTicket(newTicket, user->email);
                    success = true;
                }
                else
                {
                    message = "All seats in this class were booked.";
                }
            }
        }
    }

    if (success)
    {
        printAlert("Seat Booked Successfully.");
    }
    else
    {
        printAlert("Booking Failed!\n" + message);
    }
}
void TicketManager::flightTicketCancelling(std::vector<FlightTicket *> &flightTickets)
{
    int ticketsCt;

    printAllFlightTickets(flightTickets);

    bool success = false;
    std::string message;

    ticketsCt = flightTickets.size();
    if (ticketsCt == 0)
    {
        printContinue();
        return;
    }
    else
    {
        int ticketChoice = inputNumber("Enter choice number to cancel ticket: ");
        if (ticketChoice < 0 || ticketChoice > ticketsCt)
        {
            message = "Invalid Choice!";
        }
        else
        {
            ticketChoice--;

            // increment seats in flight
            int seatChoice = flightTickets[ticketChoice]->seatChoice;
            flightTickets[ticketChoice]->flightPtr->bookedSeats[seatChoice]--;

            // delete ticket from flightTickets;
            FlightTicket *fT = flightTickets[ticketChoice];
            flightTickets.erase(flightTickets.begin() + ticketChoice);

            // update in file
            csvManager->saveAllFlightTickets();

            success = true;
            delete fT;
        }
    }

    if (success)
    {
        printAlert("Ticket Canceled Successfully.");
    }
    else
    {
        printAlert("Failed to cancel Ticket!\n" + message);
    }
}

// Terminal Class
Terminal::Terminal(std::string name, std::string city, std::string code)
{
    this->name = name;
    this->city = city;
    this->code = code;
}

// FlightTicket Class
FlightTicket::FlightTicket(int seatChoice, Flight *flightPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr, DateTime *bookingDate, std::string name, std::string email)
    : flightPtr(flightPtr)
{
    this->seatChoice = seatChoice;
    this->pnr = pnr;
    this->boardingListNode = boardingListNode;
    this->destinationListNode = destinationListNode;
    this->bookingDate = bookingDate;
    this->price = flightPtr->seatPrices[seatChoice];
    this->passengerName = name;
    this->passengerEmail = email;
}

// TrainTicket Class
TrainTicket::TrainTicket(int seatChoice, Train *trainPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr, DateTime *bookingDate, std::string name, std::string email)
    : trainPtr(trainPtr)
{
    this->seatChoice = seatChoice;
    this->pnr = pnr;
    this->boardingListNode = boardingListNode;
    this->destinationListNode = destinationListNode;
    this->bookingDate = bookingDate;
    this->price = trainPtr->seatPrices[seatChoice];
    this->passengerName = name;
    this->passengerEmail = email;
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
            codeToStation[code] = new Terminal(name, city, code);
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
            codeToAirport[code] = new Terminal(name, city, code);
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
        std::string number, name, totalSeatsStr, airportsStr, timesStr;

        if (std::getline(ss, number, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, totalSeatsStr, ',') &&
            std::getline(ss, airportsStr, ',') &&
            std::getline(ss, timesStr, ','))
        {
            std::vector<int> totalSeats = splitAsNums(totalSeatsStr, '|');
            std::vector<std::string> cityCodes = splitAsStrings(airportsStr, '|');
            std::vector<std::string> cityTimes = splitAsStrings(timesStr, '|');

            ListNode *citiesList = new ListNode();
            ListNode *movingPtr = citiesList;
            for (int i = 0; i < cityCodes.size(); i++)
            {
                if (!isAirportCodeExists(cityCodes[i]))
                    continue;

                movingPtr->next = new ListNode();
                movingPtr = movingPtr->next;
                movingPtr->currentTerminal = *codeToAirport[cityCodes[i]];
                movingPtr->departureTime = *stringToDateTime(cityTimes[i]);
            }

            codeToFlight[number] = new Flight(name, number, citiesList->next, totalSeats);
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
        std::string number, name, totalSeatsStr, stationsStr, timesStr;

        if (std::getline(ss, number, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, totalSeatsStr, ',') &&
            std::getline(ss, stationsStr, ',') &&
            std::getline(ss, timesStr, ','))
        {
            std::vector<int> totalSeats = splitAsNums(totalSeatsStr, '|');
            std::vector<std::string> stationCodes = splitAsStrings(stationsStr, '|');
            std::vector<std::string> departureTimes = splitAsStrings(timesStr, '|');

            ListNode *citiesList = new ListNode();
            ListNode *movingPtr = citiesList;
            for (int i = 0; i < stationCodes.size(); i++)
            {
                if (!isStationCodeExists(stationCodes[i]))
                    continue;

                movingPtr->next = new ListNode();
                movingPtr = movingPtr->next;
                movingPtr->currentTerminal = *codeToStation[stationCodes[i]];
                movingPtr->departureTime = *stringToDateTime(departureTimes[i]);
            }

            codeToTrain[number] = new Train(name, number, citiesList->next, totalSeats);
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
            if (!isFlightCodeExists(flightNumber))
                continue;
            Flight *flight = codeToFlight[flightNumber];
            ListNode *boardingTerminal = terminalInTransportRoute(flight->coveringCities, boarding);
            ListNode *destinationTerminal = terminalInTransportRoute(boardingTerminal, destination);
            if (boardingTerminal == NULL || destinationTerminal == NULL)
                continue;

            User *user = userManager->findUser(email);
            int seatType = stoi(seatChoice);
            DateTime *bookingDateTime = stringToDateTime(bookigDate);

            user->flightTickets.push_back(new FlightTicket(seatType, flight, boardingTerminal, destinationTerminal, pnr, bookingDateTime, user->fullName, user->email));
            flight->bookedSeats[seatType]++;
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
            if (!isTrainCodeExists(trainNumber))
                continue;
            Train *train = codeToTrain[trainNumber];
            User *user = userManager->findUser(email);
            ListNode *boardingTerminal = terminalInTransportRoute(train->coveringCities, boarding);
            ListNode *destinationTerminal = terminalInTransportRoute(boardingTerminal, destination);
            if (boardingTerminal == NULL || destinationTerminal == NULL)
                continue;

            int seatType = stoi(seatChoice);
            DateTime *bookingDateTime = stringToDateTime(bookigDate);
            auto terminals = terminalInTransportRoute(train->coveringCities, boarding);

            train->bookedSeats[seatType]++;
            user->trainTickets.push_back(new TrainTicket(seatType, train, boardingTerminal, destinationTerminal, pnr, bookingDateTime, user->fullName, user->email));
        }
    }

    file.close();
}

void CSVManager ::saveNewUser(User *newUser)
{
    std::ofstream file("data/users.csv", std::ios::app);

    if (!file.is_open())
    {
        printAlert("Error opening users file for appending.");
        return;
    }

    file << newUser->fullName << "," << newUser->email << "," << newUser->getPassword() << "\n";

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
void CSVManager ::saveNewStation(Terminal *newStation)
{
    std::ofstream file("data/stations.csv", std::ios::app);

    if (!file.is_open())
    {
        printAlert("Error opening stations file for appending.");
        return;
    }

    file << newStation->code << "," << newStation->name << "," << newStation->city << "\n";

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

    for (const auto &[_, station] : codeToStation)
    {
        file << station->code << "," << station->name << "," << station->city << "\n";
    }

    file.close();
}
void CSVManager ::saveNewAirport(Terminal *newAirport)
{
    std::ofstream file("data/airports.csv", std::ios::app);

    if (!file.is_open())
    {
        printAlert("Error opening airports file for appending.");
        return;
    }

    file << newAirport->code << "," << newAirport->name << "," << newAirport->city << "\n";

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

    for (const auto &[_, airport] : codeToAirport)
    {
        file << airport->code << "," << airport->name << "," << airport->city << "\n";
    }

    file.close();
}
void CSVManager ::saveNewTrain(Train *newTrain)
{
    std::ofstream file("data/trains.csv", std::ios::app);
    if (!file.is_open())
    {
        printAlert("Error opening trains file for appending.");
        return;
    }

    std::string totalSeats = join(newTrain->totalSeats, '|');
    auto coveringCities = join(newTrain->coveringCities, '|');

    file << newTrain->number << ","
         << newTrain->name << ","
         << totalSeats << ","
         << coveringCities.first << ","
         << coveringCities.second << "\n";

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

    for (const auto &[_, train] : codeToTrain)
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
void CSVManager ::saveNewFlight(Flight *newFlight)
{
    std::ofstream file("data/flights.csv", std::ios::app);
    if (!file.is_open())
    {
        printAlert("Error opening flights file for appending.");
        return;
    }

    std::string totalSeats = join(newFlight->totalSeats, '|');
    auto coveringCities = join(newFlight->coveringCities, '|');

    file << newFlight->number << ","
         << newFlight->name << ","
         << totalSeats << ","
         << coveringCities.first << ","
         << coveringCities.second << "\n";

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

    for (const auto &[_, flight] : codeToFlight)
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
void CSVManager ::saveNewTrainTicket(TrainTicket *newTicket, std::string email)
{
    std::ofstream file("data/train_tickets.csv", std::ios::app);
    if (!file.is_open())
    {
        printAlert("Error opening train tickets file for appending.");
        return;
    }
    file << email << ","
         << newTicket->pnr << ","
         << newTicket->trainPtr->number << ","
         << newTicket->boardingListNode->currentTerminal.code << ","
         << newTicket->destinationListNode->currentTerminal.code << ","
         << std::to_string(newTicket->seatChoice) << ","
         << newTicket->bookingDate->rawDT << "\n";

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
void CSVManager ::saveNewFlightTicket(FlightTicket *newTicket, std::string email)
{
    std::ofstream file("data/flight_tickets.csv", std::ios::app);
    if (!file.is_open())
    {
        printAlert("Error opening flight tickets file for appending.");
        return;
    }

    file << email << ","
         << newTicket->pnr << ","
         << newTicket->flightPtr->number << ","
         << newTicket->boardingListNode->currentTerminal.code << ","
         << newTicket->destinationListNode->currentTerminal.code << ","
         << std::to_string(newTicket->seatChoice) << ","
         << newTicket->bookingDate->rawDT << "\n";

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
        printTransportOrTerminalSelectionPanel("Trains", "Stations", codeToTrain, codeToFlight, codeToStation);
        break;
    }
    case '2':
    {
        printTransportOrTerminalSelectionPanel("Flights", "Airports", codeToTrain, codeToFlight, codeToAirport);
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
            printAlert("Wrong Choice!");
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
        printAlert("Wrong Choice!");
        break;
    }
    }
    ControlPanel();
}

// free the allocated space in memory
void cleanMemory()
{
    for (const auto &[_, t] : codeToStation)
        delete t;
    for (const auto &[_, t] : codeToAirport)
        delete t;
    for (const auto &[_, t] : codeToTrain)
        delete t;
    for (const auto &[_, t] : codeToFlight)
        delete t;

    // delete all listnode pointers in train
    // delete all users pointers in unsermanager
    // delete all tickets pointers of each user

    codeToStation.clear();
    codeToAirport.clear();
    codeToTrain.clear();
    codeToFlight.clear();

    delete flightAdmin;
    delete trainAdmin;
    delete userManager;
    delete csvManager;
}
