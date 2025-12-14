//Covid-19 Management Program for UTAR students and staff
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <regex>
#include <ctime>
#include <iomanip>
using namespace std;

// Enum to represent different categories related to COVID-19 for tracking and reporting purposes.
enum Category
{
	POSITIVE,	// Confirmed positive case of COVID-19.
	CLOSE_CONTACT,	// Close contact with a confirmed positive case.
	SUSPECTED,	// Showing symptoms of COVID-19 but not confirmed positive yet.
	TRAVEL_HISTORY,	// Has recently traveled to a high-risk area with active COVID-19 cases.
	LOW_RISK	// Low-risk category, typically used for individuals with no known exposure to COVID-19.
};

//structure used for capturing data about users
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

//define vector used to store users 
vector<User> users;

// Function prototypes.
void registration(vector<User>& users);	//function to register a new user
void login(vector<User>& users);	//function to log in a user
void saveUsersToFile(string filename, vector<User>* users);	//function to save user data to a file

void viewProfile(User* user);	//function to view user profile
void updateProfile(User* user, vector<User>& users);	//function to update user profile
void takeTest(vector<User>& users, string name);	//function for a user to take a COVID-19 test
void viewCategory(vector<User>& users, string name);	//function for user to view Covid-19 categories and suggested actions 

void reminder(vector<User>& users, string name);	//function to remind a user to take a COVID-19 test
int categoryToInt(Category category);	//function to convert Category enum to integer value
bool isValidDate(const string& dateStr);	//function to check if a given date is valid
void updateCategory(vector<User>& users, int index, int daysSinceTest, string name);	//function to update user category based on COVID-19 test results
bool checkIfNeedsTesting(vector<User>& users, string name);	//function to check if user needs testing based on current category and 
//the number of days that have passed since the last test. 
int randomQuote();	//function to generate a random inspirational quote for user 

bool dataModified;//boolean variable to keep track of whether user data has been modified
bool check;	//boolean variable to check if user needs testing 


int main()
{
	//Get users data from text file
	ifstream infile("userdata.txt");
	if (infile.is_open())
	{
		string line;
		while (getline(infile, line))
		{
			stringstream ss(line);
			User user;
			getline(ss, user.username, '|');
			getline(ss, user.password, '|');
			getline(ss, user.name, '|');
			string ageStr;
			getline(ss, ageStr, '|');
			user.age = stoi(ageStr);
			getline(ss, user.address, '|');
			getline(ss, user.phone, '|');
			getline(ss, user.IC, '|');
			string categoryStr;
			getline(ss, categoryStr, '|');

			//change category from string to int
			int categoryInt = (categoryStr == "") ? 0 : stoi(categoryStr);
			switch (categoryInt)
			{
			case 0:
				user.category = Category::LOW_RISK;
				break;
			case 1:
				user.category = Category::TRAVEL_HISTORY;
				break;
			case 2:
				user.category = Category::SUSPECTED;
				break;
			case 3:
				user.category = Category::CLOSE_CONTACT;
				break;
			case 4:
				user.category = Category::POSITIVE;
				break;
			default:
				cout << "Invalid category integer: " << categoryInt << endl;
				break;
			}

			getline(ss, user.testdate);

			users.push_back(user);
		}

		infile.close();
	}
	else
	{
		cerr << "Unable to load data, please standby while our technicians resolve the problem. Sorry for any inconvenience caused." << endl;
		exit(1);
	}

	// Main menu for the system 
	bool dataModified = false;
	do {
		cout << "#     # #######    #    ######  " << endl;
		cout << "#     #    #      # #   #     # " << endl;
		cout << "#     #    #     #   #  #     #" << endl;
		cout << "#     #    #    #     # ######  " << endl;
		cout << "#     #    #    ####### #   #   " << endl;
		cout << "#     #    #    #     # #    # " << endl;
		cout << " #####     #    #     # #     # " << endl;
		cout << "----------------------------" << endl;
		cout << "|COVID-19 Management System|\n";
		cout << "----------------------------\n" << endl;
		cout << "1. Register a new account" << endl;
		cout << "2. Login" << endl;
		cout << "3. Exit\n" << endl;
		cout << "Enter your choice(1,2,3): ";
		int choice;
		cin >> choice;
		system("cls");

		switch (choice)
		{
		case 1:
			registration(users);
			saveUsersToFile("userdata.txt", &users);
			dataModified = true;
			system("pause");
			system("cls");
			break;
		case 2:
			login(users);
			saveUsersToFile("userdata.txt", &users);
			dataModified = true;
			system("pause");
			system("cls");
			break;
		case 3:
			if (dataModified == true)
			{
				saveUsersToFile("userdata.txt", &users);
				cout << "User data has been updated." << endl;
			}
			else
			{
				cout << "User data has not been updated." << endl;
			}

			cout << "Thank you for using The UTAR Covid-19 Management System!" << endl;

			return 0;
		default:
			cout << "Invalid choice, please try again." << endl;
			break;
		}
	} while (true);

	return 0;
}

