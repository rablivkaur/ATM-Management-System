
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int MAX_ACCOUNTS = 100;

// ==========================================
// BANK ACCOUNT CLASS
// ==========================================

class BankAccount
{
private:
    int accountNumber;
    string customerName;
    int pin;
    double balance;

public:

    // Default Constructor
    BankAccount()
    {
        accountNumber = 0;
        customerName = "";
        pin = 0;
        balance = 0;
    }

    // Parameterized Constructor
    BankAccount(int accNo, string name, int p, double bal)
    {
        accountNumber = accNo;
        customerName = name;
        pin = p;
        balance = bal;
    }

    // Set Account Details
    void setAccountDetails(int accNo, string name, int p, double bal)
    {
        accountNumber = accNo;
        customerName = name;
        pin = p;
        balance = bal;
    }

    // Verify PIN
    bool verifyPIN(int enteredPIN)
    {
        return pin == enteredPIN;
    }

    // Get Account Number
    int getAccountNumber()
    {
        return accountNumber;
    }

    // Display Account Details
    void displayAccountDetails()
    {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nCustomer Name: " << customerName;
    }

    // Check Balance
    void checkBalance()
    {
        cout << "\nCurrent Balance: Rs. " << balance << endl;
    }

    // Record Transaction
    void recordTransaction(string type, double amount)
    {
        ofstream file("transactions.txt", ios::app);

        if (!file)
        {
            cout << "\nError opening transaction file!";
            return;
        }

        file << accountNumber << " "
             << type << " "
             << amount << " "
             << balance << endl;

        file.close();
    }

    // Deposit Money
    void deposit(double amount)
    {
        if (amount <= 0)
        {
            cout << "\nInvalid deposit amount!";
        }
        else
        {
            balance += amount;

            recordTransaction("Deposit", amount);

            cout << "\nAmount deposited successfully!";
            cout << "\nNew Balance: Rs. " << balance;
        }
    }

    // Withdraw Money
    void withdraw(double amount)
    {
        if (amount <= 0)
        {
            cout << "\nInvalid withdrawal amount!";
        }
        else if (amount > balance)
        {
            cout << "\nInsufficient balance!";
        }
        else
        {
            balance -= amount;

            recordTransaction("Withdrawal", amount);

            cout << "\nPlease collect your cash.";
            cout << "\nRemaining Balance: Rs. " << balance;
        }
    }

    // Save Account Information
    void saveToFile(ofstream &file)
    {
        file << accountNumber << " "
             << customerName << " "
             << pin << " "
             << balance << endl;
    }

    // Load Account Information
    bool loadFromFile(ifstream &file)
    {
        if (file >> accountNumber
                 >> customerName
                 >> pin
                 >> balance)
        {
            return true;
        }

        return false;
    }
};

// ==========================================
// SAVE ALL ACCOUNTS
// ==========================================

void saveAllAccounts(BankAccount accounts[], int totalAccounts)
{
    ofstream file("accounts.txt");

    if (!file)
    {
        cout << "\nError opening account file!";
        return;
    }

    for (int i = 0; i < totalAccounts; i++)
    {
        accounts[i].saveToFile(file);
    }

    file.close();
}

// ==========================================
// LOAD ALL ACCOUNTS
// ==========================================

int loadAllAccounts(BankAccount accounts[], int maxAccounts)
{
    ifstream file("accounts.txt");

    if (!file)
    {
        return 0;
    }

    int count = 0;

    while (count < maxAccounts &&
           accounts[count].loadFromFile(file))
    {
        count++;
    }

    file.close();

    return count;
}

// ==========================================
// VIEW TRANSACTION HISTORY
// ==========================================

void viewTransactionHistory(int accountNumber)
{
    ifstream file("transactions.txt");

    if (!file)
    {
        cout << "\nNo transaction history available.";
        return;
    }

    int accNo;
    string type;
    double amount;
    double balance;
    bool found = false;

    cout << "\n========== TRANSACTION HISTORY ==========";

    while (file >> accNo >> type >> amount >> balance)
    {
        if (accNo == accountNumber)
        {
            found = true;

            cout << "\nTransaction Type: " << type;
            cout << "\nAmount: Rs. " << amount;
            cout << "\nBalance After Transaction: Rs. " << balance;
            cout << "\n-----------------------------------------";
        }
    }

    if (!found)
    {
        cout << "\nNo transactions found for this account.";
    }

    file.close();
}

// ==========================================
// MAIN FUNCTION
// ==========================================

