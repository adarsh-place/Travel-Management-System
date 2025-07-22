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

- User Registration
- Separate user and admin login
- View available schedules
- User capabilities:
   - Can book and manage tickets
   - Can update profile
- Admin capabilities:
   - Add and remove transports/terminals
   - View all available transports/terminals
- Use of **.csv** files fo persistence data storage
 
## Sections Overview

### 1. Guest Panel
   - Can check for available schedules
   - Can register with unique email id
   - Can login using registered email to manage bookings 
### 2. User Panel
   - Book tickets (train or flight)
   - View booked tickets
   - Cancel tickets
   - Update profile (Name, Password)
### 3. Admin Panel (Two types of admin access)
   - **Train Admin**
   - **Flight Admin**
### 4. Train Admin capabilities:
   - Add and remove train/station
   - View all available trains/stations   
### 5. Flight Admin capabilities:
   - Add and remove flight/airport
   - View all available flights/airports

## Setup Instructions

### Prerequisites
- C++17 or higher
- Windows terminal (some Windows-specific functions have been used.)

### Program Execution
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
