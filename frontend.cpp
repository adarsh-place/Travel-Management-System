#include <bits/stdc++.h>
#include <conio.h>

std::vector<std::string> trainSeatTypes = {"1st AC", "2nd AC", "3rd AC", "Sleeper Class"};
std::vector<std::string> flightSeatTypes = {"Economy Class", "Business Class"};

void ignoreUptoNewLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printContinue(std::string message = "")
{
    std::cout << message << "\nPress any key to continue.";
    _getch();
}
void printAlert(std::string message)
{
    system("cls");
    std::cout << message << "\n";
    printContinue();
}
void printExit()
{
    system("cls");
    std::cout << "Thank you for visiting.\n";
}

char inputUserChoice(std::string text = "Enter choice: ")
{
    std::cout << "\n"
              << text;
    char choice = _getch();
    return choice;
}
int inputNumber(std::string text = "Enter choice: ")
{
    std::string input;
    std::cout << "\n"
              << text;
    std::cin >> input;
    ignoreUptoNewLine();
    for (char &ch : input)
    {
        if (!isdigit(ch))
            return -1;
    }
    return stoi(input);
}

std::string putInCenter(std::string str, int width = 78)
{
    int leftSpace = width - str.size();
    for (int i = 1; i <= leftSpace / 2; i++)
        str = " " + str;
    for (int i = 1; i <= (leftSpace + 1) / 2; i++)
        str = str + " ";

    return str;
}
std::string putLeftPaddingAndFillRight(std::string str, int padding, int width = 78)
{
    for (int i = 1; i <= padding; i++)
    {
        str = " " + str;
    }
    for (int i = str.size() + 1; i <= width; i++)
    {
        str = str + " ";
    }
    return str;
}

