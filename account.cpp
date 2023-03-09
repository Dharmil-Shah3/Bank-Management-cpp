#include "account.h"

using namespace std;
using namespace utils;
using namespace bankerror;

using json = nlohmann::json;

// ------- CONSTRUCTOR -------
Account::Account(long int accountNumber)
{
    try {
        json data = readData("account", to_string(accountNumber));
        if(!data.empty()){
            // if account found, initializing data from json file
            this->accountNumber = accountNumber;
            this->balance = data["balance"];
            this->accountHolderId = data["account_holder_id"];
            this->type = data["type"] == "s"? 's': 'c';
        } else {
            // if account NOT found
            throw ERROR_BANK_ACCOUNT::ACCOUNT_NOT_FOUND;
        }
    } catch (const ERROR_BANK_ACCOUNT &error) {
        throw error;
    } catch (const std::exception &error) {
        throw error; // rethrowing error to caller function
    }
}

// ------- STATIC METHODS -------
void Account::displayAccountDetails(const long &accountNumber)
{
    json data = readData("account", to_string(accountNumber));
    try {
        if (data.empty()){ // if account not found
            throw ERROR_BANK_ACCOUNT::ACCOUNT_NOT_FOUND;
        }
        else {
            string type = data["type"];
            cout << endl << "\n =============== ACCOUNT DETAILS ===============\n" << endl;
            cout << " Account Number        : " << accountNumber << endl;
            cout << " Account Holder Number : " << data["account_holder_id"] << endl;
            cout << " Balance               : Rs." << data["balance"] << endl;
            cout << " Account Type          : " << trim(type, "\"") << endl;
        }
    } catch (const ERROR_BANK_ACCOUNT &error) {
        /// @todo handle it everywhere else
        throw error;
    } catch (const std::exception &error) {
        throw error;
    }
}

long int Account::createNewAccount(const long int &accountHolderId, const long int &balance, const string &type)
{
    long int accountNumber;
    try {
        // creating new json Account object
        json newAccount = {
            {"account_holder_id", accountHolderId},
            {"balance", balance},
            {"type", type}
        };

        json data = readData();

        // account holder not found
        if(! data["account_holder"].contains(to_string(accountHolderId))){
            throw ERROR_ACCOUNT_HOLDER::USER_NOT_FOUND;
        }

        // getting count of accounts to get new account id
        accountNumber = data["account"].size()+1;

        // adding new object in the 'account' object
        data["account"] += json::object_t::value_type(to_string(accountNumber), newAccount);

        // writing data to json file
        updateData(data);

        return accountNumber;
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        throw error;
    }
    catch (const ERROR_ACCOUNT_HOLDER &error) {
        throw error;
    }
    catch (const json::exception &error) {
        cout << "\n ERROR: " << error.what() << " in Account::createNewAccount()" << endl;
    }
    return 0;
}

void Account::removeAccount(const long &accountNumber, const string &staffId)
{
    try {
        json data = readData();
        if(data["account"].contains(to_string(accountNumber))){ // if account found
            Account::displayAccountDetails(accountNumber);
            cout << "\n ===== ARE YOU SURE YOU WANT TO PERMENETALY DELETE THIS BANK ACCOUNT? =====" << endl;
            cout << "\n       0) NO"
                    "\n       1) YES";
            short choice;

            scanNumber(choice, "\n Enter choice: ");
            string password;

            switch (choice) {
                case 0: cout << "\n => Opration Canceled..." << endl;
                    return;
                case 1:
                    // password validation of staff, before removing account
                    cout << "\n Enter your password: ";
                    cin >> password;
                    if (password == data["staff"][staffId]["password"]){
                        // removing object from json
                        short removeCount = data["account"].erase(to_string(accountNumber));
                        if (removeCount == 1){ // 1 element is removed
                            updateData(data);
                            cout << "\n => Account is REMOVED Successfully !" << endl;
                        }
                        else
                            throw "BANK ACCOUNT IS NOT REMOVED CORRECTLY, VERIFY AND TRY AGAIN...";
                    }
                    else {
                        throw ERROR_STAFF::INVALID_PASSWORD;
                    }
                    break;
                default: cout << "\n => Please enter 0 or 1" << endl;
            }
        }
        else { // if account not found
            throw ERROR_BANK_ACCOUNT::ACCOUNT_NOT_FOUND;
        }
    } catch (const ERROR_BANK_ACCOUNT &error) {
        throw error;
    } catch (const ERROR_STAFF &error) {
        throw error;
    } catch (const char* error) {
        throw error;
    } catch (const json::exception &error) {
        cout << "\n ERROR (JSON): " << error.what()  << " in Account::removeAccount()" << endl;
    } catch (const std::exception &error) {
        cout << "\n ERROR: " << error.what()  << " in Account::removeAccount()" << endl;
    }
}

long int Account::getLastAccountNumber(){ return readData("last_account_number"); }

// ------- GETTER METHODS -------
long int Account::getAccountNumber(){ return this->accountNumber; }
long int Account::getBalance(){ return this->balance; }
long int Account::getAccountHolderId(){ return this->accountHolderId; }
string Account::getType(){ return this->type; }

// ------- Util METHODS -------
void Account::displayAccountDetails()
{
    cout << endl << "\n =============== ACCOUNT DETAILS ===============\n" << endl;
    cout << "\tAccount Number : " << accountNumber << endl;
    cout << "\tAccount Holder : " << accountHolderId << endl;
    cout << "\tBalance        : Rs." << balance << endl;
    cout << "\tAccount Type   : " << type << endl;
}

void Account::withdraw(const long int &amount, const std::string &staffId)
{
    try {
        if(this->balance < amount){ //  insufficient balance
            cout << "\n => Available Balance is " << this->balance << endl;
            throw ERROR_BANK_ACCOUNT::INSUFFICIENT_BALANCE;
        }

        // reading data from json file
        json data = readData();
        data["account"][to_string(accountNumber)]["balance"] = (this->balance - amount);

        // Wrtiting updated data into json file
        updateData(data);

        // writing log of the transaction
        string type = "withdraw";
        writeWithdrawDepositeLog(BANK_LOG_TYPES::WITHDRAW, this->accountNumber, amount, staffId);

        this->balance -= amount;
        cout << "\n => Rs." << amount << " is Debited from account_number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (const ERROR_BANK_ACCOUNT &error){
        throw error;
    } catch (const json::exception &error){
        cout << "\n ERROR (JSON): " << error.what() << " in Account::withdraw()" << endl;
    } catch (const std::exception &error) {
        cout << "\n ERROR: " << error.what() << " in Account::withdraw()" << endl;
    }
}

void Account::deposit(const long int &amount, const std::string &staffId)
{
    try {
        // reading data from json file
        json data = readData();
        data["account"][to_string(accountNumber)]["balance"] = this->balance + amount;

        // Wrtiting updated data into json file
        updateData(data);
        writeWithdrawDepositeLog(BANK_LOG_TYPES::DEPOSIT, this->accountNumber, amount, staffId);

        this->balance += amount;
        cout << "\n => Rs." << amount << " is Credited to account_number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (const exception &error){
        cout << "\n ERROR: " << error.what() << " in Account::deposit()" << endl;
    }
}
// ======================================= END ACCOUNT ======================================
