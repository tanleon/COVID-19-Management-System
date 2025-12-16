// Covid-19 Health Management Program
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <regex>
#include <ctime>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

// Enum to represent different categories related to COVID-19 for tracking and reporting purposes.
enum Category
{
    LOW_RISK,           // Low-risk category, typically used for individuals with no known exposure to COVID-19.
    TRAVEL_HISTORY,     // Has recently traveled to a high-risk area with active COVID-19 cases.
    SUSPECTED,          // Showing symptoms of COVID-19 but not confirmed positive yet.
    CLOSE_CONTACT,      // Close contact with a confirmed positive case.
    POSITIVE            // Confirmed positive case of COVID-19.
};

// Structure for capturing data about users
struct User
{
    string username;
    string password;
    string name;
    int age;
    string address;
    string phone;
    string IC;
    Category category;
    string testdate;
};

// Global constants
const string DATA_FILE = "userdata.txt";
const string DEFAULT_DATE = "00/00/0000";
const int TEST_REMINDER_DAYS = 3;
const int QUARANTINE_DAYS = 7;

// Function prototypes
void loadUsersFromFile(const string& filename, vector<User>& users);
void saveUsersToFile(const string& filename, const vector<User>& users);
void clearScreen();
void waitForUser();

// User management
void registration(vector<User>& users);
bool login(vector<User>& users, User*& currentUser);
void logout(User*& currentUser);

// User operations
void viewProfile(const User* user);
void updateProfile(User* user, vector<User>& users);
void takeTest(vector<User>& users, User* user);
void viewCategory(const User* user);
void showReminder(const User* user);

// Helper functions
int categoryToInt(Category category);
Category intToCategory(int value);
bool isValidDate(const string& dateStr);
bool needsTesting(const User* user);
void updateCategoryBasedOnTime(User* user);
void displayQuote();
int getValidatedInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX);
string getValidatedString(const string& prompt, bool allowSpaces = true);
string getCurrentDate();
string normalizeDate(const string& dateStr);

int main()
{
    vector<User> users;
    User* currentUser = nullptr;
    bool dataModified = false;

    // Load users from file
    loadUsersFromFile(DATA_FILE, users);

    // Main program loop
    while (true)
    {
        clearScreen();
        
        // Display header
        cout << "========================================\n";
        cout << "    COVID-19 HEALTH MANAGEMENT SYSTEM   \n";
        cout << "========================================\n\n";
        
        if (currentUser == nullptr)
        {
            // Not logged in - show main menu
            cout << "MAIN MENU:\n";
            cout << "1. Register New Account\n";
            cout << "2. Login\n";
            cout << "3. Exit\n\n";
            
            int choice = getValidatedInt("Enter your choice (1-3): ", 1, 3);
            clearScreen();
            
            switch (choice)
            {
                case 1:
                    registration(users);
                    saveUsersToFile(DATA_FILE, users);
                    dataModified = true;
                    waitForUser();
                    break;
                    
                case 2:
                    if (login(users, currentUser))
                    {
                        if (currentUser->testdate == DEFAULT_DATE)
                        {
                            cout << "\nNOTICE: Please update your test result after login.\n";
                        }
                        showReminder(currentUser);
                        waitForUser();
                    }
                    break;
                    
                case 3:
                    if (dataModified)
                    {
                        saveUsersToFile(DATA_FILE, users);
                        cout << "User data has been saved.\n";
                    }
                    cout << "Thank you for using the COVID-19 Health Management System!\n";
                    return 0;
            }
        }
        else
        {
            // Logged in - show user menu
            displayQuote();
            cout << "\nUSER MENU - Welcome, " << currentUser->name << "\n";
            cout << "========================================\n";
            cout << "1. View Profile\n";
            cout << "2. Update Profile\n";
            cout << "3. Take COVID-19 Test\n";
            cout << "4. View Health Category\n";
            cout << "5. Logout\n\n";
            
            int choice = getValidatedInt("Enter your choice (1-5): ", 1, 5);
            clearScreen();
            
            switch (choice)
            {
                case 1:
                    viewProfile(currentUser);
                    waitForUser();
                    break;
                    
                case 2:
                    updateProfile(currentUser, users);
                    saveUsersToFile(DATA_FILE, users);
                    dataModified = true;
                    break;
                    
                case 3:
                    takeTest(users, currentUser);
                    saveUsersToFile(DATA_FILE, users);
                    dataModified = true;
                    waitForUser();
                    break;
                    
                case 4:
                    viewCategory(currentUser);
                    waitForUser();
                    break;
                    
                case 5:
                    logout(currentUser);
                    break;
            }
        }
    }
    
    return 0;
}

