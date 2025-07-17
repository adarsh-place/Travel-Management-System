#include <bits/stdc++.h>
#include <conio.h>

std::vector<std::string> trainSeatTypes = {"First AC", "Second AC", "Third AC", "Sleeper Class"};
std::vector<std::string> flightSeatTypes = {"Economy Class", "Business Class"};

void ignoreUptoNewLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
std::string putInCenter(std::string str, int width = 78)
{
    int leftSpace = width - str.size();
    str = std::string(leftSpace / 2, ' ') + str + std::string((leftSpace + 1) / 2, ' ');
    return str;
}
std::string putLeftPaddingAndFillRight(std::string str, int padding = 0, int width = 78)
{
    str = std::string(padding, ' ') + str + std::string(width - str.size() - padding, ' ');
    return str;
}

void printContinue(std::string message = "")
{
    std::cout << "  " << message << "\n  Press any key to continue.";
    _getch();
}
void printAlert(std::string message)
{
    system("cls");
    std::cout << "  " << message << "\n";
    printContinue();
}
void printExit()
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Thank you for visiting") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}

char inputUserChoice(std::string text = "Enter choice: ")
{
    std::cout << "\n"
              << "  " << text;
    char choice = _getch();
    return choice;
}
int inputNumber(std::string text = "Enter choice: ")
{
    std::string input;
    std::cout << "\n"
              << "  " << text;
    std::cin >> input;
    ignoreUptoNewLine();
    for (char &ch : input)
    {
        if (!isdigit(ch))
            return -1;
    }
    return stoi(input);
}
std::string inputHashedPassword()
{
    std::string password;
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '\r' || ch == '\n') // Enter key
        {
            std::cout << "\n";
            break;
        }
        else if (ch == '\b' || ch == 127) // Backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else if (isprint(static_cast<unsigned char>(ch)))
        {
            password += ch;
            std::cout << '*';
        }
    }
    return password;
}

