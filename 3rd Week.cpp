#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

class Employee {
private:
    long empID;
    string firstName;
    string lastName;

public:
    Employee() {}
    Employee(long id, string fname, string lname) : empID(id), firstName(fname), lastName(lname) {}

    long getEmpID() const { return empID; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    void setFirstName(const string& fname) { firstName = fname; }
    void setLastName(const string& lname) { lastName = lname; }
    void setFullName(const string& fname, const string& lname) {
        firstName = fname;
        lastName = lname;
    }

    friend ofstream& operator<<(ofstream& ofs, const Employee& emp);
    friend ifstream& operator>>(ifstream& ifs, Employee& emp);
    friend ostream& operator<<(ostream& os, const Employee& emp);
};

ofstream& operator<<(ofstream& ofs, const Employee& emp) {
    ofs << emp.empID << endl;
    ofs << emp.firstName << endl;
    ofs << emp.lastName << endl;
    return ofs;
}

ifstream& operator>>(ifstream& ifs, Employee& emp) {
    ifs >> emp.empID;
    ifs >> emp.firstName;
    ifs >> emp.lastName;
    return ifs;
}

ostream& operator<<(ostream& os, const Employee& emp) {
    os << "Employee ID: " << emp.getEmpID() << endl;
    os << "Name: " << emp.getFirstName() << " " << emp.getLastName() << endl;
    return os;
}

class EmployeeSystem {
private:
    map<long, Employee> employees;
    static long nextEmpID;
    const string empFile = "employees.data";
    const string attendanceFile = "attendance.txt";
    const string adminName = "Zubayed Amin";
    const string adminPassword = "2677";

    void saveToFile();
    void loadFromFile();

    bool isValidDate(const string& date) {
        regex datePattern("\\d{4}-\\d{2}-\\d{2}");
        return regex_match(date, datePattern);
    }

    string getHiddenInput();

    void generateWeeklyReport();
    void generateMonthlyReport();

public:
    EmployeeSystem() { loadFromFile(); }
    ~EmployeeSystem() { saveToFile(); }

    Employee addEmployee(string fname, string lname);
    void showAllEmployees();
    void updateEmployee();
    void markAttendance();
    void adminPanel();
};

long EmployeeSystem::nextEmpID = 0;

void EmployeeSystem::saveToFile() {
    ofstream ofs(empFile, ios::trunc);
    if (!ofs) {
        cerr << "Error saving employee records to file!" << endl;
        return;
    }
    ofs << nextEmpID << endl;
    for (const auto& emp : employees) {
        ofs << emp.second;
    }
    ofs.close();
}

void EmployeeSystem::loadFromFile() {
    ifstream ifs(empFile);
    if (!ifs) {
        cerr << "No existing employee records found. Starting fresh..." << endl;
        return;
    }
    ifs >> nextEmpID;
    Employee emp;
    while (ifs >> emp) {
        employees.insert({ emp.getEmpID(), emp });
    }
    ifs.close();
}

string EmployeeSystem::getHiddenInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

Employee EmployeeSystem::addEmployee(string fname, string lname) {
    nextEmpID++;
    Employee emp(nextEmpID, fname, lname);
    employees.insert({ nextEmpID, emp });
    cout << "Employee Added Successfully!" << endl;
    cout << emp;
    return emp;
}

void EmployeeSystem::showAllEmployees() {
    if (employees.empty()) {
        cout << "No employees to display!" << endl;
        return;
    }

    for (const auto& emp : employees) {
        cout << emp.second << endl;
    }
}

void EmployeeSystem::updateEmployee() {
    long empID;
    cout << "Enter Employee ID to Update: ";
    cin >> empID;

    auto it = employees.find(empID);
    if (it != employees.end()) {
        int choice;
        cout << "What would you like to update?" << endl;
        cout << "1. First Name" << endl;
        cout << "2. Last Name" << endl;
        cout << "3. Full Name" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string newFirstName;
            cout << "Enter New First Name: ";
            cin >> newFirstName;
            it->second.setFirstName(newFirstName);
            cout << "First Name Updated Successfully!" << endl;
            break;
        }
        case 2: {
            string newLastName;
            cout << "Enter New Last Name: ";
            cin >> newLastName;
            it->second.setLastName(newLastName);
            cout << "Last Name Updated Successfully!" << endl;
            break;
        }
        case 3: {
            string newFirstName, newLastName;
            cout << "Enter New First Name: ";
            cin >> newFirstName;
            cout << "Enter New Last Name: ";
            cin >> newLastName;
            it->second.setFullName(newFirstName, newLastName);
            cout << "Full Name Updated Successfully!" << endl;
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
        }
    } else {
        cout << "Employee ID not found." << endl;
    }
}