void loadUsersFromFile(const string& filename, vector<User>& users)
{
    ifstream infile(filename);
    if (!infile.is_open())
    {
        // This is not an error - it's normal if the file doesn't exist yet
        cout << "No data file found. Starting with empty user database.\n";
        return;
    }
    
    string line;
    int lineNumber = 0;
    int loadedCount = 0;
    
    while (getline(infile, line))
    {
        lineNumber++;
        
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(' ') == string::npos)
            continue;
        
        // Count the number of delimiters
        int delimiterCount = 0;
        for (char c : line) {
            if (c == '|') delimiterCount++;
        }
        
        // We need exactly 8 delimiters for 9 fields
        if (delimiterCount != 8) {
            cerr << "Warning: Line " << lineNumber << " has " << delimiterCount 
                 << " delimiters (expected 8). Skipping.\n";
            cerr << "Line: " << line << endl;
            continue;
        }
        
        stringstream ss(line);
        vector<string> fields;
        string field;
        
        // Split by '|'
        while (getline(ss, field, '|')) {
            fields.push_back(field);
        }
        
        // Check if we got exactly 9 fields
        if (fields.size() != 9) {
            cerr << "Warning: Line " << lineNumber << " has " << fields.size() 
                 << " fields (expected 9). Skipping.\n";
            continue;
        }
        
        try {
            User user;
            user.username = fields[0];
            user.password = fields[1];
            user.name = fields[2];
            
            // Parse age
            try {
                user.age = stoi(fields[3]);
                if (user.age < 1 || user.age > 120) {
                    throw out_of_range("Age out of range");
                }
            } catch (const exception& e) {
                cerr << "Warning: Invalid age on line " << lineNumber 
                     << ": '" << fields[3] << "'. Using default age 25.\n";
                user.age = 25;
            }
            
            user.address = fields[4];
            user.phone = fields[5];
            user.IC = fields[6];
            
            // Parse category
            try {
                int catVal = stoi(fields[7]);
                if (catVal < 0 || catVal > 4) {
                    throw out_of_range("Category out of range");
                }
                user.category = intToCategory(catVal);
            } catch (const exception& e) {
                cerr << "Warning: Invalid category on line " << lineNumber 
                     << ": '" << fields[7] << "'. Using default LOW_RISK.\n";
                user.category = LOW_RISK;
            }
            
            user.testdate = normalizeDate(fields[8]);
            
            // Validate date
            if (user.testdate != DEFAULT_DATE && !isValidDate(user.testdate)) {
                cerr << "Warning: Invalid date format on line " << lineNumber 
                     << ": '" << fields[8] << "'. Using default date.\n";
                user.testdate = DEFAULT_DATE;
            }
            
            users.push_back(user);
            loadedCount++;
            
        } catch (const exception& e) {
            cerr << "Error processing line " << lineNumber << ": " << e.what() << endl;
            cerr << "Line: " << line << endl;
            continue;
        }
    }
    
    infile.close();
    
    if (loadedCount > 0) {
        cout << "Successfully loaded " << loadedCount << " user(s) from file." << endl;
    } else if (lineNumber > 0) {
        cout << "No valid user data found in file." << endl;
    }
}

