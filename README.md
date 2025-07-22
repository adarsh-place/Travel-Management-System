# Travel Management System

A console-based C++ application that allows users to book **train** and **flight** tickets and check for the schedules. It also has an admin access to manage and update data.

## Disclaimer

This is a virtual ticket booking program for demonstration purposes only. It does not perform real-world ticket bookings.

## Concepts
- Object Oriented Programming
- File Handling
- Data Structures
- Input Validation
- Masked Password Input

## Key Features

### For Users
- Register and login
- View available trains and flights
- Book tickets (train or flight)
- View booked tickets
- Cancel tickets
- Update profile (name, password)

### For Admins
Two types of admin access:
- **Train Admin**
- **Flight Admin**

Admin capabilities:
- Add and remove trains/flights
- Add and remove stations/airports
- View all available transports and terminals

## Setup Instructions

### Prerequisites
- C++17 or higher
- Windows terminal (some Windows-specific functions have been used.)

### How to Run
1. Clone this repository

   ```bash
   git clone https://github.com/adarsh-place/Travel-Management-System
2. Navigate to the folder

   ```bash
   cd Travel-Management-System
3. Use the following command to compile the code :

   ```bash
   g++ -I. driver.cpp -o app.exe
4. Run the program

   ```bash
   ./app.exe

## Resources
- ASCII Box Drawing: Unicode Box Drawing Characters
