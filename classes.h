#pragma once
#include <bits/stdc++.h>

class Admin;
class FlightAdmin;
class TrainAdmin;
class User;
class UserManager;
class CSVManager;
class Transport;
class Flight;
class Train;
class Terminal;
class Ticket;
class TrainTicket;
class FlightTicket;

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
    virtual void addNewTerminal() = 0;
    virtual void addNewTransport() = 0;
    virtual void removeTerminal() = 0;
    virtual void removeTransport() = 0;
};
class FlightAdmin : public Admin
{
private:
    void adminDashboard() override;
    void addNewTerminal() override;
    void addNewTransport() override;
    void removeTerminal() override;
    void removeTransport() override;

public:
    FlightAdmin() {}
    FlightAdmin(std::string email, std::string password) : Admin(email, password) {}
};
class TrainAdmin : public Admin
{
private:
    void adminDashboard() override;
    void addNewTerminal() override;
    void addNewTransport() override;
    void removeTerminal() override;
    void removeTransport() override;

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
class UserManager
{
private:
    User *loggedInUser;

    bool isEmailRegistered(std::string email);
    User *findUser(std::string email);
    void userDashboard();
    void ticketBookingPanel(std::string transport);
    void cancelTicketPanel(std::string transport);
    void changeUserPassword();
    void changeUserName();

public:
    UserManager();
    void userRegistrationPanel();
    void userLoginPanel();
};

class CSVManager
{
private:
    // std::vector<int> splitAsNums(const std::string &s, char delimiter);
    std::string join(const std::vector<std::string> &items, char delimiter);
    std::string join(const std::vector<int> &items, char delimiter);
    std::pair<std::string, std::string> join(ListNode *items, char delimiter);

public:
    CSVManager() {}
    void loadUsersFromCSV();
    void saveUsersToCSV();

    void loadAllStations();
    void saveAllStations();

    void loadAllAirports();
    void saveAllAirports();

    void loadAllFlights();
    void saveAllFlights();

    void loadAllTrains();
    void saveAllTrains();
};

class Ticket
{
public:
    DateTime *bookingDate;
    ListNode *boardingListNode;
    ListNode *destinationListNode;
    std::string pnr;
    int seatChoice;
    Ticket() {}
};
class FlightTicket : public Ticket
{
public:
    Flight *flightPtr;
    int price = 0;
    FlightTicket() {}
    FlightTicket(int seatChoice, Flight *flightPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr);
};
class TrainTicket : public Ticket
{
public:
    Train *trainPtr;
    int price = 0;
    TrainTicket() {}
    TrainTicket(int seatChoice, Train *trainPtr, ListNode *boardingListNode, ListNode *destinationListNode, std::string pnr);
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
    Flight() : Transport() {}
    Flight(std::string name, std::string number, ListNode *coveringCities, std::vector<int> totalSeats) : Transport(name, number, coveringCities, totalSeats) {}
};
class Train : public Transport
{
    std::string trainType;

public:
    Train() : Transport() {}
    Train(std::string name, std::string number, ListNode *coveringCities, std::vector<int> totalSeats) : Transport(name, number, coveringCities, totalSeats) {}
};