void saveUsersToFile(const string& filename, const vector<User>& users)
{
    ofstream outfile(filename);
    if (!outfile.is_open())
    {
        cerr << "Error: Could not save user data to file.\n";
        return;
    }
    
    for (const auto& user : users)
    {
        outfile << user.username << "|"
                << user.password << "|"
                << user.name << "|"
                << user.age << "|"
                << user.address << "|"
                << user.phone << "|"
                << user.IC << "|"
                << categoryToInt(user.category) << "|"
                << user.testdate << endl;
    }
    
    outfile.close();
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForUser()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void registration(vector<User>& users)
{
    cout << "REGISTRATION\n";
    cout << "============\n\n";
    
    User newUser;
    
    // Username
    while (true)
    {
        cout << "Username: ";
        cin >> newUser.username;
        
        bool usernameExists = false;
        for (const auto& user : users)
        {
            if (user.username == newUser.username)
            {
                cout << "Username already exists. Please choose another.\n";
                usernameExists = true;
                break;
            }
        }
        
        if (!usernameExists) break;
    }
    
    // Password
    cout << "Password: ";
    cin >> newUser.password;
    
    // Name
    cin.ignore();
    cout << "Full Name: ";
    getline(cin, newUser.name);
    
    // Age
    newUser.age = getValidatedInt("Age: ", 1, 120);
    
    // Address
    cout << "Address: ";
    getline(cin, newUser.address);
    
    // Phone
    cout << "Phone Number: ";
    getline(cin, newUser.phone);
    
    // IC/Passport
    cout << "IC/Passport Number: ";
    getline(cin, newUser.IC);
    
    // Default values
    newUser.category = LOW_RISK;
    newUser.testdate = DEFAULT_DATE;
    
    users.push_back(newUser);
    cout << "\nRegistration successful!\n";
}

bool login(vector<User>& users, User*& currentUser)
{
    cout << "LOGIN\n";
    cout << "=====\n\n";
    
    string username, password;
    
    cout << "Username: ";
    cin >> username;
    
    cout << "Password: ";
    cin >> password;
    
    for (auto& user : users)
    {
        if (user.username == username && user.password == password)
        {
            currentUser = &user;
            updateCategoryBasedOnTime(currentUser);
            cout << "\nLogin successful! Welcome, " << user.name << "!\n";
            return true;
        }
    }
    
    cout << "\nInvalid username or password.\n";
    waitForUser();
    return false;
}

void logout(User*& currentUser)
{
    currentUser = nullptr;
    cout << "You have been logged out.\n";
    waitForUser();
}

void viewProfile(const User* user)
{
    cout << "PROFILE INFORMATION\n";
    cout << "===================\n\n";
    
    cout << "Name: " << user->name << endl;
    cout << "Age: " << user->age << endl;
    cout << "Address: " << user->address << endl;
    cout << "Phone: " << user->phone << endl;
    cout << "IC/Passport: " << user->IC << endl;
    cout << "Last Test Date: " << (user->testdate == DEFAULT_DATE ? "Not recorded" : user->testdate) << endl;
    
    string categoryNames[] = {"Low Risk", "Travel History", "Suspected Case", "Close Contact", "Positive Case"};
    cout << "Health Category: " << categoryNames[user->category] << endl;
}

void updateProfile(User* user, vector<User>& users)
{
    while (true)
    {
        clearScreen();
        cout << "UPDATE PROFILE\n";
        cout << "==============\n\n";
        
        cout << "Current Information:\n";
        cout << "1. Name: " << user->name << endl;
        cout << "2. Age: " << user->age << endl;
        cout << "3. Address: " << user->address << endl;
        cout << "4. Phone: " << user->phone << endl;
        cout << "5. IC/Passport: " << user->IC << endl;
        cout << "6. Password: " << string(user->password.length(), '*') << endl;
        cout << "7. Username: " << user->username << endl;
        cout << "8. Return to Menu\n\n";
        
        int choice = getValidatedInt("Select field to update (1-8): ", 1, 8);
        
        if (choice == 8) return;
        
        cin.ignore();
        switch (choice)
        {
            case 1:
                cout << "Enter new name: ";
                getline(cin, user->name);
                cout << "Name updated.\n";
                break;
                
            case 2:
                user->age = getValidatedInt("Enter new age: ", 1, 120);
                cout << "Age updated.\n";
                break;
                
            case 3:
                cout << "Enter new address: ";
                getline(cin, user->address);
                cout << "Address updated.\n";
                break;
                
            case 4:
                cout << "Enter new phone number: ";
                getline(cin, user->phone);
                cout << "Phone number updated.\n";
                break;
                
            case 5:
                cout << "Enter new IC/Passport number: ";
                getline(cin, user->IC);
                cout << "IC/Passport updated.\n";
                break;
                
            case 6:
            {
                cout << "Enter old password: ";
                string oldPass;
                getline(cin, oldPass);
                
                if (oldPass == user->password)
                {
                    cout << "Enter new password: ";
                    getline(cin, user->password);
                    cout << "Password updated.\n";
                }
                else
                {
                    cout << "Incorrect password.\n";
                }
                break;
            }
                
            case 7:
            {
                cout << "Enter new username: ";
                string newUsername;
                getline(cin, newUsername);
                
                bool usernameExists = false;
                for (const auto& otherUser : users)
                {
                    if (otherUser.username == newUsername && &otherUser != user)
                    {
                        usernameExists = true;
                        break;
                    }
                }
                
                if (!usernameExists)
                {
                    user->username = newUsername;
                    cout << "Username updated.\n";
                }
                else
                {
                    cout << "Username already exists.\n";
                }
                break;
            }
        }
        
        waitForUser();
    }
}

void takeTest(vector<User>& users, User* user)
{
    cout << "COVID-19 SELF-ASSESSMENT\n";
    cout << "========================\n\n";
    
    // Ask screening questions
    bool hasFever = getValidatedInt("Do you have a fever? (1=Yes, 0=No): ", 0, 1);
    bool hasCough = getValidatedInt("Do you have a cough? (1=Yes, 0=No): ", 0, 1);
    bool hasBreathingDifficulty = getValidatedInt("Do you have difficulty breathing? (1=Yes, 0=No): ", 0, 1);
    bool hasTravelHistory = getValidatedInt("Have you traveled to a high-risk area in the past 14 days? (1=Yes, 0=No): ", 0, 1);
    bool hasCloseContact = getValidatedInt("Have you been in close contact with a COVID-19 positive individual? (1=Yes, 0=No): ", 0, 1);
    
    int testResult = getValidatedInt("Enter test result (1=Positive, 0=Negative): ", 0, 1);
    
    // Determine category based on responses
    int symptomScore = (hasFever ? 1 : 0) + (hasCough ? 1 : 0) + (hasBreathingDifficulty ? 1 : 0);
    
    if (testResult == 1)
    {
        user->category = POSITIVE;
    }
    else if (hasCloseContact)
    {
        user->category = CLOSE_CONTACT;
    }
    else if (symptomScore >= 2 || (symptomScore >= 1 && hasTravelHistory))
    {
        user->category = SUSPECTED;
    }
    else if (hasTravelHistory)
    {
        user->category = TRAVEL_HISTORY;
    }
    else
    {
        user->category = LOW_RISK;
    }
    
    // Get test date
    while (true)
    {
        cout << "\nEnter test date (DD/MM/YYYY) or 'today' for current date: ";
        string dateInput;
        cin >> dateInput;
        
        if (dateInput == "today")
        {
            user->testdate = getCurrentDate();
            break;
        }
        else
        {
            string normalizedDate = normalizeDate(dateInput);
            if (isValidDate(normalizedDate))
            {
                user->testdate = normalizedDate;
                break;
            }
            else
            {
                cout << "Invalid date format. Please use DD/MM/YYYY or D/M/YYYY.\n";
            }
        }
    }
    
    cout << "\nAssessment completed. Your health category has been updated.\n";
}

void viewCategory(const User* user)
{
    cout << "HEALTH CATEGORY & RECOMMENDATIONS\n";
    cout << "==================================\n\n";
    
    string categoryNames[] = {"Low Risk", "Travel History", "Suspected Case", "Close Contact", "Positive Case"};
    cout << "Your Category: " << categoryNames[user->category] << "\n\n";
    
    switch (user->category)
    {
        case POSITIVE:
            cout << "RECOMMENDED ACTIONS:\n";
            cout << "1. Isolate immediately for " << QUARANTINE_DAYS << " days\n";
            cout << "2. Notify your supervisor/lecturer\n";
            cout << "3. Follow local health authority guidelines\n";
            cout << "4. Monitor symptoms closely\n";
            cout << "5. Seek medical attention if symptoms worsen\n";
            break;
            
        case CLOSE_CONTACT:
            cout << "RECOMMENDED ACTIONS:\n";
            cout << "1. Self-quarantine for " << TEST_REMINDER_DAYS << " days\n";
            cout << "2. Get tested immediately\n";
            cout << "3. Monitor for symptoms\n";
            cout << "4. Wear a mask around others\n";
            break;
            
        case SUSPECTED:
            cout << "RECOMMENDED ACTIONS:\n";
            cout << "1. Self-isolate immediately\n";
            cout << "2. Get tested as soon as possible\n";
            cout << "3. Rest and monitor symptoms\n";
            cout << "4. Avoid contact with others\n";
            break;
            
        case TRAVEL_HISTORY:
            cout << "RECOMMENDED ACTIONS:\n";
            cout << "1. Monitor for symptoms for 14 days\n";
            cout << "2. Consider getting tested\n";
            cout << "3. Follow local quarantine requirements\n";
            break;
            
        case LOW_RISK:
            cout << "RECOMMENDED ACTIONS:\n";
            cout << "1. Continue practicing preventive measures\n";
            cout << "2. Wash hands regularly\n";
            cout << "3. Wear mask in crowded places\n";
            cout << "4. Get tested if you develop symptoms\n";
            break;
    }
    
    cout << "\nLast Test Date: " << (user->testdate == DEFAULT_DATE ? "Not recorded" : user->testdate) << endl;
}

void showReminder(const User* user)
{
    if (user->testdate == DEFAULT_DATE)
    {
        cout << "\nREMINDER: Please complete your COVID-19 test assessment.\n";
        return;
    }
    
    // Parse test date
    stringstream ss(user->testdate);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;
    
    // Calculate days since test
    tm testDate = {0};
    testDate.tm_mday = day;
    testDate.tm_mon = month - 1;
    testDate.tm_year = year - 1900;
    
    time_t testTime = mktime(&testDate);
    time_t currentTime = time(nullptr);
    
    double diffSeconds = difftime(currentTime, testTime);
    int daysSinceTest = static_cast<int>(diffSeconds / (24 * 3600));
    
    if (daysSinceTest >= TEST_REMINDER_DAYS && daysSinceTest < QUARANTINE_DAYS)
    {
        cout << "\nREMINDER: It has been " << daysSinceTest << " days since your last test. ";
        cout << "Consider getting tested again.\n";
    }
}

int categoryToInt(Category category)
{
    return static_cast<int>(category);
}

Category intToCategory(int value)
{
    if (value >= 0 && value <= 4)
        return static_cast<Category>(value);
    return LOW_RISK;
}

bool isValidDate(const string& dateStr)
{
    // Check for default date
    if (dateStr == DEFAULT_DATE) return true;
    
    // Remove any trailing whitespace
    string trimmed = dateStr;
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
    
    // Accept various date formats
    regex datePattern1("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/([0-9]{4})$");  // DD/MM/YYYY
    regex datePattern2("^([1-9]|[12][0-9]|3[01])/([1-9]|1[0-2])/([0-9]{4})$");     // D/M/YYYY
    regex datePattern3("^(0[1-9]|[12][0-9]|3[01])/([1-9]|1[0-2])/([0-9]{4})$");    // DD/M/YYYY
    regex datePattern4("^([1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/([0-9]{4})$");    // D/MM/YYYY
    
    if (!regex_match(trimmed, datePattern1) && 
        !regex_match(trimmed, datePattern2) &&
        !regex_match(trimmed, datePattern3) &&
        !regex_match(trimmed, datePattern4)) {
        return false;
    }
    
    // Parse the date
    stringstream ss(trimmed);
    int day, month, year;
    char slash;
    
    if (!(ss >> day >> slash >> month >> slash >> year)) {
        return false;
    }
    
    // Basic validation
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    // Check specific month-day combinations
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }
    
    return true;
}