void registration(vector<User>& users)
{
	User user;
	ofstream data;

	string a = "|";
	string input;

	data.open("userdata.txt", ios::app);

	cout << "Please enter your personal details: " << endl;

	// Check if username already exists
	bool validUsername = false;
	while (!validUsername)
	{
		cout << "Username: ";
		cin >> user.username;

		validUsername = true;

		for (const auto& existingUser : users)
		{
			if (existingUser.username == user.username)
			{
				cout << "Username already exists. Please choose a different username." << endl;
				validUsername = false;
				break;
			}
		}
	}

	cout << "Password: ";
	cin >> user.password;

	cout << "Name: ";
	cin.ignore();
	getline(cin, user.name);

	cout << "Age: ";
	bool validInput = false;	//check age is numeric  
	while (!validInput)
	{
		cin >> input;

		bool isNumeric = true;
		for (char c : input)
		{
			if (!isdigit(c))
			{
				isNumeric = false;
				break;
			}
		}

		if (isNumeric)
		{
			int ageInt = stoi(input);
			user.age = ageInt;
			validInput = true;
		}
		else
		{
			cout << "Invalid input. Please enter number." << endl;
		}
	}

	cout << "Address: ";
	cin.ignore();
	getline(cin, user.address);

	cout << "Phone number: ";
	cin >> user.phone;

	cout << "Identification number (IC) (Enter Passport Number if you are non-malaysian): ";
	cin >> user.IC;

	user.category = Category::LOW_RISK;	//initialise temporary default values 
	user.testdate = "00/00/0000";

	users.push_back(user);

	// Save user data to file
	data << user.username << a << user.password << a << user.name << a << user.age << a <<
		user.address << a << user.phone << a << user.IC << a << categoryToInt(user.category) << a << user.testdate << endl;

	cout << "Registration successful!" << endl;
}

void login(vector<User>& users)
{
	string username, password;
	int choice;
	string name = "";
	int index = -1;

	while (name.empty())
	{
		cout << "*********************************************************\n";
		cout << "*            For first time login user.               *\n";
		cout << "*    Please update your test result after login.      *\n";
		cout << "*         Thank you for your cooperation!             *\n";
		cout << "*********************************************************\n";
		cout << "Please enter your username and password to login: " << endl;

		cout << "Username: ";

		cin >> username;

		cout << "Password: ";

		cin >> password;

		// Check if the username and password match any user in the vector
		for (int i = 0; i < users.size(); i++)
		{
			if (users[i].username == username && users[i].password == password)
			{
				// User found, display reminder if applicable and profile menu
				cout << "Login successful! Welcome " << users[i].name << endl;

				reminder(users, users[i].name);

				name = users[i].name;

				index = i;

				break;
			}
		}

		if (name.empty())
		{
			cout << "Incorrect username or password. Please try again." << endl;
		}
	}

	User* currentUser = &users[index];

	do {
		randomQuote();
		cout << "======================================================" << endl;
		cout << "UTAR Covid-19 Management System for students and staff" << endl;
		cout << "======================================================" << endl;
		cout << "Good Day, " << name << "." << endl;
		cout << "||1.View Profile         ||  " << endl;
		cout << "||2.Update Profile       ||  " << endl;
		cout << "||3.Take Test            ||  " << endl;
		cout << "||4.View Your Category   ||  " << endl;
		cout << "||5.Logout               ||  " << endl;
		cout << "Please Enter the Number (1,2,3,4,5)" << endl;
		cin >> choice;
		system("cls");
		switch (choice)
		{
		case 1:
			viewProfile(currentUser);

			break;

		case 2:
			updateProfile(currentUser, users);

			break;

		case 3:
			takeTest(users, name);

			break;

		case 4:
			viewCategory(users, name);

			break;

		case 5:
			name = "";

			break;

		default:
			cout << "Invalid choice. Please try again." << endl;

			break;
		}

		if (choice == 5)
		{
			break;
		}
	} while (!name.empty() || choice != 5);

}

