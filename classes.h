#pragma once
#include <bits/stdc++.h>

struct DateTime;
struct ListNode;

class Terminal;
class Admin;
class FlightAdmin;
class TrainAdmin;
class User;
class TicketManager;
class UserManager;
class Ticket;
class TrainTicket;
class FlightTicket;
class Transport;
class Flight;
class Train;
class CSVManager;

class Terminal
{
public:
    std::string name;
    std::string city;
    std::string code;
    // std::vector<Train *> trainsPassing;
    // std::vector<Flight *> flightsPassing;
    Terminal() {}
    Terminal(std::string name, std::string city, std::string code);
};

struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    std::string rawDT;
    DateTime() {}
    DateTime(std::string rawDT, int year, int month, int day, int hour, int minute) : rawDT(rawDT), year(year), month(month), day(day), hour(hour), minute(minute) {}

    // == operator
    bool operator==(const DateTime &other) const;
    // < operator
    bool operator<(const DateTime &other) const;
    // > operator
    bool operator>(const DateTime &other) const;
    // <= operator
    bool operator<=(const DateTime &other) const;
    // >= operator
    bool operator>=(const DateTime &other) const;
    // != operator
    bool operator!=(const DateTime &other) const;
    // + operator
    DateTime operator+(const DateTime &other) const;
    // - operator
    DateTime operator-(const DateTime &other) const;
};
struct ListNode
{
    Terminal currentTerminal;
    DateTime departureTime;
    ListNode *next;
};

class Admin
{
private:
    std::string email;
    std::string password;

public:
    Admin() {}
    Admin(std::string email, std::string passsword);
    void adminLoginPanel();
    virtual void adminDashboard() = 0;
    virtual void manageAddNewTerminal() = 0;
    virtual void manageAddNewTransport() = 0;
    virtual void manageRemoveTerminal() = 0;
    virtual void manageRemoveTransport() = 0;
};
class FlightAdmin : public Admin
{
private:
    void adminDashboard() override;
    void manageAddNewTerminal() override;
    void manageAddNewTransport() override;
    void manageRemoveTerminal() override;
    void manageRemoveTransport() override;

public:
    FlightAdmin() {}
    FlightAdmin(std::string email, std::string password) : Admin(email, password) {}
};
class TrainAdmin : public Admin
{
private:
    void adminDashboard() override;
    void manageAddNewTerminal() override;
    void manageAddNewTransport() override;
    void manageRemoveTerminal() override;
    void manageRemoveTransport() override;

public:
    TrainAdmin() {}
    TrainAdmin(std::string email, std::string password) : Admin(email, password) {}
};

class User
{
private:
    std::string password;

public:
    std::string email;
    std::string fullName;
    std::vector<TrainTicket *> trainTickets;
    std::vector<FlightTicket *> flightTickets;

    User() {}
    User(std::string fullName, std::string email, std::string password) : fullName(fullName), email(email), password(password) {}
    std::string getPassword();
    void setPassword(std::string newPassword);
};

class TicketManager
{
private:
    void trainTicketBooking(User *user);
    void trainTicketCancelling(std::vector<TrainTicket *> &trainTickets);
    void flightTicketBooking(User *user);
    void flightTicketCancelling(std::vector<FlightTicket *> &flightTickets);

protected:
    void ticketViewingPanel(User *user);
    void ticketBookingPanel(User *user);
    void ticketCancellingPanel(User *user);

public:
    TicketManager() {}
};
class UserManager : public TicketManager
{
private:
    User *loggedInUser;

    void userDashboard();
    bool isEmailRegistered(std::string email);
    void changeUserPassword();
    void changeUserName();

public:
    UserManager();
    void userRegistrationPanel();
    void userLoginPanel();
    User *findUser(std::string email);
};

class Ticket
{
public:
    std::string pnr;
    int seatChoice;
    int price;
    DateTime *bookingDate;
    ListNode *boardingListNode;
    ListNode *destinationListNode;
    std::string passengerName;
    std::string passengerEmail;
    Ticket() {}
};
class FlightTicket : public Ticket
{
public:
    Flight *flightPtr;
    FlightTicket() {}
    FlightTicket(int seatChoice, Flight *flightPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr, DateTime *bookingDate, std::string name, std::string email);
};
class TrainTicket : public Ticket
{
public:
    Train *trainPtr;
    TrainTicket() {}
    TrainTicket(int seatChoice, Train *trainPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr, DateTime *bookingDate, std::string name, std::string email);
};

class Transport
{
public:
    std::string name;
    std::string number;
    ListNode *coveringCities;
    std::vector<int> totalSeats;
    std::vector<int> bookedSeats;
    std::vector<int> seatPrices;
    Transport() {}
    Transport(std::string name, std::string number, ListNode *coveringCities, std::vector<int> totalSeats);
};
class Flight : public Transport
{
    std::string airlineName;

public:
    std::vector<int> seatPrices = {6000, 8000};
    Flight() : Transport() {}
    Flight(std::string name, std::string number, ListNode *coveringCities, std::vector<int> totalSeats) : Transport(name, number, coveringCities, totalSeats) {}
};
class Train : public Transport
{
    std::string trainType;

public:
    std::vector<int> seatPrices = {4000, 3000, 2000, 1000};
    Train() : Transport() {}
    Train(std::string name, std::string number, ListNode *coveringCities, std::vector<int> totalSeats) : Transport(name, number, coveringCities, totalSeats) {}
};

class CSVManager
{
private:
    std::string join(std::vector<int> &items, char delimiter);
    std::pair<std::string, std::string> join(ListNode *items, char delimiter);

public:
    CSVManager() {}
    void loadAllUsers();
    void saveNewUser(User *newUser);
    void saveAllUsers();

    void loadAllStations();
    void saveNewStation(Terminal *newStation);
    void saveAllStations();

    void loadAllAirports();
    void saveNewAirport(Terminal *newAirport);
    void saveAllAirports();

    void loadAllTrains();
    void saveNewTrain(Train *newTrain);
    void saveAllTrains();

    void loadAllFlights();
    void saveNewFlight(Flight *newFlight);
    void saveAllFlights();

    void loadAllTrainTickets();
    void saveNewTrainTicket(TrainTicket *newTicket, std::string email);
    void saveAllTrainTickets();

    void loadAllFlightTickets();
    void saveNewFlightTicket(FlightTicket *newTicket, std::string email);
    void saveAllFlightTickets();
};