void printLandingPanel()
{
    system("cls");

    //                                             80
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Travel Management System") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║        1. View trains and stations details                                   ║" << "\n";
    std::cout << "║        2. View flights and airport details                                   ║" << "\n";
    std::cout << "║        3. Register                                                           ║" << "\n";
    std::cout << "║        4. User Login                                                         ║" << "\n";
    std::cout << "║        5. Admin Login                                                        ║" << "\n";
    std::cout << "║        6. Quit                                                               ║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}

void printTrainDetails(int count, Train *train)
{
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "║" << putInCenter(std::to_string(count) + ". " + train->name + " (" + train->number + ")") << "║\n";
    std::cout << "║" << putInCenter("") << "║" << "\n";

    std::cout << "║" << putLeftPaddingAndFillRight("Covering Stations: ", 4) << "║" << "\n";

    ListNode *tempNode = train->coveringCities;
    int cityCount = 1;
    while (tempNode != NULL)
    {
        std::cout << "║" << putLeftPaddingAndFillRight("(" + std::to_string(cityCount) + ") " + tempNode->currentTerminal.city + ", " + tempNode->currentTerminal.code + " : " + tempNode->departureTime.rawDT, 4) << "║" << "\n";
        tempNode = tempNode->next;
        cityCount++;
        if (tempNode != NULL)
            std::cout << "║     ↓                                                                        ║" << "\n";
    }

    std::cout << "║" << putInCenter("") << "║" << "\n";

    std::cout << "║       ┌────────────────┬────────────┬────────────────────────────────┐       ║\n";
    std::cout << "║       │   Seat Type    │   Price    │     Total Seats(Available)     │       ║\n";
    std::cout << "║       ├────────────────┼────────────┼────────────────────────────────┤       ║\n";
    std::cout << "║       │    First AC    │" << putInCenter("Rs. " + std::to_string(train->seatPrices[0]), 12) << "│" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[0]) + "(" + std::to_string(train->totalSeats[0] - train->bookedSeats[0]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │    Second AC   │" << putInCenter("Rs. " + std::to_string(train->seatPrices[1]), 12) << "│" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[1]) + "(" + std::to_string(train->totalSeats[1] - train->bookedSeats[1]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │    Third AC    │" << putInCenter("Rs. " + std::to_string(train->seatPrices[2]), 12) << "│" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[2]) + "(" + std::to_string(train->totalSeats[2] - train->bookedSeats[2]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │  Sleeper Class │" << putInCenter("Rs. " + std::to_string(train->seatPrices[3]), 12) << "│" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[3]) + "(" + std::to_string(train->totalSeats[3] - train->bookedSeats[3]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       └────────────────┴────────────┴────────────────────────────────┘       ║\n";
}
void printAllTrains(std::vector<Train *> allTrainsList, bool printPositions = false, std::vector<int> positions = {})
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("All Available Trains") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    int count = 1;
    if (!printPositions)
    {
        if (allTrainsList.size() == 0)
        {
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("No Trains are scheduled!") << "║" << "\n";
        }
        else
        {
            for (auto train : allTrainsList)
            {
                if (count != 1)
                {
                    std::cout << "║" << "  --------------------------------------------------------------------------  " << "║" << "\n";
                }
                printTrainDetails(count, train);
                count++;
            }
        }
    }
    else
    {
        if (positions.size() == 0)
        {
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("No Trains are scheduled around that date!") << "║" << "\n";
        }
        else
        {
            for (int index : positions)
            {
                if (count != 1)
                {
                    std::cout << "║" << "  --------------------------------------------------------------------------  " << "║" << "\n";
                }
                printTrainDetails(count, allTrainsList[index]);
                count++;
            }
        }
    }
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printFlightDetails(int count, Flight *flight)
{
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "║" << putInCenter(std::to_string(count) + ". " + flight->name + " (" + flight->number + ")") << "║\n";
    std::cout << "║" << putInCenter("") << "║" << "\n";

    ListNode *tempNode = flight->coveringCities;
    std::cout << "║" << putLeftPaddingAndFillRight("Boarding Airport: " + tempNode->currentTerminal.city + ", " + tempNode->currentTerminal.code, 4) << "║\n";
    std::cout << "║" << putLeftPaddingAndFillRight("Boarding Date and Time: " + tempNode->departureTime.rawDT, 4) << "║\n";

    std::cout << "║" << putInCenter("") << "║" << "\n";

    tempNode = tempNode->next;
    std::cout << "║" << putLeftPaddingAndFillRight("Destination Airport: " + tempNode->currentTerminal.city + ", " + tempNode->currentTerminal.code, 4) << "║\n";
    std::cout << "║" << putLeftPaddingAndFillRight("Arrival Date and Time: " + tempNode->departureTime.rawDT, 4) << "║\n";

    std::cout << "║" << putInCenter("") << "║" << "\n";

    //                            16             12                   32
    std::cout << "║       ┌────────────────┬────────────┬────────────────────────────────┐       ║\n";
    std::cout << "║       │   Seat Type    │    Price   │     Total Seats(Available)     │       ║\n";
    std::cout << "║       ├────────────────┼────────────┼────────────────────────────────┤       ║\n";
    std::cout << "║       │ Economy Class  │" << putInCenter("Rs. " + std::to_string(flight->seatPrices[0]), 12) << "│" + putLeftPaddingAndFillRight(std::to_string(flight->totalSeats[0]) + "(" + std::to_string(flight->totalSeats[0] - flight->bookedSeats[0]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │ Business Class │" << putInCenter("Rs. " + std::to_string(flight->seatPrices[1]), 12) << "│" + putLeftPaddingAndFillRight(std::to_string(flight->totalSeats[1]) + "(" + std::to_string(flight->totalSeats[1] - flight->bookedSeats[1]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       └────────────────┴────────────┴────────────────────────────────┘       ║\n";
}
void printAllFlights(std::vector<Flight *> allFlightsList, bool printPositions = false, std::vector<int> positions = {})
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("All Available Flights") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    int count = 1;
    if (!printPositions)
    {
        if (allFlightsList.size() == 0)
        {
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("No Flights are scheduled!") << "║" << "\n";
        }
        else
        {
            for (auto flight : allFlightsList)
            {
                if (count != 1)
                {
                    std::cout << "║" << "  --------------------------------------------------------------------------  " << "║" << "\n";
                }
                printFlightDetails(count, flight);
                count++;
            }
        }
    }
    else
    {
        if (positions.size() == 0)
        {
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("No Flights are scheduled around that day!") << "║" << "\n";
        }
        else
        {
            for (int index : positions)
            {
                if (count != 1)
                {
                    std::cout << "║" << "  --------------------------------------------------------------------------  " << "║" << "\n";
                }
                printFlightDetails(count, allFlightsList[index]);
                count++;
            }
        }
    }

    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printAllTerminals(std::string terminal, std::vector<Terminal *> allTerminals)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("All " + terminal + "s List") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    if (allTerminals.size() == 0)
    {
        std::cout << "║" << putInCenter("") << "║" << "\n";
        std::cout << "║" << putInCenter("No " + terminal + "s are present.") << "║" << "\n";
    }
    else
    {
        //                                              61                                  8
        std::cout << "║   ┌─────────────────────────────────────────────────────────────┬────────┐   ║\n";
        std::cout << "║   │                           Name,City                         │  Code  │   ║\n";
        std::cout << "║   ├─────────────────────────────────────────────────────────────┼────────┤   ║\n";
        for (auto t : allTerminals)
        {
            std::cout << "║   │" << putInCenter("", 61) << "│" << putInCenter("", 8) << "│   ║" << "\n";
            std::cout << "║   │" << putInCenter(t->name + "," + t->city, 61) << "│" << putInCenter(t->code, 8) << "│   ║\n";
        }
        std::cout << "║   │" << putInCenter("", 61) << "│" << putInCenter("", 8) << "│   ║" << "\n";
        std::cout << "║   └─────────────────────────────────────────────────────────────┴────────┘   ║\n";
    }
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printTransportOrTerminalSelectionPanel(std::string transport, std::string terminal, std::vector<Train *> &allTrainsList,
                                            std::vector<Flight *> &allFlightsList, std::vector<Terminal *> &terminalsList)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Travel Management System") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("1. View all " + transport, 8, 39) << putLeftPaddingAndFillRight("2. View all " + terminal, 4, 39) << "║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("3. Go Back", 8) << "║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    char choice = inputUserChoice();
    switch (choice)
    {
    case '1':
    {
        if (transport == "Trains")
            printAllTrains(allTrainsList);
        else
            printAllFlights(allFlightsList);
        printContinue();
        break;
    }
    case '2':
    {
        if (terminal == "Stations")
            printAllTerminals("Station", terminalsList);
        else
            printAllTerminals("Airport", terminalsList);
        printContinue();
        break;
    }
    case '3':
    {
        return;
    }
    default:
    {
        printAlert("Wrong Choice");
        break;
    }
    }
    printTransportOrTerminalSelectionPanel(transport, terminal, allTrainsList, allFlightsList, terminalsList);
}

std::vector<std::string> printLoginPanel(std::string userType)
{
    std::string iEmail, iPassword;

    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter(userType + " Login") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";
    std::cout << "  Enter your email: ";
    std::cin >> iEmail;
    ignoreUptoNewLine();
    std::cout << "  Enter password: ";
    iPassword = inputHashedPassword();
    std::vector<std::string> info = {iEmail, iPassword};

    // remove
    // printAlert(iEmail + iPassword);

    return info;
}

void printAdminLoginOptions()
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Select Admin Login Type") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║          1. Flight Admin                   2. Train Admin                    ║" << "\n";
    std::cout << "║          3. Go Back                                                          ║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printAdminDashboard(std::string transport, std::string terminal)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter(transport + " Management") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("1. View " + transport + " and " + terminal + " details", 8) << "║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("2. Add New " + transport, 8) << "║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("3. Add New " + terminal, 8) << "║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("4. Remove " + transport, 8) << "║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("5. Remove " + terminal, 8) << "║" << "\n";
    std::cout << "║" << putLeftPaddingAndFillRight("6. Logout", 8) << "║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printRemoveTransportPanel(std::string transport, std::string &number)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Remove a " + transport) << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";
    std::cout << "  Enter " << transport << " Number: ";
    std::cin >> number;
    ignoreUptoNewLine();
    // remove
    // printAlert(code);
}
void printAddNewTerminalPanel(std::string terminal, std::string &name, std::string &city, std::string &code)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Add New " + terminal) << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";
    std::cout << "  Enter " << terminal << " Name: ";
    getline(std::cin, name);
    std::cout << "  Enter " << terminal << " City: ";
    getline(std::cin, city);
    std::cout << "  Enter " << terminal << " Code (alphabets only and length <= 4): ";
    std::cin >> code;
    ignoreUptoNewLine();

    // remove
    // printAlert(name + city + code);
}
void printRemoveTerminalPanel(std::string terminal, std::string &code)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Remove a " + terminal) << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";
    std::cout << "  Enter " << terminal << " code: ";
    std::cin >> code;
    ignoreUptoNewLine();

    // remove
    // printAlert(code);
}
void printAddNewTransportPanel(std::string transport, int &citiesCount, std::string &name, std::string &number,
                               std::string &coveringCitiesCode, std::string &departureTimes, std::string &totalSeats)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Add New " + transport) << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";

    std::cout << "  Enter " << transport << " Name: ";
    getline(std::cin, name);
    std::cout << "\n";
    if (transport == "Train")
    {
        std::cout << "  Enter Train Number (having digits only and length = 5): ";
        std::cin >> number;
        ignoreUptoNewLine();
        // std::cout << "Enter Train Type:";
        citiesCount = inputNumber("Enter number of covering stations: ");
        std::cout << "\n";
        std::cout << "  Enter covering stations code \n  (separated by a single space): ";
        getline(std::cin, coveringCitiesCode);
        std::cout << "\n";
        std::cout << "  Enter departure time from stations in order, in DD/MM/YY-HH:MM format \n  (separated by a single space): ";
        getline(std::cin, departureTimes);
        std::cout << "\n";
        std::cout << "  Enter number of seats in each of 1st AC, 2nd AC, 3rd AC and Sleeper Class\n  (separated by a single space): ";
        getline(std::cin, totalSeats);
    }
    else
    {
        std::string board, dest;
        std::string boardTime, destTime;
        citiesCount = 2;

        std::cout << "  Enter Flight Number (having alphabets/digits and length = 5): ";
        std::cin >> number;
        std::cout << "\n";
        std::cout << "  Enter Boarding Airpot Code: ";
        std::cin >> board;
        ignoreUptoNewLine();
        std::cout << "  Enter Destination Airport Code: ";
        std::cin >> dest;
        ignoreUptoNewLine();
        std::cout << "\n";
        std::cout << "  Enter Departure date and time (in DD/MM/YY-HH:MM format): ";
        std::cin >> boardTime;
        ignoreUptoNewLine();
        std::cout << "  Enter Departure date and time (in DD/MM/YY-HH:MM format): ";
        std::cin >> destTime;
        ignoreUptoNewLine();
        std::cout << "\n";
        std::cout << "  Enter number of seats in each of Economy and Business Class \n  (separated by a single space): ";
        getline(std::cin, totalSeats);

        coveringCitiesCode = board + " " + dest;
        departureTimes = boardTime + " " + destTime;
    }

    // remove
    // printAlert(name + number + coveringCitiesCode + departureTimes + totalSeats);
}