void saveUsersToFile(string filename, vector<User>* users)
{
	ofstream file(filename);
	if (file.is_open())
	{
		for (auto& user : *users)
		{
			file << user.username << "|" << user.password << "|" << user.name << "|" << user.age << "|" << user.address << "|" << user.phone << "|" << user.IC << "|" << categoryToInt(user.category) << "|" << user.testdate << endl;
		}

		file.close();
	}
}

void viewProfile(User* user)
{
	system("cls");
	cout << "==================================================================" << endl;

	cout << "\t\t\tYour Profile\t\t\t" << endl;

	cout << "==================================================================" << endl;

	cout << "Name: " << user->name << endl;

	cout << "Age: " << user->age << endl;

	cout << "Address: " << user->address << endl;

	cout << "Phone number: " << user->phone << endl;

	cout << "IC / Passport Number (non-malaysian): " << user->IC << endl;

	cout << "Latest Covid-19 test date: " << user->testdate << endl;

	cout << "==================================================================" << endl;

	system("pause");

	system("cls");
}

void updateProfile(User* user, vector<User>& users)
{
	do {
		system("cls");
		int choice;
		string name;
		string age;
		string address;
		string phone;
		string IC;
		string password;
		string username;
		int temp = -1;

		cout << "Choose select which you want to update:\n";
		cout << "1. Name: " << user->name << endl;
		cout << "2. Age: " << user->age << endl;
		cout << "3. Address: " << user->address << endl;
		cout << "4. Phone number: " << user->phone << endl;
		cout << "5. IC number: " << user->IC << endl;
		cout << "6. Password: " << user->password << endl;
		cout << "7. Username: " << user->username << endl;
		cout << "8. Back to Profile Menu.\n";

		cin >> choice;
		system("cls");

		if (choice == 1)	//update name
		{
			cout << "Enter your name : ";
			cin.ignore();
			getline(cin, name);
			(*user).name = name;
			cout << "Your name was successfully updated!" << endl;

			saveUsersToFile("userdata.txt", &users);
			dataModified = true;
		}
		else if (choice == 2)	//update age
		{
			cout << "Enter your age : ";
			bool validInput = false;
			while (!validInput)
			{
				cin >> age;
				bool isNumeric = true;
				for (char c : age)
				{
					if (!isdigit(c))
					{
						isNumeric = false;
						break;
					}
				}

				if (isNumeric)
				{
					int ageInt = stoi(age);

					(*user).age = ageInt;

					validInput = true;
					cout << "Your age was successfully updated!" << endl;

					saveUsersToFile("userdata.txt", &users);
					dataModified = true;
				}
				else
				{
					cout << "Invalid input. Please enter number." << endl;
				}
			}
		}
		else if (choice == 3)	//update address
		{
			cout << "Enter your address : ";
			cin.ignore();
			getline(cin, address);
			(*user).address = address;
			cout << "Your address was successfully updated!" << endl;

			saveUsersToFile("userdata.txt", &users);
			dataModified = true;
		}
		else if (choice == 4)	//update phone number
		{
			cout << "Enter your phone number : ";
			cin.ignore();
			getline(cin, phone);
			(*user).phone = phone;
			cout << "Your phone number was successfully updated!" << endl;

			saveUsersToFile("userdata.txt", &users);
			dataModified = true;
		}
		else if (choice == 5)	//update ic number
		{
			cout << "Enter your IC number : ";
			cin.ignore();
			getline(cin, IC);
			(*user).IC = IC;
			cout << "Your IC number was successfully updated!" << endl;

			saveUsersToFile("userdata.txt", &users);
			dataModified = true;
		}
		else if (choice == 6)	// update password
		{
			cout << "Enter your old Password : ";
			string oldPassword;
			cin.ignore();
			getline(cin, oldPassword);

			if (oldPassword == (*user).password)
			{
				cout << "Enter your new Password : ";
				string newPassword;
				getline(cin, newPassword);
				(*user).password = newPassword;
				cout << "Your password was successfully updated!" << endl;

				saveUsersToFile("userdata.txt", &users);
				dataModified = true;
			}
			else
			{
				cout << "Incorrect password. Please try again." << endl;
			}
		}
		else if (choice == 7)	// update username
		{
			cout << "Enter your new username: ";
			cin.ignore();
			getline(cin, username);

			// Check if the new username is unique
			bool isUnique = true;
			for (const auto& otherUser : users)
			{
				if (otherUser.username == username)
				{
					isUnique = false;
					break;
				}
			}

			if (isUnique)
			{
				(*user).username = username;
				cout << "Your username was successfully updated!" << endl;

				saveUsersToFile("userdata.txt", &users);
				dataModified = true;
			}
			else
			{
				cout << "Username already exists. Please choose a different username." << endl;
			}
		}
		else if (choice == 8)	//exit
		{
			return;
		}
		else	//invalid choice
		{
			cout << "Invalid choice. Please try again." << endl;
		}

		// Wait for user to press a key before clearing the screen
		system("pause");
		system("cls");
	} while (true);
}

