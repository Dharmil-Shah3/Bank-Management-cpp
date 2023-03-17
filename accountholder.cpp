#include "accountholder.h"

using namespace std;
using namespace utils;
using namespace bankerror;

using json = nlohmann::json;

// ------- CONSTRUCTOR -------
AccountHolder::AccountHolder(const string &accountHolderId, const string &password)
{
    try {
        json accountHolder = readData("account_holder", accountHolderId);

        // account holder found and password is valid
        if(!accountHolder.empty() && password == accountHolder["password"]){
            this->id = accountHolderId;
            this->name = accountHolder["name"];
            this->mobile = accountHolder["mobile"];
            this->address = accountHolder["address"];
            // storing bank account ids in vector
            for(size_t i=0; i<accountHolder["bank_accounts"].size(); i++){
                bankAccounts.push_back(accountHolder["bank_accounts"][i]);
            }
            isUserValid = true;
        } else {
            isUserValid = false;
        }
    } catch (const exception &error) {
        displayCustomErrorMessage( __PRETTY_FUNCTION__, __FILE__, error.what());
        isUserValid = false;
    } catch (...) {
        displayCustomErrorMessage( __PRETTY_FUNCTION__, __FILE__);
        isUserValid = false;
    }
}

// ------- STATIC METHODS -------
AccountHolder * AccountHolder::login(const string &id, const string &password){
    try {
        unique_ptr<AccountHolder> accountHolder(new AccountHolder(id, password));

        if(accountHolder->isValid()){ // userid and password is valid
            return accountHolder.release();
        } else {
            throw INVALID_USER;
        }
    }
    catch (const ERROR_USER &error) {
        cout << "\n ERROR: " << errmsg::INVALID_USER << endl;
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    return NULL;
}

string AccountHolder::createAccountHolder(const std::string &name,
                                          const std::string &mobile,
                                          const std::string &address,
                                          const std::string &password)
{
    try {
        json data = readData();
        json newAccountHolder = {
            {"name", name},
            {"mobile", mobile},
            {"address", address},
            {"password", password}
        };

        long int accountHolderId = data["last_account_holder_number"];
        accountHolderId++;
        data["account_holder"].push_back(json::object_t::value_type(to_string(accountHolderId), newAccountHolder));
        updateData(data);

        return to_string(accountHolderId); // return the id of new account holder
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    return NULL; // if account is not created
}

// ------- OTHER METHODS -------
bool AccountHolder::isValid(){ return  this->isUserValid; }

void AccountHolder::displayPanel(){
    if(!isUserValid) throw INVALID_USER;

    try {
        short choice = 1;
        system("clear");
        while(choice != 0){
            cout << "\n ----------- ADMIN PANEL -----------\n" << endl
                 << " 1) Display Your Details " << endl
                 << " 0) Logout" << endl;

            scanNumber(choice, " => Enter Choice: ");

            switch (choice) {
                case 0: system("clear");
                    return;
                default:
                    cout << "\n Invalid choice !" << endl;
            }
        }

    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
}
// =================================== END ACCOUNT_HOLDER ======================================