string normalizeDate(const string& dateStr)
{
    if (dateStr.empty() || dateStr == DEFAULT_DATE) {
        return DEFAULT_DATE;
    }
    
    string trimmed = dateStr;
    // Remove any trailing whitespace
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
    
    // Try to parse the date
    stringstream ss(trimmed);
    int day, month, year;
    char slash;
    
    if (!(ss >> day >> slash >> month >> slash >> year)) {
        return DEFAULT_DATE;
    }
    
    // Format to DD/MM/YYYY
    stringstream normalized;
    normalized << setfill('0') << setw(2) << day << "/"
               << setfill('0') << setw(2) << month << "/"
               << setfill('0') << setw(4) << year;
    
    return normalized.str();
}

bool needsTesting(const User* user)
{
    if (user->testdate == DEFAULT_DATE)
        return true;
        
    // Similar date calculation as showReminder
    stringstream ss(user->testdate);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;
    
    tm testDate = {0};
    testDate.tm_mday = day;
    testDate.tm_mon = month - 1;
    testDate.tm_year = year - 1900;
    
    time_t testTime = mktime(&testDate);
    time_t currentTime = time(nullptr);
    
    double diffSeconds = difftime(currentTime, testTime);
    int daysSinceTest = static_cast<int>(diffSeconds / (24 * 3600));
    
    return (daysSinceTest >= TEST_REMINDER_DAYS);
}