void printLandingPanel()
{
    system("cls");

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

    std::cout << "║" << putLeftPaddingAndFillRight("Covering Stations: ", 4) << "║\n";

    ListNode *tempNode = train->coveringCities;
    int cityCount = 1;
    while (tempNode != NULL)
    {
        std::cout << "║" << putLeftPaddingAndFillRight("(" + std::to_string(cityCount) + ") " + tempNode->currentTerminal.name + " (" + tempNode->currentTerminal.code + "), " + tempNode->currentTerminal.city + " : " + tempNode->departureTime.rawDT, 8) << "║\n";
        tempNode = tempNode->next;
        cityCount++;
        if (tempNode != NULL)
            std::cout << "║         ↓                                                                    ║" << "\n";
    }

    std::cout << "║" << putInCenter("") << "║" << "\n";

    std::cout << "║       ┌────────────────┬────────────┬────────────────────────────────┐       ║\n";
    std::cout << "║       │   Seat Type    │    Price   │ Total Seats(Available Seats)   │       ║\n";
    std::cout << "║       ├────────────────┼────────────┼────────────────────────────────┤       ║\n";
    std::cout << "║       │     1st AC     │  Rs. 4000  │" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[3]) + "(" + std::to_string(train->totalSeats[3] - train->bookedSeats[3]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │     2nd AC     │  Rs. 3000  │" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[2]) + "(" + std::to_string(train->totalSeats[2] - train->bookedSeats[2]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │     3rd AC     │  Rs. 2000  │" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[1]) + "(" + std::to_string(train->totalSeats[1] - train->bookedSeats[1]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │  Sleeper Class │  Rs. 1000  │" + putLeftPaddingAndFillRight(std::to_string(train->totalSeats[0]) + "(" + std::to_string(train->totalSeats[0] - train->bookedSeats[0]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       └────────────────┴────────────┴────────────────────────────────┘       ║\n";
}
void printAllTrains(std::vector<Train *> allTrainsList, std::vector<int> positions = {})
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("All Available Trains") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    if (allTrainsList.size() == 0)
    {
        std::cout << "║" << putInCenter("") << "║" << "\n";
        std::cout << "║" << putInCenter("No Trains are available") << "║" << "\n";
        std::cout << "║" << putInCenter("") << "║" << "\n";
    }
    else
    {
        int count = 1;
        if (positions.size() == 0)
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
    std::cout << "║" << putLeftPaddingAndFillRight("Boarding Airport: " + tempNode->currentTerminal.name + " (" + tempNode->currentTerminal.code + "), " + tempNode->currentTerminal.city, 4) << "║\n";
    std::cout << "║" << putLeftPaddingAndFillRight("Boarding Date and Time: " + tempNode->departureTime.rawDT, 4) << "║\n";

    std::cout << "║" << putInCenter("") << "║" << "\n";

    tempNode = tempNode->next;
    std::cout << "║" << putLeftPaddingAndFillRight("Destination Airport: " + tempNode->currentTerminal.name + " (" + tempNode->currentTerminal.code + "), " + tempNode->currentTerminal.city, 4) << "║\n";
    std::cout << "║" << putLeftPaddingAndFillRight("Arrival Date and Time: " + tempNode->departureTime.rawDT, 4) << "║\n";

    std::cout << "║" << putInCenter("") << "║" << "\n";

    //                            16             12                   32
    std::cout << "║       ┌────────────────┬────────────┬────────────────────────────────┐       ║\n";
    std::cout << "║       │   Seat Type    │    Price   │ Total Seats(Available Seats)   │       ║\n";
    std::cout << "║       ├────────────────┼────────────┼────────────────────────────────┤       ║\n";
    std::cout << "║       │ Economy Class  │  Rs. 6000  │" + putLeftPaddingAndFillRight(std::to_string(flight->totalSeats[0]) + "(" + std::to_string(flight->totalSeats[0] - flight->bookedSeats[0]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       │ Business Class │  Rs. 8000  │" + putLeftPaddingAndFillRight(std::to_string(flight->totalSeats[1]) + "(" + std::to_string(flight->totalSeats[1] - flight->bookedSeats[1]) + ")", 10, 32) << "│       ║\n";
    std::cout << "║       └────────────────┴────────────┴────────────────────────────────┘       ║\n";
}
void printAllFlights(std::vector<Flight *> allFlightsList, std::vector<int> positions = {})
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("All Available Flights") << "║" << "\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << "\n";

    if (allFlightsList.size() == 0)
    {
        std::cout << "║" << putInCenter("") << "║" << "\n";
        std::cout << "║" << putInCenter("No Flights are available") << "║" << "\n";
        std::cout << "║" << putInCenter("") << "║" << "\n";
    }
    else
    {
        int count = 1;
        if (positions.size() == 0)
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
        std::cout << "║" << putInCenter("") << "║" << "\n";
    }
    else
    {
        //                                  28                        22              12
        std::cout << "║      ┌────────────────────────────┬──────────────────────┬────────────┐      ║\n";
        std::cout << "║      │            Name            │         City         │    Code    │      ║\n";
        std::cout << "║      ├────────────────────────────┼──────────────────────┼────────────┤      ║\n";
        for (auto t : allTerminals)
        {
            std::cout << "║      │" << putInCenter(t->name, 28) << "│" << putInCenter(t->city, 22) << "│" << putInCenter(t->code, 12) << "│      ║\n";
        }
        std::cout << "║      └────────────────────────────┴──────────────────────┴────────────┘      ║\n";
    }
    std::cout << "║" << putInCenter("") << "║" << "\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << "\n";
}
void printTransportOrTerminalSelectionPanel(std::string transport, std::string terminal, std::vector<Train *> &allTrainsList,
                                            std::vector<Flight *> &allFlightsList, std::vector<Terminal *> &terminalsList)
{
    system("cls");
    std::cout << transport << " and " << terminal << " Panel\n";
    std::cout << "1. " << transport << "\n";
    std::cout << "2. " << terminal << "\n";
    std::cout << "3. Go Back\n";
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
    std::cout << userType << " Login\n";
    std::cout << "Enter your email: ";
    std::cin >> iEmail;
    ignoreUptoNewLine();
    std::cout << "Enter password: ";
    std::cin >> iPassword;
    ignoreUptoNewLine();
    std::vector<std::string> info = {iEmail, iPassword};

    // remove
    // printAlert(iEmail + iPassword);

    return info;
}

void printAdminLoginOptions()
{
    system("cls");
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << "\n";
    std::cout << "║" << putInCenter("Admin Login Options") << "║" << "\n";
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
    std::cout << transport << " Management\n";
    std::cout << "1. View " << transport << " and " << terminal << " details\n";
    std::cout << "2. Add " << transport << "\n";
    std::cout << "3. Add " << terminal << "\n";
    std::cout << "4. Remove " << transport << "\n";
    std::cout << "5. Remove " << terminal << "\n";
    std::cout << "6. Log Out\n";
}
void printRemoveTransportPanel(std::string transport, std::string &code)
{
    system("cls");
    std::cout << "Remove a " << transport << "\n";
    std::cout << "Enter " << transport << " code: ";
    std::cin >> code;
    ignoreUptoNewLine();
    // remove
    // printAlert(code);
}
void printAddNewTerminalPanel(std::string terminal, std::string &name, std::string &city, std::string &code)
{
    system("cls");
    std::cout << "Add New " << terminal << "\n";
    std::cout << "Enter " << terminal << " Name: ";
    getline(std::cin, name);
    std::cout << "Enter " << terminal << " City: ";
    getline(std::cin, city);
    std::cout << "Enter " << terminal << " Code (alphabets only and length <= 4): ";
    std::cin >> code;
    ignoreUptoNewLine();

    // remove
    // printAlert(name + city + code);
}
void printRemoveTerminalPanel(std::string terminal, std::string &code)
{
    system("cls");
    std::cout << "Remove a " << terminal << "\n";
    std::cout << "Enter " << terminal << " number: ";
    std::cin >> code;
    ignoreUptoNewLine();

    // remove
    // printAlert(code);
}
void printAddNewTransportPanel(std::string transport, int &citiesCount, std::string &name, std::string &number,
                               std::string &coveringCitiesCode, std::string &departureTimes, std::string &totalSeats)
{
    system("cls");
    std::cout << "Add New " << transport << "\n";
    std::cout << "Enter " << transport << " Name: ";
    getline(std::cin, name);
    if (transport == "Train")
    {
        std::cout << "Enter Train Number (having digits only and length = 5): ";
        std::cin >> number;
        ignoreUptoNewLine();
        // std::cout << "Enter Train Type:";
        citiesCount = inputNumber("Enter number of covering stations: ");
        std::cout << "Enter covering stations code \n (separated by space): ";
        getline(std::cin, coveringCitiesCode);
        std::cout << "Enter departure time from stations in order \n (separated by space in DD/MM/YY-HH:MM format): ";
        getline(std::cin, departureTimes);
        std::cout << "Enter number of seats in each of Sleeper Class, 3rd AC, 2nd AC and 1st AC \n (separated by space): ";
        getline(std::cin, totalSeats);
    }
    else
    {
        std::cout << "Enter Flight Number (having alphabets/digits and length = 5): ";
        std::cin >> number;
        std::string board, dest;
        std::string boardTime, destTime;
        citiesCount = 2;
        std::cout << "Enter Boarding Airpot Code: ";
        std::cin >> board;
        ignoreUptoNewLine();
        std::cout << "Enter Destination Airport Code: ";
        std::cin >> dest;
        ignoreUptoNewLine();
        std::cout << "Enter Departure date and time (in DD/MM/YY-HH:MM format): ";
        std::cin >> boardTime;
        ignoreUptoNewLine();
        std::cout << "Enter Departure date and time (in DD/MM/YY-HH:MM format): ";
        std::cin >> destTime;
        ignoreUptoNewLine();
        std::cout << "Enter number of seats in each of Economy and Business Class \n (separated by space): ";
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
    std::cout << "User Registration\n";
    std::cout << "Enter your name: ";
    std::getline(std::cin, iName);
    std::cout << "Enter your email: ";
    std::cin >> iEmail;
    ignoreUptoNewLine();
    std::cout << "Enter password: ";
    std::cin >> iPassword;
    ignoreUptoNewLine();
    std::cout << "Confirm password: ";
    std::cin >> iConfirmPassword;
    ignoreUptoNewLine();

    // remove
    // printAlert(iName + iEmail + iPassword + iConfirmPassword);

    std::vector<std::string> info = {iName, iEmail, iPassword, iConfirmPassword};
    return info;
}
void printUserDashboard(std::string name)
{
    system("cls");
    std::cout << "Welcome " << name << "\n";
    std::cout << "1. View trains and stations details\n";
    std::cout << "2. View flights and airports details\n";
    std::cout << "3. Book Ticket\n";
    std::cout << "4. View Booked Tickets\n";
    std::cout << "5. Cancel Ticket\n";
    std::cout << "6. Change Password\n";
    std::cout << "7. Change Name\n";
    std::cout << "8. Logout\n";
}
void printBookingTransportSelection()
{
    system("cls");
    std::cout << "Booking Panel\n";
    std::cout << "1. Train Booking\n";
    std::cout << "2. Flight Booking\n";
    std::cout << "3. Go Back\n";
}
void printTicketTransportSelection(std::string func)
{
    system("cls");
    std::cout << func << " Tickets\n";
    std::cout << "1. Train Tickets\n";
    std::cout << "2. Flight Tickets\n";
    std::cout << "3. Go Back\n";
}
void printAllTrainTickets(std::vector<TrainTicket *> trainTickets)
{
    system("cls");
    std::cout << "All Booked Train Tickets.\n";

    if (trainTickets.size() == 0)
    {
        std::cout << "============================================================================\n";
        std::cout << "No tickets are Booked.";
        std::cout << "============================================================================\n";
    }
    else
    {
        int count = 1;
        for (auto t : trainTickets)
        {
            std::cout << "\n";
            std::cout << "============================================================================\n";
            std::cout << "\n";
            std::cout << count << "\n";
            std::cout << "Booking Date: " << t->bookingDate->rawDT << "\n";
            std::cout << "Train name: " << t->trainPtr->name << "\n";
            std::cout << "Train number: " << t->trainPtr->number << "\n";
            std::cout << "Boarding Station: " << t->boardingTerminal->name << "(" << t->boardingTerminal->code << ") , " << t->boardingTerminal->city << "\n";
            std::cout << "Destination Station: " << t->destinationTerminal->name << "(" << t->destinationTerminal->code << ") , " << t->destinationTerminal->city << "\n";
            std::cout << "Seat Class: " << trainSeatTypes[t->seatChoice] << "\n";
            std::cout << "Booking Price: " << t->price << "\n";
            count++;
        }
        std::cout << "\n";
        std::cout << "============================================================================\n";
    }
}
void printAllFlightTickets(std::vector<FlightTicket *> flightTickets)
{
    system("cls");
    std::cout << "All Booked Flight Tickets.\n";

    if (flightTickets.size() == 0)
    {
        std::cout << "============================================================================\n";
        std::cout << "No tickets are Booked.";
        std::cout << "============================================================================\n";
    }
    else
    {
        int count = 1;
        for (auto t : flightTickets)
        {
            std::cout << "\n";
            std::cout << "============================================================================\n";
            std::cout << "\n";
            std::cout << count << "\n";
            std::cout << "Booking Date: " << t->bookingDate->rawDT << "\n";
            std::cout << "Flight name: " << t->flightPtr->name << "\n";
            std::cout << "Flight number: " << t->flightPtr->number << "\n";
            std::cout << "Boarding Station: " << t->boardingTerminal->name << "(" << t->boardingTerminal->code << ") , " << t->boardingTerminal->city << "\n";
            std::cout << "Destination Station: " << t->destinationTerminal->name << "(" << t->destinationTerminal->code << ") , " << t->destinationTerminal->city << "\n";
            std::cout << "Seat Class: " << flightSeatTypes[t->seatChoice] << "\n";
            std::cout << "Booking Price: " << t->price << "\n";
            count++;
        }
        std::cout << "\n";
        std::cout << "============================================================================\n";
    }
}

std::vector<std::string> printTicketBookingPanel(std::string transport)
{
    std::string boadringTerminalCode, destinationTerminalCode, journeyDate;

    system("cls");
    std::cout << transport << " Ticket Booking\n";
    std::cout << "Enter Boarding City Code: ";
    std::cin >> boadringTerminalCode;
    ignoreUptoNewLine();
    std::cout << "Enter Destinatin City Code: ";
    std::cin >> destinationTerminalCode;
    ignoreUptoNewLine();
    std::cout << "Enter Date (DD/MM/YY): ";
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
    if (transport == "Train")
    {
        std::cout << "Train Booking\n";
        std::cout << "Train Name: " << transportName << "\n";
        std::cout << "1. First AC\n";
        std::cout << "2. Second AC\n";
        std::cout << "3. Third AC\n";
        std::cout << "4. Sleeper Class\n";
        std::cout << "5. Cancel Booking\n";
    }
    else
    {
        std::cout << "Flight Booking\n";
        std::cout << "Flight Name: " << transportName << "\n";
        std::cout << "1. Economy Class\n";
        std::cout << "2. Business Class\n";
        std::cout << "3. Cancel Booking\n";
    }
}
std::vector<std::string> printChangePasswordPanel()
{
    std::string iOldPassword, iNewPassword, iConfirmPassword;
    system("cls");
    std::cout << "Change Your Password\n";
    std::cout << "Enter old password: ";
    std::cin >> iOldPassword;
    ignoreUptoNewLine();
    std::cout << "Enter new password: ";
    std::cin >> iNewPassword;
    ignoreUptoNewLine();
    std::cout << "Confirm password: ";
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
    std::cout << "Change Your Name\n";
    std::cout << "Current Name is " << oldName << "\n";
    std::cout << "New Name: ";
    std::getline(std::cin, newName);

    // remove
    // printAlert(newName);

    return newName;
}
