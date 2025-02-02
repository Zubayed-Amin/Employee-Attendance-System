#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <ctime>
#include <sstream>
using namespace std;

struct Employee {
    int id;
    string name;
};

vector<Employee> employees;

const string adminName = "Zubayed Amin";

int lastEmployeeID = 100;

void addEmployee() {
    Employee emp;
    emp.id = ++lastEmployeeID;
    cout << "Enter Name: ";
    cin >> emp.name;
    employees.push_back(emp);
    cout << "Employee added successfully with ID: " << emp.id << "\n";
}

void showEmployees() {
    if (employees.empty()) {
        cout << "No employees to display!\n";
        return;
    }

    cout << "\n+-------------------------------+\n";
    cout << "| ID        | Name              |\n";
    cout << "+-------------------------------+\n";
    for (const auto& emp : employees) {
        cout << "| " << left << setw(10) << emp.id << " | " << setw(16) << emp.name << " |\n";
    }
    cout << "+-------------------------------+\n";
}

void updateEmployee() {
    int id;
    cout << "Enter Employee ID to update: ";
    cin >> id;
    for (auto& emp : employees) {
        if (emp.id == id) {
            cout << "Enter New Name: ";
            cin >> emp.name;
            cout << "Employee details updated!\n";
            return;
        }
    }
    cout << "Employee not found!\n";
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string date = to_string(1900 + ltm->tm_year) + "-" +
                  to_string(1 + ltm->tm_mon) + "-" +
                  to_string(ltm->tm_mday);
    return date;
}

string getWeekDateRange() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream dateRange;
    ltm->tm_mday -= ltm->tm_wday;
    mktime(ltm);
    string startDate = to_string(1900 + ltm->tm_year) + "-" +
                       to_string(1 + ltm->tm_mon) + "-" +
                       to_string(ltm->tm_mday);

    ltm->tm_mday += 6;
    mktime(ltm);
    string endDate = to_string(1900 + ltm->tm_year) + "-" +
                     to_string(1 + ltm->tm_mon) + "-" +
                     to_string(ltm->tm_mday);

    dateRange << startDate << " to " << endDate;
    return dateRange.str();
}

string getMonthRange() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream dateRange;
    string startDate = to_string(1900 + ltm->tm_year) + "-" +
                       to_string(1 + ltm->tm_mon) + "-01";

    string endDate = to_string(1900 + ltm->tm_year) + "-" +
                     to_string(1 + ltm->tm_mon) + "-" +
                     to_string(ltm->tm_mday);

    dateRange << startDate << " to " << endDate;
    return dateRange.str();
}

void markAttendance() {
    string currentDate = getCurrentDate();
    string status;
    cout << "\nMark Attendance for " << currentDate << ":\n";

    for (const auto& emp : employees) {
        int statusChoice;
        cout << "Enter attendance status for Employee ID " << emp.id << " (" << emp.name << "):\n";
        cout << "1. Present\n";
        cout << "2. Absent\n";
        cout << "3. Leave\n";
        cout << "Enter choice: ";
        cin >> statusChoice;

        switch (statusChoice) {
            case 1: status = "Present"; break;
            case 2: status = "Absent"; break;
            case 3: status = "Leave"; break;
            default:
                cout << "Invalid choice! Attendance not recorded.\n";
                return;
        }

        ofstream file("attendance.txt", ios::app);
        file << "Date: " << currentDate << " - Employee ID: " << emp.id << " - Name: " << emp.name
             << " - Status: " << status << endl;
        file.close();

        cout << "Attendance marked as " << status << " for Employee ID " << emp.id << " (" << emp.name << ")\n";
    }
}

void showAttendance(string period) {
    ifstream file("attendance.txt");
    string line;
    string dateRange = (period == "Weekly") ? getWeekDateRange() : getMonthRange();
    cout << "\nAttendance Report (" << period << " - " << dateRange << "):\n";
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

bool login(string& enteredPass) {
    string pass = "";
    const string correctPass = "2677";
    cout << "Enter Password: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !pass.empty()) {
            cout << "\b \b";
            pass.pop_back();
        } else if (ch != '\b') {
            pass.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;

    if (pass == correctPass) {
        enteredPass = pass;
        cout << "Welcome, " << adminName << "!\n";
        return true;
    }
    return false;
}

int main() {
    string enteredPass;
    if (!login(enteredPass)) {
        cout << " Error Password";
        return 0;
    }

    int choice;
    do {
        cout << "\n1. Add Employee";
        cout << "\n2. Show All Employees";
        cout << "\n3. Update Employee";
        cout << "\n4. Mark Attendance";
        cout << "\n5. Weekly Attendance Report";
        cout << "\n6. Monthly Attendance Report";
        cout << "\n7. Exit Admin Panel";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: showEmployees(); break;
            case 3: updateEmployee(); break;
            case 4: markAttendance(); break;
            case 5: showAttendance("Weekly"); break;
            case 6: showAttendance("Monthly"); break;
            case 7: cout << "Exiting Admin Panel...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 7);

    return 0;
}
