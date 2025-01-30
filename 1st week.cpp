#include<bits/stdc++.h>

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
    const string filename = "employees.data";

    void saveToFile();
    void loadFromFile();

public:
    EmployeeSystem() { loadFromFile(); }
    ~EmployeeSystem() { saveToFile(); }

    Employee addEmployee(string fname, string lname);
    void showAllEmployees();
    void updateEmployee();
};

long EmployeeSystem::nextEmpID = 0;

void EmployeeSystem::saveToFile() {
    ofstream ofs(filename, ios::trunc);
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
    ifstream ifs(filename);
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

int main() {
    EmployeeSystem system;
    int choice;

    do {
        cout << "\n*** Employee Management System ***" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Show All Employees" << endl;
        cout << "3. Update Employee" << endl;
        cout << "4. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string fname, lname;
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            system.addEmployee(fname, lname);
            break;
        }
        case 2:
            system.showAllEmployees();
            break;
        case 3:
            system.updateEmployee();
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