void takeTest(vector<User>& users, string name)
{
	int testResult = 0;
	string date;
	int symptoms = 0;
	int travelHistory = 0;
	bool hasFever = false;
	bool hasCough = false;
	bool hasBreathingDifficulty = false;
	bool hasBeenToHighRiskArea = false;
	bool hasCloseContact = false;
	ofstream data;

	data.open("userdata.txt", ios::app);
	// Get user ID
	int index = -1;

	// Find user in vector of users
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].name == name)
		{
			index = i;
			break;
		}
	}

	cout << "Do you have a fever? (1 for yes, 0 for no)\n";
	cin >> hasFever;

	cout << "Do you have a cough? (1 for yes, 0 for no)\n";
	cin >> hasCough;

	cout << "Do you have difficulty breathing? (1 for yes, 0 for no)\n";
	cin >> hasBreathingDifficulty;

	cout << "Have you traveled to a high-risk area in the past 14 days? (1 for yes, 0 for no)\n";
	cin >> hasBeenToHighRiskArea;

	cout << "Have you been in close contact with a confirmed Covid-19 infected individual? (1 for yes, 0 for no)\n";
	cin >> hasCloseContact;

	// Get test result
	cout << "Enter test result (1 for positive, 0 for negative): ";
	cin >> testResult;

	if (hasFever && hasCough && hasBreathingDifficulty)
	{
		symptoms = 3;
	}
	else if (hasFever && hasCough)
	{
		symptoms = 2;
	}
	else if (hasFever || hasCough)
	{
		symptoms = 1;
	}

	if (hasBeenToHighRiskArea)
	{
		travelHistory = 1;
	}

	if (symptoms == 3 || (symptoms == 2 && travelHistory == 1) || (testResult == 1))
	{
		users[index].category = Category::POSITIVE;
	}
	else if (symptoms == 2 || (symptoms == 1 && travelHistory == 1))
	{
		users[index].category = Category::SUSPECTED;
	}
	else if (hasCloseContact == 1)
	{
		users[index].category = Category::CLOSE_CONTACT;
	}
	else if (travelHistory == 1)
	{
		users[index].category = Category::TRAVEL_HISTORY;
	}
	else
	{
		users[index].category = Category::LOW_RISK;
	}


	// Get test date
	do {
		cout << "Enter test date (DD/MM/YYYY): ";

		cin >> date;

		users[index].testdate = date;

		if (isValidDate(date))
		{
			users[index].testdate = date;
			data << "," << users[index].testdate << endl;
			cout << "Test date recorded.\n";
			check = checkIfNeedsTesting(users, name);
		}
		else
		{
			cout << "Invalid date format. Test date not recorded.\n";
		}
	} while (!isValidDate(date));
	saveUsersToFile("userdata.txt", &users);
	dataModified = true;

	data.close();

}

