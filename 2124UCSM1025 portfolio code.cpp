#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

// Base class for Investments
class Investment {
protected:
    string name;
    double amount;
    double currentPrice;

public:
    Investment(string n, double amt, double price) : name(n), amount(amt), currentPrice(price) {}

    virtual void display() = 0;
    virtual double calculateValue() = 0;
    virtual string getType() = 0;
    virtual ~Investment() {}

    virtual void saveToFile(ofstream& file) {
        file << name << " " << amount << " " << currentPrice << " ";
    }

    virtual void loadFromFile(ifstream& file) {
        file >> name >> amount >> currentPrice;
    }
};

// Stock class
class Stock : public Investment {
    int shares;

public:
    Stock(string n, int sh, double price) : Investment(n, sh * price, price), shares(sh) {}

    void display() override {
        cout << "Stock: " << name << " | Shares: " << shares << " | Current Price: $" << currentPrice << endl;
    }

    double calculateValue() override {
        return shares * currentPrice;
    }

    string getType() override {
        return "Stock";
    }

    void saveToFile(ofstream& file) override {
        file << "Stock ";
        Investment::saveToFile(file);
        file << shares << endl;
    }

    void loadFromFile(ifstream& file) override {
        Investment::loadFromFile(file);
        file >> shares;
    }
};

// Bond class
class Bond : public Investment {
    double interestRate;

public:
    Bond(string n, double amt, double rate) : Investment(n, amt, amt), interestRate(rate) {}

    void display() override {
        cout << "Bond: " << name << " | Amount: $" << amount << " | Interest Rate: " << interestRate << "%" << endl;
    }

    double calculateValue() override {
        return amount * (1 + (interestRate / 100));
    }

    string getType() override {
        return "Bond";
    }

    void saveToFile(ofstream& file) override {
        file << "Bond ";
        Investment::saveToFile(file);
        file << interestRate << endl;
    }

    void loadFromFile(ifstream& file) override {
        Investment::loadFromFile(file);
        file >> interestRate;
    }
};

// Mutual Fund class
class MutualFund : public Investment {
    double nav;

public:
    MutualFund(string n, double amt, double nav) : Investment(n, amt, nav), nav(nav) {}

    void display() override {
        cout << "Mutual Fund: " << name << " | Amount: $" << amount << " | NAV: $" << nav << endl;
    }

    double calculateValue() override {
        return amount;
    }

    string getType() override {
        return "MutualFund";
    }

    void saveToFile(ofstream& file) override {
        file << "MutualFund ";
        Investment::saveToFile(file);
        file << nav << endl;
    }

    void loadFromFile(ifstream& file) override {
        Investment::loadFromFile(file);
        file >> nav;
    }
};

// Cryptocurrency class
class Cryptocurrency : public Investment {
    double units;

public:
    Cryptocurrency(string n, double u, double price) : Investment(n, u * price, price), units(u) {}

    void display() override {
        cout << "Cryptocurrency: " << name << " | Units: " << units << " | Current Price: $" << currentPrice << endl;
    }

    double calculateValue() override {
        return units * currentPrice;
    }

    string getType() override {
        return "Cryptocurrency";
    }

    void saveToFile(ofstream& file) override {
        file << "Cryptocurrency ";
        Investment::saveToFile(file);
        file << units << endl;
    }

    void loadFromFile(ifstream& file) override {
        Investment::loadFromFile(file);
        file >> units;
    }
};

// Portfolio class
class Portfolio {
    vector<Investment*> investments;

public:
    void addInvestment(Investment* inv) {
        investments.push_back(inv);
    }

    void removeInvestment(int index) {
        if (index >= 0 && index < investments.size()) {
            delete investments[index]; // Free memory
            investments.erase(investments.begin() + index);
        }
    }

    void displayPortfolio() {
        if (investments.empty()) {
            cout << "No investments in the portfolio." << endl;
        } else {
            for (size_t i = 0; i < investments.size(); i++) {
                cout << i + 1 << ". ";
                investments[i]->display();
            }
        }
    }

    double calculateTotalValue() {
        double total = 0;
        for (auto& inv : investments) {
            total += inv->calculateValue();
        }
        return total;
    }

