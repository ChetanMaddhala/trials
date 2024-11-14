#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

using namespace std;

class Car {
    private:
    string brand;
    string model;
    int year;
    double price;

public:
    Car(string b, string m, int y, double p) : brand(b), model(m), year(y), price(p) {}
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    double getPrice() const { return price; }
    void displayCar() const {
        cout << "Brand: " << brand << ", Model: " << model << ", Year: " << year << ", Price: ₹" << price << endl;
    }

    string carInfo() const {
        return "Brand: " + brand + ", Model: " + model + ", Year: " + to_string(year) + ", Price: ₹" + to_string(price) + "\n";
    }

};


class Employee {
public:
    string username;
    string password;
    string contactNumber;

    Employee() : username(""), password(""), contactNumber("") {}

    Employee(string u, string p, string c) : username(u), password(p), contactNumber(c) {}
};


class Customer {
public:
    string name;
    Car purchasedCar;

    Customer(string n, const Car& c) : name(n), purchasedCar(c) {}

};

class CarDealership {
   private:
    vector<Car> inventory;
    unordered_map<string, Employee> employees;
    vector<Customer> soldCars;
    int activeEmployees = 0;

public:
    void addCar(const Car& car) {
        inventory.push_back(car);
        cout << "Car added to inventory.\n";
    }

    void showInventory() const {
        if (inventory.empty()) {
            cout << "No cars available in the inventory.\n";
            return;
        }

        cout << "Current inventory:\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << i + 1 << ". ";
            inventory[i].displayCar();
        }
    }

    bool isInventoryEmpty() const {
        return inventory.empty();
    }

    bool sellCar(int index, const string& customerName) {
        if (index < 0 || index >= static_cast<int>(inventory.size())) {
            cout << "Invalid car index.\n";
            return false;
        }

        cout << "Selling car: ";
        inventory[index].displayCar();
        soldCars.push_back(Customer(customerName, inventory[index]));
        inventory.erase(inventory.begin() + index);
        cout << "Car sold successfully.\n";
        return true;
    }

    void addEmployee(const string& username, const string& password, const string& contactNumber) {
        employees[username] = Employee(username, password, contactNumber);
        cout << "Employee added successfully.\n";
    }

    bool verifyEmployee(const string& username, const string& password) {
        auto it = employees.find(username);
        if (it != employees.end() && it->second.password == password) {
            ++activeEmployees;
            return true;
        }
        return false;
    }

    void logoutEmployee() {
        --activeEmployees;
    }

    void generateReport(const string& reportType) const {
        // Open file for writing
        ofstream reportFile;

        if (reportType == "admin") {
            reportFile.open("admin_report.txt");
            reportFile << "Admin Report:\n\n";
            
            if (inventory.empty()) {
            reportFile << "No cars available in the inventory.\n";
        } else {
            reportFile << "\nCurrent Inventory:\n";
            for (size_t i = 0; i < inventory.size(); ++i) {
                reportFile << i + 1 << ". "<<inventory[i].carInfo();
            }
        }
        
        reportFile << "\n-------------------------\n";
            // Write sold cars information
            if (soldCars.empty()) {
                reportFile << "No cars have been sold yet.\n";
            } else {
                reportFile << "Customer Section (Sold Cars):\n";
                for (const auto& customer : soldCars) {
                    reportFile << "Customer: " << customer.name << ", Car Purchased: ";
                    reportFile << customer.purchasedCar.carInfo() << "\n";
                }
            }
            reportFile << "\n-------------------------\n";
            // Write employee details
            if (employees.empty()) {
                reportFile << "\nNo employees found.\n";
            } else {
                reportFile << "\nEmployee Details:\n";
                for (const auto& pair : employees) {
                    const Employee& emp = pair.second;
                    reportFile << "Username: " << emp.username
                               << ", Contact Number: " << emp.contactNumber << "\n";
                }
            }
            reportFile << "\n-------------------------\n";
        } else if (reportType == "employee") {
            reportFile.open("employee_inventory_report.txt");
            reportFile << "Employee Report (Inventory):\n\n";
            
            // Show inventory to employees
            if (inventory.empty()) {
                reportFile << "No cars available in the inventory.\n";
            } else {
                reportFile << "Current Inventory:\n";
                for (const auto& car : inventory) {
                    reportFile << car.carInfo();
                }
            }

        } else if (reportType == "customer") {
            reportFile.open("customer_inventory_report.txt");
            reportFile << "Customer Report (Inventory with Employee Info):\n\n";
            
            // Show inventory with employee details to customers
            if (inventory.empty()) {
                reportFile << "No cars available in the inventory.\n";
            } else {
                reportFile << "Current Inventory:\n";
                for (const auto& car : inventory) {
                    reportFile << car.carInfo();

                    // Display employee contact info with inventory for customers
                    for (const auto& pair : employees) {
                        const Employee& emp = pair.second;
                        reportFile << "Assisting Employee: " << emp.username
                                   << ", Contact Number: " << emp.contactNumber << "\n";
                    }
                }
            }

        } else {
            cout << "Invalid report type.\n";
            return;
        }

        // Close the file after writing
        reportFile.close();
        cout << "Report has been generated successfully in the file.\n";
    }
};