void viewCategory(vector<User>& users, string name)
{
	int a = -1;
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].name == name)
		{
			a = i;
			break;
		}
	}

	if (a != -1)
	{
		switch (users[a].category)
		{
		case Category::POSITIVE:
			cout << "User " << users[a].name << " is positive for Covid-19.\n";

			cout << "Please follow these actions:\n";

			cout << "-Decide whether a Hospital Quarantine or Home Surveillance Order (HSO) subjected to MOH instructions would be more appropriate for your situation.\n";

			cout << "-Report to immediate supervisor (for staff) / respective lecturer (for students)\n";

			cout << "-Report to Bilik Gerakan for close monitoring and sanitisation process.\n";

			cout << "-Undergo qurantine for a period of 7 days.\n";

			cout << "-However, you may be released earlier if you are testesd negative on Day 4 (with supervision of medical practitioner)\n";

			break;

		case Category::SUSPECTED:
			cout << "User " << users[a].name << " is suspected to have Covid-19.\n";

			cout << "Please answer the following questions:\n";

			bool isSymptomatic, liveWithPositive;

			cout << "- Do you feel any symptoms of COVID-19? (1 for yes, 0 for no)\n";

			cout << "  Common symptoms include fever, cough, shortness of breath, fatigue, body aches, headache, loss of taste or smell.\n";

			cin >> isSymptomatic;

			cout << "- Do you live with someone who tested positive for COVID-19? (1 for yes, 0 for no) ";

			cin >> liveWithPositive;

			if (isSymptomatic)
			{
				// Option 1: Symptomatic
				cout << "Please follow these actions:\n";

				cout << "-Home Surveillance Order (HSO) subjected to MOH instructions or\n";

				cout << "-Self-isolate and scheduled to Work From Home (Staff) / Online Teaching and Learning (Student) subjected to UTAR procedures\n";

				cout << "-Report to immediate supervisor (for staff) / respective lecturer (for students)\n";

				cout << "-Report to Bilik Gerakan for close monitoring and sanitisation process.\n";

				cout << "-Undergo qurantine for a period of 3 days\n";

				cout << "-Note: If your self-test results on the 3rd day were negative and no symptoms (or improved greatly), you will be allowed to work/study as usual and access campus\n";

				cout << "-Additionally, You may refer to a doctor if you continue to have symptoms even if the self-test result on the 3rd day is negative.\n";
			}
			else if (liveWithPositive)
			{
				// Option 2: Living with positive case
				cout << "Please follow these actions:\n";

				cout << "-Home Surveillance Order (HSO) subjected to MOH instructions or\n";

				cout << "Self-isolate and scheduled to Work From Home (Staff) / Online Teaching and Learning (Student) subjected to UTAR procedures\n";

				cout << "-Report to immediate supervisor (for staff) / respective lecturer (for students)\n";

				cout << "-Report to Bilik Gerakan for close monitoring and sanitisation process.\n";

				cout << "-Undergo qurantine for a period of 3 days\n";

				cout << "-Note: If your self-test results on the 3rd day were negative and no symptoms (or improved greatly), you will be allowed to work/study as usual and access campus\n";
			}
			else
			{
				// Option 3: Not symptomatic and not living with positive case
				cout << "Please follow these actions:\n";

				cout << "-Home Surveillance Order (HSO) subjected to MOH instructions or\n";

				cout << "-Self-isolate and scheduled to Work From Home (Staff) / Online Teaching and Learning (Student) subjected to UTAR procedures\n";

				cout << "-Report to immediate supervisor (for staff) / respective lecturer (for students)\n";

				cout << "-Report to Bilik Gerakan for close monitoring and sanitisation process.\n";
			}

			break;
			break;
		case Category::CLOSE_CONTACT:
			cout << "User " << users[a].name << " has been in close contact with a confirmed Covid-19 case.\n";

			cout << "Suggested action: Please self-isolate and get tested for Covid-19 as soon as possible.\n";

			break;

		case Category::TRAVEL_HISTORY:
			cout << "User " << users[a].name << " has recently traveled to a high-risk area for Covid-19.\n";

			break;

		case Category::LOW_RISK:
			check = checkIfNeedsTesting(users, name);
			if (check == false)
			{
				cout << "User " << users[a].name << " is at low risk for Covid-19.\n";

				break;
			}
			else
			{
				break;
			}

		default:
			cout << "Invalid category.\n";

			break;
		}
	}
	else
	{
		cout << "User not found.\n";
	}

	system("pause");

	system("cls");

}