std::vector<std::string> printUserRegistrationPanel()
{
    std::string iName, iEmail, iPassword, iConfirmPassword;

    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("User Registration") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";

    std::cout << "  Enter your name: ";
    std::getline(std::cin, iName);
    std::cout << "  Enter your email: ";
    std::cin >> iEmail;
    ignoreUptoNewLine();
    std::cout << "\n";
    std::cout << "  Enter password: ";
    iPassword = inputHashedPassword();
    std::cout << "  Confirm password: ";
    iConfirmPassword = inputHashedPassword();

    // remove
    // printAlert(iName + iEmail + iPassword + iConfirmPassword);

    std::vector<std::string> info = {iName, iEmail, iPassword, iConfirmPassword};
    return info;
}
void printUserDashboard(std::string name)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Travel Management") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║        1. View trains and stations details                                   ║" << "\n";
    std::cout << "║        2. View flights and airport details                                   ║" << "\n";
    std::cout << "║        3. Book Ticket                                                        ║" << "\n";
    std::cout << "║        4. View Booked Tickets                                                ║" << "\n";
    std::cout << "║        5. Cancel Ticket                                                      ║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║        a. Change Name               b. Change Password                       ║" << "\n";
    std::cout << "║        c. Logout                                                             ║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "Welcome " << name << "\n";
}
void printBookingTransportSelection()
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Booking Panel") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║        1. Train Ticket Booking          2. Flight Ticket Booking             ║" << "\n";
    std::cout << "║        3. Go Back                                                            ║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printTicketTransportSelection(std::string mode)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter(mode + " Booked Tickets") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║        1. Train Ticket                 2. Flight Ticket                      ║" << "\n";
    std::cout << "║        3. Go Back                                                            ║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printAllTrainTickets(std::vector<TrainTicket *> trainTickets)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Your Train Reservations") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    if (trainTickets.size() == 0)
    {
        std::cout << "║" << putInCenter("") << "║" << "\n";
        std::cout << "║" << putInCenter("You Haven't Booked Any Tickets") << "║" << "\n";
    }
    else
    {
        int count = 1;
        for (auto t : trainTickets)
        {
            if (count != 1)
            {
                std::cout << "║" << "  --------------------------------------------------------------------------  " << "║" << "\n";
            }
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter(std::to_string(count) + ". " + t->trainPtr->name + "(" + t->trainPtr->number + ")") << "║" << "\n";
            std::cout << "║" << putInCenter("") << "║" << "\n";
            // std::cout << "║           " << putInCenter(t->boardingListNode->currentTerminal->name, 26) << "    " << putInCenter(t->destinationTerminal->name, 26) << "           ║" << "\n";
            std::cout << "║           " << putInCenter(t->boardingListNode->currentTerminal.code, 26) << "    " << putInCenter(t->destinationListNode->currentTerminal.code, 26) << "           ║" << "\n";
            std::cout << "║           " << putInCenter(t->boardingListNode->currentTerminal.city, 26) << "--->" << putInCenter(t->destinationListNode->currentTerminal.city, 26) << "           ║" << "\n";
            std::cout << "║           " << putInCenter("[" + t->boardingListNode->departureTime.rawDT + "]", 26) << "    " << putInCenter("[" + t->destinationListNode->departureTime.rawDT + "]", 26) << "           ║" << "\n";
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("PNR: " + t->pnr, 30) << putInCenter("", 18) << putInCenter(trainSeatTypes[t->seatChoice], 30) << "║" << "\n";
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("", 78 - 32) << putInCenter("Booking Price: Rs." + std::to_string(t->price), 32) << "║" << "\n";
            std::cout << "║" << putInCenter("", 78 - 32) << putInCenter("Booked At: " + t->bookingDate->rawDT, 32) << "║" << "\n";
            count++;
        }
    }
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printAllFlightTickets(std::vector<FlightTicket *> flightTickets)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Your Flight Reservations") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    if (flightTickets.size() == 0)
    {
        std::cout << "║" << putInCenter("") << "║" << "\n";
        std::cout << "║" << putInCenter("You Haven't Booked Any Tickets") << "║" << "\n";
    }
    else
    {
        int count = 1;
        for (auto t : flightTickets)
        {
            if (count != 1)
            {
                std::cout << "║" << "  --------------------------------------------------------------------------  " << "║" << "\n";
            }
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter(std::to_string(count) + ". " + t->flightPtr->name + "(" + t->flightPtr->number + ")") << "║" << "\n";
            std::cout << "║" << putInCenter("") << "║" << "\n";
            // std::cout << "║           " << putInCenter(t->boardingTerminal->name, 26) << "    " << putInCenter(t->destinationTerminal->name, 26) << "           ║" << "\n";
            std::cout << "║           " << putInCenter(t->boardingListNode->currentTerminal.code, 26) << "    " << putInCenter(t->destinationListNode->currentTerminal.code, 26) << "           ║" << "\n";
            std::cout << "║           " << putInCenter(t->boardingListNode->currentTerminal.city, 26) << "--->" << putInCenter(t->destinationListNode->currentTerminal.city, 26) << "           ║" << "\n";
            std::cout << "║           " << putInCenter("[" + t->boardingListNode->departureTime.rawDT + "]", 26) << "    " << putInCenter("[" + t->destinationListNode->departureTime.rawDT + "]", 26) << "           ║" << "\n";
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("PNR: " + t->pnr, 30) << putInCenter("", 18) << putInCenter(flightSeatTypes[t->seatChoice], 30) << "║" << "\n";
            std::cout << "║" << putInCenter("") << "║" << "\n";
            std::cout << "║" << putInCenter("", 78 - 32) << putInCenter("Booking Price: Rs." + std::to_string(t->price), 32) << "║" << "\n";
            std::cout << "║" << putInCenter("", 78 - 32) << putInCenter("Booked At: " + t->bookingDate->rawDT, 32) << "║" << "\n";

            count++;
        }
    }
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}

