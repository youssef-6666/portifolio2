#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <iomanip>
using namespace std;

// Structs
struct Address {
    string city;
    string streetName;
    string buildingDetails;
};

struct Medicine {
    string name;
    double price;
};

struct PersonalInfo {
    string name;
    string passwordHash; // Storing password securely
    int accountNumber;
    int phoneNumber;
    vector<Address> addresses;
};

// Global Variables
vector<PersonalInfo> personalInfo;
unordered_map<int, vector<pair<string, double>>> shoppingCarts;

// Dummy database of medicines
vector<Medicine> medicineDatabase = {
    {"panadol", 5.99},
    {"voltaren", 8.50},
    {"catafast", 6.75},
    {"congestal", 7.25},
    {"omega3", 10.00}
};

// Function Prototypes
void print(const string& text);
void printError(const string& text);
void newAccount();
void login();
void atm(PersonalInfo& info);
void performTransaction(PersonalInfo& info);
void searchAndAddToCart();
void search(const string& medicineName);
int accountNumCreation();
string hashPassword(const string& password);
bool verifyPassword(const string& inputPassword, const string& storedHash);
void pay(PersonalInfo& info);
void displayCart(const vector<pair<string, double>>& cart);

int main() {
    print("Welcome to PharmaGo!\n");
    print("Please select from the following options:\n");
    print("1. New Customer\n");
    print("2. Existing Customer\n");

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
        newAccount();
        break;
    case 2:
        login();
        break;
    default:
        printError("Invalid choice.\n");
    }

    return 0;
}

void newAccount() {
    PersonalInfo info;
    print("Create your account:\n");

    print("Enter your name: ");
    cin.ignore();
    getline(cin, info.name);

    print("Create your password: ");
    string password;
    cin >> password;
    info.passwordHash = hashPassword(password);

    print("Enter your phone number: ");
    cin >> info.phoneNumber;

    Address address;
    print("Enter your address:\n");
    print("City: ");
    cin.ignore();
    getline(cin, address.city);
    print("Street Name: ");
    getline(cin, address.streetName);
    print("Building Details: ");
    getline(cin, address.buildingDetails);
    info.addresses.push_back(address);
    info.accountNumber = info.phoneNumber;
    personalInfo.push_back(info);

    print("Your account has been created successfully!\n");
    login();
}

void login() {
    print("Enter your phone number: ");
    int accountNumber;
    cin >> accountNumber;

    auto it = find_if(personalInfo.begin(), personalInfo.end(), [accountNumber](const PersonalInfo& info) {
        return info.accountNumber == accountNumber;
        });

    if (it == personalInfo.end()) {
        printError("Account not found. Please try again.\n");
        login();
    }
    else {
        print("Enter your password: ");
        string password;
        cin >> password;

        if (verifyPassword(password, it->passwordHash)) {
            print("Login successful!\n");
            atm(*it);
        }
        else {
            printError("Incorrect password. Please try again.\n");
            login();
        }
    }
}

void atm(PersonalInfo& info) {
    while (true) {
        print("\n1. Order Medicine\n");
        print("2. View Cart\n");
        print("3. Checkout\n");
        print("4. Logout\n");

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            searchAndAddToCart();
            break;
        case 2:
            displayCart(shoppingCarts[info.accountNumber]);
            break;
        case 3:
            pay(info);
            break;
        case 4:
            print("Logging out...\n");
            return;
        default:
            printError("Invalid choice. Please try again.\n");
        }
    }
}

void searchAndAddToCart() {
    print("Enter the medicine name: ");
    string medicineName;
    cin >> medicineName;

    search(medicineName);
}

void search(const string& medicineName) {
    // Search the medicine database for the medicine
    auto it = find_if(medicineDatabase.begin(), medicineDatabase.end(),
        [medicineName](const Medicine& med) { return med.name == medicineName; });

    if (it != medicineDatabase.end()) {
        print("The item you want is available.\n");
        print("Added to your cart.\n");
        shoppingCarts[personalInfo.back().accountNumber].push_back(make_pair(it->name, it->price));
    }
    else {
        print("The item you want is not available.\n");
    }
}

void pay(PersonalInfo& info) {
    vector<pair<string, double>>& cart = shoppingCarts[info.accountNumber];
    if (cart.empty()) {
        printError("Your cart is empty.\n");
        return;
    }

    print("Your Cart:\n");
    displayCart(cart);

    // Dummy payment process
    double totalPrice = 0;
    for (const auto& item : cart) {
        totalPrice += item.second;
    }
    print("Total Price: $" + to_string(totalPrice) + "\n");

    // Clear the cart after checkout
    cart.clear();
}

void displayCart(const vector<pair<string, double>>& cart) {
    if (cart.empty()) {
        print("Your cart is empty.\n");
    }
    else {
        print("Items in your cart:\n");
        for (const auto& item : cart) {
            cout << "- " << item.first << " ($" << fixed << setprecision(2) << item.second << ")\n";
        }
    }
}

int accountNumCreation() {
    return personalInfo.empty() ? 1000 : personalInfo.back().accountNumber + 1;
}

string hashPassword(const string& password) {
    // Dummy implementation for demonstration
    // In a real system, use a secure hashing algorithm like bcrypt
    return password; // For simplicity, just returning the password as is
}

bool verifyPassword(const string& inputPassword, const string& storedHash) {
    // Dummy implementation for demonstration
    // In a real system, compare the hashed versions of passwords
    return inputPassword == storedHash;
}

void print(const string& text) {
    cout << "\033[1m\033[34;3m" << text;
    cout << "\033[0m";
}

void printError(const string& text) {
    cout << "\033[1;31m" << text << "\033[0m";
}