void reminder(vector<User>& users, string name)
{
	// Get current local time
	time_t now = time(0);

	tm localtm;

	localtime_s(&localtm, &now);

	// Get current day of the month
	int currentDay = localtm.tm_mday;

	int temp = -1;
	// Find user with specified name in the vector
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].name == name)
		{
			temp = i;
			break;
		}
	}

	// Check user's category and test date

	if (users[temp].testdate == "00/00/0000") 
	{
		cout << "Reminder: You should get tested for Covid-19.\n";
	}
	else
	{
		// User needs to get tested for Covid-19
		stringstream ss(users[temp].testdate);

		string day, month, year;

		getline(ss, day, '/');

		getline(ss, month, '/');

		getline(ss, year);

		// Calculate days since user's last test
		// Calculate days since user's last test
		int testDay = stoi(day);

		int testMonth = stoi(month) - 1;

		int testYear = stoi(year) - 1900;

		tm testtm = { 0 };

		testtm.tm_mday = testDay;

		testtm.tm_mon = testMonth;

		testtm.tm_year = testYear;

		time_t testtime = mktime(&testtm);

		double secondsSinceTest = difftime(now, testtime);

		int daysSinceTest = static_cast<int> (secondsSinceTest / (24 * 3600));

		// Remind user to get tested again if days since the last test is more than or equal to 3 and less than 7 days
		if (daysSinceTest >= 3 && daysSinceTest < 7)
		{
			cout << "Reminder: It has been " << daysSinceTest << " days since your last Covid-19 test. You should get tested again.\n";
		}

		if (users[temp].category == Category::CLOSE_CONTACT || users[temp].category == Category::SUSPECTED)
		{
			cout << "Reminder: You are a close contact/suspected case. Please take the test daily.\n";

			takeTest(users, name);
			dataModified = true;
		}
		else
		{
			updateCategory(users, temp, daysSinceTest, name);
			stringstream ss;
			ss << setfill('0') << setw(2) << localtm.tm_mday << '/' <<
				setfill('0') << setw(2) << localtm.tm_mon + 1 << '/' <<
				localtm.tm_year + 1900;
			//change testdate to today's date
			users[temp].testdate = ss.str();
		}
	}
}

int categoryToInt(Category category)
{
	switch (category)
	{
	case Category::LOW_RISK:
		return 0;
	case Category::TRAVEL_HISTORY:
		return 1;
	case Category::SUSPECTED:
		return 2;
	case Category::CLOSE_CONTACT:
		return 3;
	case Category::POSITIVE:
		return 4;
	default:
		return -1;
	}
}

bool isValidDate(const string& dateStr)
{
	// Use regular expressions to check the format
	regex dateRegex("\\d{2}/\\d{2}/\\d{4}");
	if (!regex_match(dateStr, dateRegex))
	{
		return false;
	}

	// Check if the date is a valid date
	stringstream ss(dateStr);
	int day, month, year;
	char sep;
	ss >> day >> sep >> month >> sep >> year;

	if (ss.fail() || (sep != '/') || (month < 1) || (month > 12) || (day < 1) || (day > 31))
	{
		return false;
	}

	// Check for specific invalid dates
	if ((month == 2) && (day > 29))
	{
		return false;
	}
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day > 30))
	{
		return false;
	}
	else if ((month == 2) && (day == 29) && ((year % 4 != 0) || ((year % 100 == 0) && (year % 400 != 0))))
	{
		return false;
	}

	return true;
}