// Function declarations
bool verifyAdmin();
bool verifyEmployee(CarDealership& dealership);
void adminSection(CarDealership& dealership);
void employeeSection(CarDealership& dealership);
void reportSection(CarDealership& dealership);
void customerSection(CarDealership& dealership);

bool verifyAdmin() {
    const string adminUsername = "chetan";
    const string adminPassword = "car@777";

    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    if (username == adminUsername && password == adminPassword) {
        cout << "Admin login successful.\n";
        return true;
    } else {
        cout << "Invalid admin credentials. Access denied.\n";
        return false;
    }
}

bool verifyEmployee(CarDealership& dealership) {
    string username, password;
    cout << "Enter employee username: ";
    cin >> username;
    cout << "Enter employee password: ";
    cin >> password;

    if (dealership.verifyEmployee(username, password)) {
        cout << "Employee login successful.\n";
        return true;
    } else {
        cout << "Invalid employee credentials. Access denied.\n";
        return false;
    }
}

void adminSection(CarDealership& dealership) {
    if (!verifyAdmin()) {
        return;
    }

    int choice;
    while (true) {
        cout << "\nAdmin Section\n";
        cout << "1. Add a Car\n";
        cout << "2. View Inventory\n";
        cout << "3. Add Employee\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // To avoid any leftover characters in the buffer

        switch (choice) {
            case 1: {
                string brand, model;
                int year;
                double price;
                cout << "Enter car brand: ";
                getline(cin, brand);
                cout << "Enter car model: ";
                getline(cin, model);
                cout << "Enter car year: ";
                cin >> year;
                cout << "Enter car price: ";
                cin >> price;
                cin.ignore();  // Ignore any extra input
                Car newCar(brand, model, year, price);
                dealership.addCar(newCar);
                break;
            }
            case 2:
                dealership.showInventory();
                break;
            case 3: {
                string username, password, contact;
                cout << "Enter employee username: ";
                getline(cin, username);
                cout << "Enter employee password: ";
                getline(cin, password);
                cout << "Enter employee contact number: ";
                getline(cin, contact);
                dealership.addEmployee(username, password, contact);
                break;
            }
            case 4:
                cout << "Logging out of Admin section.\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void employeeSection(CarDealership& dealership) {
    if (!verifyEmployee(dealership)) {
        return;
    }

    int choice;
    while (true) {
        cout << "\nEmployee Section\n";
        cout << "1. View Inventory\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                dealership.showInventory();
                break;
            case 2:
                cout << "Logging out of Employee section.\n";
                dealership.logoutEmployee();
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void reportSection(CarDealership& dealership) {
    int userType;
    cout << "\nReport Section\n";
    cout << "1. Admin\n";
    cout << "2. Employee\n";
    cout << "3. customer\n";
    cout << "Enter your choice: ";
    cin >> userType;

    bool verified = false;
    string reportType;
    if (userType == 1) {
        verified = verifyAdmin();
        reportType = "admin";
    } else if (userType == 2) {
        verified = verifyEmployee(dealership);
        reportType = "employee";
    } 
     else if (userType == 3) {
        reportType = "customer";
        dealership.generateReport(reportType);
    }
    else {
        cout << "Invalid choice.\n";
        return;
    }

    if (verified) {
        dealership.generateReport(reportType);
    }
}



void customerSection(CarDealership& dealership) {
    cout << "\nCustomer Section\n";
    dealership.showInventory();

    if (dealership.isInventoryEmpty()) {
        return;
    }

    char buyChoice;
    cout << "Would you like to buy a car? (y/n): ";
    cin >> buyChoice;

    if (buyChoice == 'y' || buyChoice == 'Y') {
        int carIndex;
        cout << "Enter the car number you want to buy: ";
        cin >> carIndex;
        cin.ignore();  // Clear buffer for customer name input
        string customerName;
        cout << "Enter your name: ";
        getline(cin, customerName);
        if (dealership.sellCar(carIndex - 1, customerName)) {
            cout << "Thank you for your purchase!\n";
        }
    }
}

int main() 
{
    CarDealership dealership;

    int choice;
    while (true) {
        cout << "\nCar Dealership System\n";
        cout << "1. Admin\n";
        cout << "2. Employee\n";
        cout << "3. Customer\n";
        cout << "4. Report\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                adminSection(dealership);
                break;
            case 2:
                employeeSection(dealership);
                break;
            case 3:
                customerSection(dealership);
                break;
            case 4:
                reportSection(dealership);
                break;
            case 5:
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