void EmployeeSystem::markAttendance() {
    long empID;
    string date, status;
    cout << "Enter Employee ID: ";
    cin >> empID;

    if (employees.find(empID) == employees.end()) {
        cout << "Employee ID not found!" << endl;
        return;
    }

    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;
    if (!isValidDate(date)) {
        cout << "Invalid date format! Please use YYYY-MM-DD." << endl;
        return;
    }

    cout << "Enter Status (Present/Absent/Leave): ";
    cin >> status;
    if (status != "Present" && status != "Absent" && status != "Leave") {
        cout << "Invalid status! Please enter Present, Absent, or Leave." << endl;
        return;
    }

    ofstream ofs(attendanceFile, ios::app);
    if (!ofs) {
        cerr << "Error saving attendance record!" << endl;
        return;
    }
    ofs << empID << " " << date << " " << status << endl;
    ofs.close();
    cout << "Attendance marked successfully!" << endl;
}

void EmployeeSystem::generateWeeklyReport() {
    ifstream ifs(attendanceFile);
    if (!ifs) {
        cerr << "Error reading attendance file!" << endl;
        return;
    }

    string line;
    map<long, map<string, string>> weeklyAttendance;

    cout << "Enter the start date of the week (YYYY-MM-DD): ";
    string weekStartDate;
    cin >> weekStartDate;

    struct tm tm = {};
    istringstream ss(weekStartDate);
    ss >> get_time(&tm, "%Y-%m-%d");

    time_t rawtime = mktime(&tm);
    tm = *localtime(&rawtime);
    tm.tm_mday -= tm.tm_wday;

    char buffer[80];
    for (int i = 0; i < 7; i++) {
        strftime(buffer, 80, "%Y-%m-%d", &tm);
        string date(buffer);

        while (getline(ifs, line)) {
            stringstream ss(line);
            long empID;
            string attendanceDate, status;
            ss >> empID >> attendanceDate >> status;

            if (attendanceDate == date) {
                weeklyAttendance[empID][attendanceDate] = status;
            }
        }

        tm.tm_mday++;
        rawtime = mktime(&tm);
        tm = *localtime(&rawtime);
    }

    for (const auto& emp : employees) {
        cout << "Employee: " << emp.second.getFirstName() << " " << emp.second.getLastName() << endl;
        for (const auto& day : weeklyAttendance[emp.first]) {
            cout << day.first << ": " << day.second << endl;
        }
        cout << endl;
    }
}

void EmployeeSystem::generateMonthlyReport() {
    ifstream ifs(attendanceFile);
    if (!ifs) {
        cerr << "Error reading attendance file!" << endl;
        return;
    }

    string line;
    map<long, map<string, string>> monthlyAttendance;

    cout << "Enter the month (YYYY-MM): ";
    string monthStartDate;
    cin >> monthStartDate;

    struct tm tm = {};
    istringstream ss(monthStartDate);
    ss >> get_time(&tm, "%Y-%m");

    tm.tm_mday = 1;
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d", &tm);
    string startOfMonth(buffer);

    tm.tm_mday = 31;
    strftime(buffer, 80, "%Y-%m-%d", &tm);
    string endOfMonth(buffer);

    while (getline(ifs, line)) {
        stringstream ss(line);
        long empID;
        string attendanceDate, status;
        ss >> empID >> attendanceDate >> status;

        if (attendanceDate >= startOfMonth && attendanceDate <= endOfMonth) {
            monthlyAttendance[empID][attendanceDate] = status;
        }
    }

    for (const auto& emp : employees) {
        cout << "Employee: " << emp.second.getFirstName() << " " << emp.second.getLastName() << endl;
        for (const auto& day : monthlyAttendance[emp.first]) {
            cout << day.first << ": " << day.second << endl;
        }
        cout << endl;
    }
}

void EmployeeSystem::adminPanel() {
    cout << "Admin Name: " << adminName << endl;
    cout << "Enter Admin Password: ";
    string password = getHiddenInput();

    if (password != adminPassword) {
        cout << "Incorrect Password! Access denied." << endl;
        return;
    }

    cout << "Access granted! Welcome to the Admin Panel, " << adminName << "." << endl;

    int choice;
    do {
        cout << "\n--- Admin Panel ---" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Show All Employees" << endl;
        cout << "3. Update Employee" << endl;
        cout << "4. Mark Attendance" << endl;
        cout << "5. Exit Admin Panel" << endl;
        cout << "6. Weekly Attendance Report" << endl;
        cout << "7. Monthly Attendance Report" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string fname, lname;
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            addEmployee(fname, lname);
            break;
        }
        case 2:
            showAllEmployees();
            break;
        case 3:
            updateEmployee();
            break;
        case 4:
            markAttendance();
            break;
        case 5:
            cout << "Exiting Admin Panel..." << endl;
            break;
        case 6:
            generateWeeklyReport();
            break;
        case 7:
            generateMonthlyReport();
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);
}

int main() {
    EmployeeSystem system;
    system.adminPanel();
    return 0;
}