void updateCategory(vector<User>& users, int index, int daysSinceTest, string name)

{
	if (users[index].category == Category::POSITIVE && daysSinceTest >= 7)
	{
		users[index].category = Category::LOW_RISK;
		check = checkIfNeedsTesting(users, name);
		if (check == false)
			cout << "You have completed 7 days of isolation.\n";
		saveUsersToFile("userdata.txt", &users);
		dataModified = true;
	}
}

bool checkIfNeedsTesting(vector<User>& users, string name)
{
	// Get current local time
	time_t now = time(0);

	tm localtm;

	localtime_s(&localtm, &now);

	// Get current day of the month
	int currentDay = localtm.tm_mday;

	int temp = -1;;

	// Find user with specified name in the vector
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].name == name)
		{
			temp = i;

			break;
		}
	}

	// Check user's category and test date
	// User needs to get tested for Covid-19
	stringstream ss(users[temp].testdate);

	string day, month, year;

	getline(ss, day, '/');

	getline(ss, month, '/');

	getline(ss, year);

	// Calculate days since user's last test
	int testDay = stoi(day);

	int testMonth = stoi(month) - 1;

	int testYear = stoi(year) - 1900;

	tm testtm = { 0 };

	testtm.tm_mday = testDay;

	testtm.tm_mon = testMonth;

	testtm.tm_year = testYear;

	time_t testtime = mktime(&testtm);

	double secondsSinceTest = difftime(now, testtime);

	int daysSinceTest = static_cast<int> (secondsSinceTest / (24 * 3600));

	if (daysSinceTest >= 7 && users[temp].category == Category::POSITIVE)
	{
		return true;
	}
	else if (daysSinceTest == 0 && users[temp].category == Category::LOW_RISK)
	{
		return false;
	}
	else
	{
		return false;
	}
}

int randomQuote()
{
	int quote;
	srand(time(NULL));	// current time is the seed
	quote = rand() % 10 + 1;	//random number 1-10

	cout << "\n";
	cout << "A quote for you:";

	switch (quote)
	{
	case 1:
		cout << "\"Health is a state of complete harmony of the body, mind and spirit. When one is free from physical disabilities and mental distractions, the gates of the soul open.\" -B.K.S.Iyengar\n";
		cout << "\n";
		break;

	case 2:
		cout << "\"To ensure good health: eat lightly, breathe deeply, live moderately, cultivate cheerfulness, and maintain an interest in life.\" -William Londen\n";
		cout << "\n";
		break;

	case 3:
		cout << "\"It is health that is the real wealth, and not pieces of gold and silver.\" -Mahatma Gandhi\n";
		cout << "\n";
		break;

	case 4:
		cout << "\"The cheerful mind perseveres, and the strong mind hews its way through a thousand difficulties.\" -Swami Vivekananda\n";
		cout << "\n";
		break;

	case 5:
		cout << "\"Keeping your body healthy is an expression of gratitude to the whole cosmos- the trees, the clouds, everything.\" -Thich Nhat Hanh\n";
		cout << "\n";
		break;

	case 6:
		cout << "\"The doctor of the future will give no medicine, but will instruct his patients in care of the human frame, in diet, and in the cause and prevention of disease.\" -Thomas Edison\n";
		cout << "\n";
		break;

	case 7:
		cout << "\"A good laugh and a long sleep are the best cures in the doctor�s book.\" -Irish proverb\n";
		cout << "\n";
		break;

	case 8:
		cout << "\"Good health is not something we can buy. However, it can be an extremely valuable savings account.\" -Anne Wilson Schaef\n";
		cout << "\n";
		break;

	case 9:
		cout << "\"He who has health has hope, and he who has hope has everything.\" -Thomas Carlyle\n";
		cout << "\n";
		break;

	case 10:
		cout << "\"The first wealth is health.\" -Ralph Waldo Emerson\n";
		cout << "\n";
		break;

	default:
		cout << "NO quote here! UwU" << endl;

		break;
	}

	return 0;
}
