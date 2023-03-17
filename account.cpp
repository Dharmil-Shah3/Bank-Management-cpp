#include "account.h"

using namespace std;
using namespace utils;
using namespace bankerror;

using json = nlohmann::json;

// ------- CONSTRUCTOR -------
Account::Account(const unsigned long int &accountNumber)
{
    try {
        json data = readData("account", to_string(accountNumber));
        if(!data.empty()){ // account found
            this->accountNumber = accountNumber;
            this->balance = data["balance"];
            this->accountHolderId = data["account_holder_id"];
            this->type = data["type"];
        } else { // account NOT found
            throw BANK_ACC_NOT_FOUND;
        }
    } catch (const ERROR_BANK_ACCOUNT &error) {
        throw error;
    } catch (const exception &error) {
        throw error; // rethrowing error to caller function
    }
}

// ------- STATIC METHODS -------
void Account::displayAccountDetails(const unsigned long int &accountNumber)
{
    json data = readData("account", to_string(accountNumber));
    try {
        if (data.empty()){ // if account not found
            throw BANK_ACC_NOT_FOUND;
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
        throw error;
    } catch (const exception &error) {
        throw error;
    }
}

long int Account::createNewAccount(const string &accountHolderId,
                                   const unsigned long int &balance,
                                   const string &type)
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
        if(! data["account_holder"].contains(accountHolderId)){
            throw USER_NOT_FOUND;
        }

        // getting count of accounts to get new account id
        accountNumber = data["account"].size()+1;

        // adding new object in the 'account' object
        data["account"] += json::object_t::value_type(to_string(accountNumber), newAccount);

        // adding bank account numbre into account holder's object for reference
        data["account_holder"][accountHolderId]["bank_accounts"].push_back(to_string(accountNumber));

        // writing data to json file
        updateData(data);

        return accountNumber;
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        throw error;
    } catch (const ERROR_USER &error) {
        throw error;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    return 0;
}

void Account::removeAccount(const unsigned long int &accountNumber, const string &staffId)
{
    try {
        json data = readData();
        if(data["account"].contains(to_string(accountNumber))) // if account found
        {
            Account::displayAccountDetails(accountNumber);
            cout << "\n ===== ARE YOU SURE YOU WANT TO PERMENETALY DELETE THIS BANK ACCOUNT? =====" << endl;
            cout << "\n 0) NO"
                    "\n 1) YES";
            unsigned short choice;

            scanNumber(choice, "\n Enter choice: ");
            string password;

            switch (choice) {
                case 0: cout << "\n => Opration Canceled..." << endl;
                    return;
                case 1:
                    // password validation of staff, before removing account
                    cout << "\n Enter your password: ";
                    cin >> password;
                    cin.clear();

                    if (password == data["staff"][staffId]["password"]){
                        // getting account object, to delete bank acc no. from that object
                        string accountHolderId = to_string(data["account"][to_string(accountNumber)]["account_holder_id"]);

                        // removing object from json
                        short removeCount = data["account"].erase(to_string(accountNumber));

                        if (removeCount == 1){ // 1 element is removed
                            // remove bank account number from account holder's object
                            for(size_t i=0; i< data["account_holder"][accountHolderId]["bank_accounts"].size() ; i++){
                                if(to_string(accountNumber) == data["account_holder"][accountHolderId]["bank_accounts"][i]){
                                    data["account_holder"][accountHolderId]["bank_accounts"].erase(i);
                                    break;
                                }
                            }
                            updateData(data);
                            cout << "\n => Account is REMOVED Successfully !" << endl;

                        }
                        else
                            throw "BANK ACCOUNT IS NOT REMOVED CORRECTLY, VERIFY AND TRY AGAIN...";
                    }
                    else {
                        throw INVALID_PASSWORD;
                    }
                    break;
                default: cout << "\n => Please enter 0 or 1" << endl;
            }
        }
        else { // if account not found
            throw BANK_ACC_NOT_FOUND;
        }
    } catch (const ERROR_BANK_ACCOUNT &error) {
        throw error;
    } catch (const ERROR_USER &error) {
        throw error;
    } catch (const char* error) {
        throw error;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
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
    cout << "  Account Number : " << accountNumber << endl;
    cout << "  Account Holder : " << accountHolderId << endl;
    cout << "  Balance        : Rs." << balance << endl;
    cout << "  Account Type   : " << type << endl;
}

void Account::withdraw(const unsigned long int &amount, const std::string &staffId)
{
    try {
        if(this->balance < amount){ //  insufficient balance
            cout << "\n => Available Balance is " << this->balance << endl;
            throw INSUFFICIENT_BALANCE;
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
        cout << "\n => Rs." << amount << " is Debited from account-number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (const ERROR_BANK_ACCOUNT &error){
        throw error;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

void Account::deposit(const unsigned long int &amount, const std::string &staffId)
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
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}
// ======================================= END ACCOUNT ======================================
