#include "accountholder.h"

using namespace std;
using namespace utils;
using namespace bankerror;

using json = nlohmann::json;

// ------- CONSTRUCTOR -------
AccountHolder::AccountHolder(const long int &accountHolderId)
{
    try {
        json accountHolder = readData("account_holder", to_string(accountHolderId));
        if(accountHolder.empty()){ // if account holder not found
            throw ERROR_ACCOUNT_HOLDER::USER_NOT_FOUND;
        } else {
            // initializing data from json file
            this->id = accountHolderId;
            this->name = accountHolder["name"];
            this->mobile = accountHolder["mobile"];
            this->address = accountHolder["address"];
            for(size_t i=0; i<accountHolder["bank_accounts"].size(); i++){ // bank account ids
                bankAccounts.push_back(accountHolder["bank_accounts"][i]);
            }
        }
    } catch (const ERROR_ACCOUNT_HOLDER &error) {
        /// @todo handle it
        throw error;
    } catch (const std::exception &error) {
        cout << "\n ERROR: " << error.what() << " in AccountHolder::AccountHolder()" << endl;
    }
}

// ------- STATIC METHODS -------
long int AccountHolder::createAccountHolder(const std::string &name, const std::string &mobile, const std::string &address)
{
    json data = readData();
    json newAccountHolder = {
        {"name", name},
        {"mobile", mobile},
        {"address", address}
    };

    try {
        long int accountHolderId = data["last_account_holder_number"];
        accountHolderId++;
        data["account_holder"].push_back(json::object_t::value_type(to_string(accountHolderId), newAccountHolder));
        updateData(data);
        return accountHolderId; // return the id of new account holder
    } catch (const json::exception &error) {
        cout << "ERROR: " << error.what() << " in AccountHolder::createAccountHolder()" << endl;
    } catch (const std::exception &error) {
        cout << "ERROR: " << error.what() << " in AccountHolder::createAccountHolder()" << endl;
    }
    return 0; // if account is not created
}

// ------- GETTER METHODS -------
long int AccountHolder::getId(){ return this->id; }
// =================================== END ACCOUNT_HOLDER ======================================
