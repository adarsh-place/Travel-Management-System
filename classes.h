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
    DateTime() {}
    DateTime(int year, int month, int day, int hour, int minute) : year(year), month(month), day(day), hour(hour), minute(minute) {}

    // == operator
    bool operator==(const DateTime &other) const
    {
        return day == other.day && month == other.month && year == other.year &&
               hour == other.hour && minute == other.minute;
    }

    // < operator
    bool operator<(const DateTime &other) const
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

    // > operator
    bool operator>(const DateTime &other) const
    {
        return other < *this;
    }

    // <= operator
    bool operator<=(const DateTime &other) const
    {
        return !(*this > other);
    }

    // >= operator
    bool operator>=(const DateTime &other) const
    {
        return !(*this < other);
    }

    // != operator
    bool operator!=(const DateTime &other) const
    {
        return !(*this == other);
    }
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
private:
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
private:
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

    void changeUserPassword();
    void changeUserName();
};

class CSVManager
{
public:
    static std::vector<std::vector<std::string>> readCSV(const std::string &filename);
    static void writeCSV(const std::string &filename, const std::vector<std::vector<std::string>> &data);
};

class Ticket
{
public:
    DateTime bookingDate;
    Terminal *boardingTerminal;
    Terminal *destinationTerminal;
    Ticket() {}
};
class FlightTicket : Ticket
{
public:
    Flight *flightPtr;
    FlightTicket() {}
};
class TrainTicket : Ticket
{
public:
    Train *trainPtr;
    TrainTicket() {}
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