int main()
{
    BankAccount accounts[MAX_ACCOUNTS];

    int totalAccounts = loadAllAccounts(accounts, MAX_ACCOUNTS);

    // Create default accounts on first run
    if (totalAccounts == 0)
    {
        accounts[0].setAccountDetails(1001, "Aman", 1234, 5000);
        accounts[1].setAccountDetails(1002, "Simran", 5678, 8000);
        accounts[2].setAccountDetails(1003, "Rahul", 4321, 10000);

        totalAccounts = 3;

        saveAllAccounts(accounts, totalAccounts);

        cout << "\nDefault accounts created successfully!";
    }
    else
    {
        cout << "\nAccount records loaded successfully!";
    }

    int mainChoice;

    do
    {
        cout << "\n\n===================================";
        cout << "\n       ATM MANAGEMENT SYSTEM";
        cout << "\n===================================";
        cout << "\n1. Login to Account";
        cout << "\n2. Create New Account";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";

        cin >> mainChoice;

        switch (mainChoice)
        {

            // ==================================
            // LOGIN TO ACCOUNT
            // ==================================

            case 1:
            {
                int enteredAccount;
                int accountIndex = -1;

                cout << "\nEnter Account Number: ";
                cin >> enteredAccount;

                // Search for Account
                for (int i = 0; i < totalAccounts; i++)
                {
                    if (accounts[i].getAccountNumber() == enteredAccount)
                    {
                        accountIndex = i;
                        break;
                    }
                }

                if (accountIndex == -1)
                {
                    cout << "\nAccount not found!";
                    break;
                }

                // PIN Authentication
                int enteredPIN;
                int attempts = 0;
                bool authenticated = false;

                while (attempts < 3)
                {
                    cout << "\nEnter your 4-digit PIN: ";
                    cin >> enteredPIN;

                    if (accounts[accountIndex].verifyPIN(enteredPIN))
                    {
                        authenticated = true;

                        cout << "\nLogin successful!";
                        accounts[accountIndex].displayAccountDetails();

                        break;
                    }
                    else
                    {
                        attempts++;

                        cout << "\nIncorrect PIN!";

                        if (attempts < 3)
                        {
                            cout << "\nAttempts remaining: "
                                 << 3 - attempts;
                        }
                    }
                }

                if (authenticated)
                {
                    int choice;
                    double amount;

                    do
                    {
                        cout << "\n\n========== ATM MENU ==========";
                        cout << "\n1. Check Balance";
                        cout << "\n2. Deposit Money";
                        cout << "\n3. Withdraw Money";
                        cout << "\n4. Account Details";
                        cout << "\n5. Transaction History";
                        cout << "\n6. Logout";
                        cout << "\nEnter your choice: ";

                        cin >> choice;

                        switch (choice)
                        {
                            case 1:
                                accounts[accountIndex].checkBalance();
                                break;

                            case 2:
                                cout << "\nEnter deposit amount: Rs. ";
                                cin >> amount;

                                accounts[accountIndex].deposit(amount);
                                saveAllAccounts(accounts, totalAccounts);
                                break;

                            case 3:
                                cout << "\nEnter withdrawal amount: Rs. ";
                                cin >> amount;

                                accounts[accountIndex].withdraw(amount);
                                saveAllAccounts(accounts, totalAccounts);
                                break;

                            case 4:
                                accounts[accountIndex].displayAccountDetails();
                                break;

                            case 5:
                                viewTransactionHistory(enteredAccount);
                                break;

                            case 6:
                                cout << "\nLogged out successfully!";
                                break;

                            default:
                                cout << "\nInvalid choice! Please try again.";
                        }

                    } while (choice != 6);
                }
                else
                {
                    cout << "\nAccount access blocked for this session.";
                }

                break;
            }

            // ==================================
            // CREATE NEW ACCOUNT
            // ==================================

            case 2:
            {
                if (totalAccounts >= MAX_ACCOUNTS)
                {
                    cout << "\nMaximum account limit reached!";
                    break;
                }

                int newAccountNumber;
                int newPIN;
                string newName;
                double initialDeposit;

                cout << "\nEnter Customer Name (without spaces): ";
                cin >> newName;

                cout << "Create a 4-digit PIN: ";
                cin >> newPIN;

                if (newPIN < 1000 || newPIN > 9999)
                {
                    cout << "\nInvalid PIN! Enter exactly 4 digits.";
                    break;
                }

                cout << "Enter Initial Deposit (minimum Rs. 500): ";
                cin >> initialDeposit;

                if (initialDeposit < 500)
                {
                    cout << "\nMinimum initial deposit is Rs. 500!";
                    break;
                }

                // Generate Unique Account Number
                newAccountNumber = 1001;

                for (int i = 0; i < totalAccounts; i++)
                {
                    if (accounts[i].getAccountNumber() >= newAccountNumber)
                    {
                        newAccountNumber =
                            accounts[i].getAccountNumber() + 1;
                    }
                }

                // Create New Account
                accounts[totalAccounts].setAccountDetails(
                    newAccountNumber,
                    newName,
                    newPIN,
                    initialDeposit
                );

                totalAccounts++;

                cout << "\nAccount created successfully!";
                cout << "\nYour Account Number: " << newAccountNumber;

                // Save Account Records
                saveAllAccounts(accounts, totalAccounts);
                break;
            }

            // ==================================
            // EXIT
            // ==================================

            case 3:
                cout << "\nThank you for using our ATM!";
                break;

            default:
                cout << "\nInvalid choice! Please try again.";
        }

    } while (mainChoice != 3);

    // Save All Account Records Before Exit
    saveAllAccounts(accounts, totalAccounts);

    return 0;
}
