# COVID-19 Health Management System

## Overview
A comprehensive C++ application designed to help organizations manage and track health status during the COVID-19 pandemic. The system collects relevant health data, categorizes users based on test results and symptoms, suggests appropriate actions, and provides reminders to ensure adherence to health protocols.

## Features

### 1. User Management
- **Registration**: Create new accounts with personal details (username, password, name, age, address, phone, IC/passport)
- **Login**: Secure authentication system with password protection
- **Profile Management**: View and update personal information with validation

### 2. COVID-19 Health Assessment
- **Interactive Self-Assessment**: Users answer questions about symptoms, travel history, and exposure
- **Automatic Categorization**: System assigns users to appropriate risk categories based on assessment results
- **Date Validation**: Ensures test dates are valid and properly formatted
- **Smart Recommendations**: Provides personalized health guidance based on category

### 3. Risk Categories & Health Guidance
System categorizes users into 5 risk levels with corresponding health guidance:

| Category | Description | Recommended Actions |
|----------|-------------|----------------------|
| **POSITIVE** | Confirmed COVID-19 case | Immediate isolation, medical consultation, 7-day quarantine |
| **SUSPECTED** | Showing COVID-19 symptoms | Self-isolation, testing, 3-day monitoring, medical consultation if needed |
| **CLOSE_CONTACT** | Exposed to confirmed case | Self-quarantine, immediate testing, symptom monitoring |
| **TRAVEL_HISTORY** | Recent travel to high-risk areas | 14-day symptom monitoring, consider testing |
| **LOW_RISK** | No known exposure or symptoms | Continue preventive measures, regular hand washing, mask use in crowded areas |

### 4. Smart Health Features
- **Reminder System**: Notifies users when testing is recommended based on last test date and health category
- **Automatic Updates**: Categories update automatically after quarantine periods
- **Health Tips**: Motivational health quotes displayed during login
- **Data Persistence**: All user data saved to file for consistency across sessions
- **Date Normalization**: Handles various date formats and converts to standard DD/MM/YYYY

## System Modules

### Module 1: Main Interface
- Clean console-based interface with clear section headers
- Options: Register New Account, Login, Exit
- Loads existing user data from `userdata.txt` at startup

### Module 2: Account Registration
- Collects personal information with validation
- Ensures unique usernames
- Sets default category as LOW_RISK
- Saves data to file immediately

### Module 3: Secure Authentication
- Credential verification with error handling
- Displays health reminders for first-time users
- Personalized welcome messages

### Module 4: Profile & Health Management
- **View Profile**: Display all personal details and latest test information
- **Update Profile**: Modify any personal information with input validation
- **Take Health Assessment**: Interactive COVID-19 risk assessment
- **View Health Category**: See current risk level and recommended health actions
- **Logout**: Secure session termination

### Module 5: Health Monitoring
- **Smart Reminders**: Calculates days since last test and prompts for re-testing when needed
- **Daily Monitoring**: For CLOSE_CONTACT and SUSPECTED categories
- **Automatic Category Updates**: POSITIVE → LOW_RISK after 7-day quarantine period
- **Health Inspiration**: Health-related motivational messages

## Technical Details

### Data Structures
- **User Struct**: Contains all user information including health category and test dates
- **Category Enum**: Defines 5 risk levels for type-safe health categorization
- **Vector Storage**: Dynamic array for efficient user management

### File Handling
- **File Format**: Pipe-separated values (|) for easy parsing
- **Automatic Saving**: Data persists across program sessions
- **Error Handling**: Graceful handling of file I/O errors with informative messages
- **Data Validation**: Checks for data integrity during loading

### Input Validation
- Unique username validation
- Numeric age validation with range checking (1-120)
- Date format validation (accepts multiple formats)
- Password confirmation for security updates
- Phone number and ID validation

### Date Management
- Accepts multiple date formats (DD/MM/YYYY, D/M/YYYY)
- Normalizes all dates to DD/MM/YYYY format
- Calculates days between dates for reminder system
- Uses system time for current date calculations

## Dependencies
- **Standard C++ Libraries**: `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<cstdlib>`, `<sstream>`, `<regex>`, `<ctime>`, `<iomanip>`, `<limits>`, `<algorithm>`
- **C++11 or later** compatible compiler

## Installation & Usage

### Compilation
```bash
g++ -o health_manager Program.cpp
```

### Running the Program
```bash
./health_manager
```
or on Windows:
```bash
health_manager.exe
```

### User Flow
1. **New Users**: Register with personal details
2. **Returning Users**: Login with credentials
3. **First Login**: Complete COVID-19 health assessment
4. **Regular Use**:
   - Take periodic health assessments
   - View health category for personalized guidance
   - Update profile information as needed
5. **Health Monitoring**: Follow system reminders for testing

### File Format
`userdata.txt` stores data in this format:
```
username|password|name|age|address|phone|ID|category|testdate
```

Where:
- `category`: 0-4 (LOW_RISK to POSITIVE)
- `testdate`: DD/MM/YYYY format (normalized)

## Target Users
- **Organization Staff**: Administrative and operational personnel
- **Students**: Educational institution members
- **General Users**: Anyone needing COVID-19 health tracking
- **International Users**: Supports both local ID and passport numbers

## System Requirements
- C++11 or later compatible compiler (GCC, Clang, MSVC)
- Standard console/terminal environment
- File write permissions for data persistence
- Minimum 4MB RAM, 10MB disk space

## Design Principles
- **User-Friendly**: Intuitive menu navigation with clear prompts
- **Robust**: Comprehensive data validation and error handling
- **Secure**: Password protection and unique username validation
- **Informative**: Clear health guidance based on medical protocols
- **Proactive**: Smart reminder system for health monitoring
- **Accessible**: Supports both local and international users

## Notes
- System uses local time for date calculations
- All dates normalized to DD/MM/YYYY format
- Test reminders trigger after 3-7 days depending on health category
- International users can use passport number instead of local ID
- Data is saved automatically after each significant operation

## Development
This system demonstrates:
- Object-oriented design principles
- File I/O operations with error handling
- Input validation and data sanitization
- Date/time manipulation
- Menu-driven console applications
- Health informatics application development

## License
Open for educational and organizational use. Adaptable for various health management scenarios.