    double calculateDiversificationRatio() {
        double stockValue = 0, bondValue = 0, mutualValue = 0, cryptoValue = 0;
        double totalValue = calculateTotalValue();

        for (auto& inv : investments) {
            if (inv->getType() == "Stock") stockValue += inv->calculateValue();
            if (inv->getType() == "Bond") bondValue += inv->calculateValue();
            if (inv->getType() == "MutualFund") mutualValue += inv->calculateValue();
            if (inv->getType() == "Cryptocurrency") cryptoValue += inv->calculateValue();
        }

        cout << "\nDiversification:\n";
        cout << "Stocks: " << (stockValue / totalValue) * 100 << "%" << endl;
        cout << "Bonds: " << (bondValue / totalValue) * 100 << "%" << endl;
        cout << "Mutual Funds: " << (mutualValue / totalValue) * 100 << "%" << endl;
        cout << "Cryptocurrency: " << (cryptoValue / totalValue) * 100 << "%" << endl;
        
        // Simplified diversification ratio calculation
        return sqrt(pow((stockValue / totalValue), 2) + pow((bondValue / totalValue), 2) +
                    pow((mutualValue / totalValue), 2) + pow((cryptoValue / totalValue), 2));
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (auto& inv : investments) {
                inv->saveToFile(file);
            }
            file.close();
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string type;

        while (file >> type) {
            if (type == "Stock") {
                Investment* stock = new Stock("", 0, 0);
                stock->loadFromFile(file);
                addInvestment(stock);
            } else if (type == "Bond") {
                Investment* bond = new Bond("", 0, 0);
                bond->loadFromFile(file);
                addInvestment(bond);
            } else if (type == "MutualFund") {
                Investment* mutual = new MutualFund("", 0, 0);
                mutual->loadFromFile(file);
                addInvestment(mutual);
            } else if (type == "Cryptocurrency") {
                Investment* crypto = new Cryptocurrency("", 0, 0);
                crypto->loadFromFile(file);
                addInvestment(crypto);
            }
        }
        file.close();
    }

    ~Portfolio() {
        for (auto& inv : investments) {
            delete inv; // Free memory
        }
    }
};

// User class to manage user information and portfolio
class User {
    string username;
    string password;
    Portfolio portfolio;

public:
    User(string uname, string pass) : username(uname), password(pass) {}

    string getUsername() {
        return username;
    }

    bool authenticate(string pass) {
        return password == pass;
    }

    void managePortfolio() {
        int choice;
        do {
            cout << "\n--- Portfolio Management ---" << endl;
            cout << "1. Add Investment" << endl;
            cout << "2. Remove Investment" << endl;
            cout << "3. View Portfolio" << endl;
            cout << "4. Save Portfolio" << endl;
            cout << "5. Load Portfolio" << endl;
            cout << "6. Calculate Diversification" << endl;
            cout << "7. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string type, name;
                    double price, amount, units;
                    int shares;
                    cout << "Enter investment type (stock/bond/mutual/crypto): ";
                    cin >> type;
                    cout << "Enter name: ";
                    cin >> name;

                    if (type == "stock") {
                        cout << "Enter shares: ";
                        cin >> shares;
                        cout << "Enter current price per share: ";
                        cin >> price;
                        portfolio.addInvestment(new Stock(name, shares, price));
                    } else if (type == "bond") {
                        cout << "Enter amount: ";
                        cin >> amount;
                        cout << "Enter interest rate: ";
                        cin >> price;
                        portfolio.addInvestment(new Bond(name, amount, price));
                    } else if (type == "mutual") {
                        cout << "Enter amount: ";
                        cin >> amount;
                        cout << "Enter NAV: ";
                        cin >> price;
                        portfolio.addInvestment(new MutualFund(name, amount, price));
                    } else if (type == "crypto") {
                        cout << "Enter units: ";
                        cin >> units;
                        cout << "Enter current price per unit: ";
                        cin >> price;
                        portfolio.addInvestment(new Cryptocurrency(name, units, price));
                    }
                    break;
                }
                case 2: {
                    int index;
                    portfolio.displayPortfolio();
                    cout << "Enter index to remove: ";
                    cin >> index;
                    portfolio.removeInvestment(index - 1);
                    break;
                }
                case 3:
                    portfolio.displayPortfolio();
                    cout << "Total Portfolio Value: $" << portfolio.calculateTotalValue() << endl;
                    break;
                case 4:
                    portfolio.saveToFile("portfolio.txt");
                    cout << "Portfolio saved!" << endl;
                    break;
                case 5:
                    portfolio.loadFromFile("portfolio.txt");
                    cout << "Portfolio loaded!" << endl;
                    break;
                case 6:
                    cout << "Diversification Ratio: " << portfolio.calculateDiversificationRatio() << endl;
                    break;
                case 7:
                    cout << "Exiting portfolio management..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Try again!" << endl;
            }
        } while (choice != 7);
    }
};

// Main system for user registration and login
class PortfolioManagementSystem {
    vector<User> users;

public:
    void registerUser() {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pass;
        users.push_back(User(uname, pass));
        cout << "Registration successful!" << endl;
    }

    void loginUser() {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pass;

        for (auto& user : users) {
            if (user.getUsername() == uname && user.authenticate(pass)) {
                cout << "Login successful!" << endl;
                user.managePortfolio();
                return;
            }
        }
        cout << "Invalid username or password!" << endl;
    }

    void start() {
        int choice;
        do {
            cout << "\n--- Portfolio Management System ---" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loginUser();
                    break;
                case 3:
                    cout << "Exiting system..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Try again!" << endl;
            }
        } while (choice != 3);
    }
};

int main() {
    PortfolioManagementSystem pms;
    pms.start();
    return 0;
}