void updateCategoryBasedOnTime(User* user)
{
    if (user->category != POSITIVE || user->testdate == DEFAULT_DATE)
        return;
    
    // Calculate days since positive test
    stringstream ss(user->testdate);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;
    
    tm testDate = {0};
    testDate.tm_mday = day;
    testDate.tm_mon = month - 1;
    testDate.tm_year = year - 1900;
    
    time_t testTime = mktime(&testDate);
    time_t currentTime = time(nullptr);
    
    double diffSeconds = difftime(currentTime, testTime);
    int daysSinceTest = static_cast<int>(diffSeconds / (24 * 3600));
    
    if (daysSinceTest >= QUARANTINE_DAYS)
    {
        user->category = LOW_RISK;
        cout << "\nNOTE: Your quarantine period has ended. Category updated to Low Risk.\n";
    }
}

void displayQuote()
{
    string quotes[] = {
        "\"Health is a state of complete harmony of the body, mind and spirit.\" - B.K.S. Iyengar",
        "\"To ensure good health: eat lightly, breathe deeply, live moderately, cultivate cheerfulness.\" - William Londen",
        "\"It is health that is the real wealth, and not pieces of gold and silver.\" - Mahatma Gandhi",
        "\"The first wealth is health.\" - Ralph Waldo Emerson",
        "\"He who has health has hope, and he who has hope has everything.\" - Thomas Carlyle",
        "\"A good laugh and a long sleep are the best cures in the doctor's book.\" - Irish Proverb",
        "\"Keeping your body healthy is an expression of gratitude to the whole cosmos.\" - Thich Nhat Hanh",
        "\"The doctor of the future will give no medicine, but will instruct his patients in care of the human frame.\" - Thomas Edison",
        "\"The cheerful mind perseveres, and the strong mind hews its way through a thousand difficulties.\" - Swami Vivekananda",
        "\"Good health is not something we can buy. However, it can be an extremely valuable savings account.\" - Anne Wilson Schaef"
    };
    
    srand(static_cast<unsigned>(time(nullptr)));
    int index = rand() % 10;
    
    cout << "Daily Inspiration: " << quotes[index] << "\n";
}

