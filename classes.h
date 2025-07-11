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
    ListNode *next;
    DateTime departureTime;
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
public:
    FlightAdmin() {}
    FlightAdmin(std::string email, std::string password) : Admin(email, password) {}
    void adminDashboard();
    void addNewTerminal();
    void addNewTransport();
    void removeTerminal();
    void removeTransport();
};
class TrainAdmin : public Admin
{
public:
    TrainAdmin() {}
    TrainAdmin(std::string email, std::string password) : Admin(email, password) {}
    void adminDashboard();
    void addNewTerminal();
    void addNewTransport();
    void removeTerminal();
    void removeTransport();
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
    std::vector<User *> usersList;
    User *loggedInUser;

public:
    UserManager();

    bool isEmailRegistered(std::string email);
    User *findUser(std::string email);

    void userRegistrationPanel();
    void userLoginPanel();

    void userDashboard();
    void ticketBookingPanel(std::string transport);
    void cancelTicketPanel(std::string transport);
    void changeUserPassword();
    void changeUserName();
};

class CSVManager
{
public:
    CSVManager() {}
};

class Ticket
{
public:
    DateTime *bookingDate;
    Terminal *boardingTerminal;
    Terminal *destinationTerminal;
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
    FlightTicket(int seatChoice, Flight *flightPtr, std::string boardingTerminalCode, std::string destinationTerminalCode,std::string pnr);
};
class TrainTicket : public Ticket
{
public:
    Train *trainPtr;
    int price = 0;
    TrainTicket() {}
    TrainTicket(int seatChoice, Train *trainPtr, std::string boardingTerminalCode, std::string destinationTerminalCode,std::string pnr);
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
