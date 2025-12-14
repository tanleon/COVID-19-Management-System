# UTAR COVID-19 Management System

## Overview
A C++ program developed to assist UTAR in managing and tracking the health status of staff and students during the COVID-19 pandemic. The system collects relevant data, categorizes users based on test results and symptoms, suggests appropriate actions, and provides reminders to ensure adherence to SOPs.

## Features

### 1. User Management
- **Registration**: Create new accounts with personal details (username, password, name, age, address, phone, IC/passport)
- **Login**: Secure authentication system
- **Profile Management**: View and update personal information

### 2. COVID-19 Assessment
- **Interactive Test**: Users answer questions about symptoms, travel history, and exposure
- **Automatic Categorization**: System assigns users to appropriate risk categories based on test results
- **Date Validation**: Ensures test dates are valid and properly formatted

### 3. Categories & Actions
System categorizes users into 5 risk levels with corresponding guidance:

| Category | Description | Suggested Actions |
|----------|-------------|-------------------|
| **POSITIVE** | Confirmed COVID-19 case | Hospital/Home quarantine, report to supervisor/lecturer, 7-day isolation |
| **SUSPECTED** | Showing symptoms | Self-isolation, work/study from home, 3-day monitoring |
| **CLOSE_CONTACT** | Exposed to confirmed case | Self-isolate and get tested immediately |
| **TRAVEL_HISTORY** | Recent travel to high-risk areas | Monitoring and testing as needed |
| **LOW_RISK** | No known exposure | Regular monitoring |

### 4. Smart Features
- **Reminder System**: Notifies users when testing is recommended based on last test date and category
- **Automatic Updates**: Categories update automatically after quarantine periods
- **Random Health Quotes**: Motivational messages displayed during login
- **Data Persistence**: All user data saved to file for consistency

## System Modules

### Module 1: Main Menu
- Entry point with ASCII art logo
- Options: Register, Login, Exit
- Loads existing user data from `userdata.txt`

### Module 2: User Registration
- Collects personal information
- Validates unique usernames
- Sets default category as LOW_RISK
- Appends data to file immediately

### Module 3: User Login
- Secure credential verification
- Displays login reminders for first-time users
- Shows personalized welcome message

### Module 4: Profile Management
- **View Profile**: Display all personal details and latest test date
- **Update Profile**: Modify any personal information with validation
- **Take Test**: Interactive COVID-19 assessment
- **View Category**: See current risk level and recommended actions
- **Logout**: Return to main menu

### Module 5: Reminders & Quotes
- **Reminder Function**: Calculates days since last test and prompts retesting
- **Daily Testing**: For CLOSE_CONTACT and SUSPECTED categories
- **Automatic Category Updates**: POSITIVE → LOW_RISK after 7 days
- **Random Quotes**: Health-related motivational messages

## Technical Details

### Data Structures
- **User Struct**: Contains all user information including COVID-19 category
- **Category Enum**: Defines 5 risk levels for type safety
- **Vector Storage**: Dynamic array for user management

### File Handling
- **File Format**: Pipe-separated values (`|`)
- **Automatic Saving**: Data persists across sessions
- **Error Handling**: Graceful handling of file I/O errors

### Validation
- Unique usernames
- Numeric age validation
- Date format validation (DD/MM/YYYY)
- Password confirmation for updates

### Dependencies
- Standard C++ Libraries: `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<cstdlib>`, `<sstream>`, `<regex>`, `<ctime>`, `<iomanip>`

## Usage Instructions

### Compilation
```bash
g++ -o covid_manager main.cpp
```

### Running the Program
```bash
./covid_manager
```

### User Flow
1. **New Users**: Register with personal details
2. **Returning Users**: Login with credentials
3. **First Login**: Update COVID-19 test results
4. **Regular Use**: 
   - Take tests when prompted
   - View category for guidance
   - Update profile as needed
5. **System Reminders**: Follow testing recommendations

## File Format
`userdata.txt` stores data in this format:
```
username|password|name|age|address|phone|IC|category|testdate
```

Where:
- `category`: 0-4 (LOW_RISK to POSITIVE)
- `testdate`: DD/MM/YYYY format

## Target Users
- **UTAR Staff**: Academic and administrative personnel
- **UTAR Students**: Local and international students
- **Both Malaysian and Non-Malaysian** users supported (IC/Passport)

## System Requirements
- C++11 or later compatible compiler
- Standard console/terminal environment
- File write permissions for data persistence

## Design Principles
1. **User-Friendly**: Intuitive menu navigation
2. **Robust**: Data validation and error handling
3. **Secure**: Password protection and unique usernames
4. **Informative**: Clear guidance based on medical protocols
5. **Proactive**: Reminder system for health monitoring

## Notes
- System uses local time for date calculations
- All dates in DD/MM/YYYY format
- Test reminders trigger after 3-7 days depending on category
- International students use passport number instead of IC

## Development Team
UTAR Computer Science Project - COVID-19 Management System
