#include <bits/stdc++.h>
using namespace std;

class Account {
private:
    string name;
    int accountNumber;
    double balance;
    string pin;

public:
    Account(string n, int accNum, double bal, string p) {
        name = n;
        accountNumber = accNum;
        balance = bal;
        pin = p;
    }

    int getAccNumber() { return accountNumber; }
    
    bool verifyPin(string p) {
        return pin == p;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\n[+] Successfully Deposited $" << amount << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "\n[!] Insufficient Balance!" << endl;
        } else if (amount <= 0) {
            cout << "\n[!] Invalid Amount!" << endl;
        } else {
            balance -= amount;
            cout << "\n[-] Successfully Withdrawn $" << amount << endl;
        }
    }

    void showDetails() {
        cout << "\n--- Account Details ---" << endl;
        cout << "Holder Name: " << name << endl;
        cout << "Account No : " << accountNumber << endl;
        cout << "Balance    : $" << balance << endl;
        cout << "-----------------------" << endl;
    }
};

void displayBanner() {
    const char* no = "\033[1;36m";
    const char* reset = "\033[0m";
    cout << no << "Welcome to " << endl;
    cout << R"(
     ____    _    _   _ _  __     _   _ _   _ _     _     ___ _____ __   __
    | __ )  / \  | \ | | |/ /    | \ | | | | | |   | |   |_ _|  ___|\ \ / /
    |  _ \ / _ \ |  \| | ' /     |  \| | | | | |   | |    | || |_    \ V / 
    | |_) / ___ \| |\  | . \     | |\  | |_| | |___| |___ | ||  _|    | |  
    |____/_/   \_\_| \_|_|\_\    |_| \_|\__,_|_____|_____|___|_|      |_|  
    =======================================================================)" << endl;
    cout << "\t\t\t" << "Let us null your account" << reset << endl;
}

int main() {
    vector<Account> accounts;
    int choice;
    int accCounter = 1001;

    while (true) {
        displayBanner();
        cout << "\n1. Create Account" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Check Balance/Details" << endl;
        cout << "5. Exit" << endl;
        cout << "\nSelect Option: ";
        cin >> choice;

        if (choice == 5) break;

        switch (choice) {
            case 1: {
                string name, pin;
                double initialDeposit;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Set 4-Digit PIN: ";
                cin >> pin;
                cout << "Initial Deposit: ";
                cin >> initialDeposit;
                accounts.push_back(Account(name, accCounter++, initialDeposit, pin));
                cout << "\nAccount Created! Your ID is: " << accCounter - 1 << endl;
                break;
            }
            case 2: {
                int id;
                double amt;
                cout << "Enter Account ID: ";
                cin >> id;
                bool found = false;
                for (auto &acc : accounts) {
                    if (acc.getAccNumber() == id) {
                        cout << "Amount to Deposit: ";
                        cin >> amt;
                        acc.deposit(amt);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account Not Found!" << endl;
                break;
            }
            case 3: {
                int id;
                string p;
                double amt;
                cout << "Enter Account ID: ";
                cin >> id;
                bool found = false;
                for (auto &acc : accounts) {
                    if (acc.getAccNumber() == id) {
                        cout << "Enter PIN: ";
                        cin >> p;
                        if (acc.verifyPin(p)) {
                            cout << "Amount to Withdraw: ";
                            cin >> amt;
                            acc.withdraw(amt);
                        } else {
                            cout << "[!] Incorrect PIN!" << endl;
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account Not Found!" << endl;
                break;
            }
            case 4: {
                int id;
                string p;
                cout << "Enter Account ID: ";
                cin >> id;
                bool found = false;
                for (auto &acc : accounts) {
                    if (acc.getAccNumber() == id) {
                        cout << "Enter PIN: ";
                        cin >> p;
                        if (acc.verifyPin(p)) {
                            acc.showDetails();
                        } else {
                            cout << "[!] Incorrect PIN!" << endl;
                        }
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account Not Found!" << endl;
                break;
            }
            default:
                cout << "Invalid Choice!" << endl;
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        system("cls");
    }

    return 0;
}