int getValidatedInt(const string& prompt, int minVal, int maxVal)
{
    int value;
    while (true)
    {
        cout << prompt;
        string input;
        getline(cin, input);
        
        // Check if input is empty
        if (input.empty()) {
            cout << "Input cannot be empty. Please enter a number.\n";
            continue;
        }
        
        // Check if all characters are digits (or negative sign at start)
        bool isNumeric = true;
        for (size_t i = 0; i < input.length(); i++) {
            if (i == 0 && input[i] == '-') {
                continue;  // Allow negative sign at beginning
            }
            if (!isdigit(input[i])) {
                isNumeric = false;
                break;
            }
        }
        
        if (!isNumeric) {
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }
        
        try {
            value = stoi(input);
        } catch (const invalid_argument& e) {
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        } catch (const out_of_range& e) {
            cout << "Number is too large or too small. Please enter a value between " 
                 << minVal << " and " << maxVal << ".\n";
            continue;
        }
        
        if (value < minVal || value > maxVal)
        {
            cout << "Please enter a value between " << minVal << " and " << maxVal << ".\n";
        }
        else
        {
            return value;
        }
    }
}

string getValidatedString(const string& prompt, bool allowSpaces)
{
    string input;
    cout << prompt;
    
    if (allowSpaces)
    {
        getline(cin, input);
    }
    else
    {
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    return input;
}

string getCurrentDate()
{
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << localTime->tm_mday << "/"
       << setfill('0') << setw(2) << (localTime->tm_mon + 1) << "/"
       << (localTime->tm_year + 1900);
    
    return ss.str();
}