std::vector<std::string> printTicketBookingPanel(std::string transport)
{
    std::string boadringTerminalCode, destinationTerminalCode, journeyDate;

    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter(transport + " Ticket Booking Panel") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "\n";
    std::cout << "  Enter Boarding City Code: ";
    std::cin >> boadringTerminalCode;
    ignoreUptoNewLine();
    std::cout << "  Enter Destinatin City Code: ";
    std::cin >> destinationTerminalCode;
    ignoreUptoNewLine();
    std::cout << "  Enter Date (DD/MM/YY): ";
    std::cin >> journeyDate;
    ignoreUptoNewLine();
    // remove
    // printAlert(boadringTerminalCode + destinationTerminalCode + journeyDate);
    std::vector<std::string> inputs = {boadringTerminalCode, destinationTerminalCode, journeyDate};
    return inputs;
}
void printSeatTypeSelectionPanel(std::string transport, std::string transportName)
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter(transport + " Booking") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "║" << putInCenter(transport + " Name: " + transportName) << "║" << "\n";
    std::cout << "║                                                                              ║" << "\n";
    if (transport == "Train")
    {

        std::cout << "║        1. First AC                         2. Second AC                      ║" << "\n";
        std::cout << "║        3. Third AC                         4. Sleeper Class                  ║" << "\n";
        std::cout << "║        5. Cancel Booking                                                     ║" << "\n";
    }
    else
    {
        std::cout << "║        1. Economy Class                    2. Business Class                 ║" << "\n";
        std::cout << "║        3. Cancel Booking                                                     ║" << "\n";
    }
    std::cout << "║                                                                              ║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
std::vector<std::string> printChangePasswordPanel()
{
    std::string iOldPassword, iNewPassword, iConfirmPassword;
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Change Password Panel") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "  Enter old password: ";
    std::cin >> iOldPassword;
    ignoreUptoNewLine();
    std::cout << "  Enter new password: ";
    std::cin >> iNewPassword;
    ignoreUptoNewLine();
    std::cout << "  Confirm password: ";
    std::cin >> iConfirmPassword;
    ignoreUptoNewLine();

    // remove
    // printAlert(iOldPassword + iNewPassword + iConfirmPassword);

    std::vector<std::string> info = {iOldPassword, iNewPassword, iConfirmPassword};
    return info;
}
std::string printChangeNamePanel(std::string oldName)
{
    std::string newName;
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Change Name Panel") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
    std::cout << "  Enter New Name: ";
    std::getline(std::cin, newName);

    // remove
    // printAlert(newName);

    return newName;
}
