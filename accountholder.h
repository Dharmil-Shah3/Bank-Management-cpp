#ifndef ACCOUNTHOLDER_H
#define ACCOUNTHOLDER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "utils.h"
#include "bankerror.h"

/** ******************************************************
 * @class AccountHolder
 * @brief Class represents the Account holder of the bank.
 ********************************************************/
class AccountHolder
{
private:
    /** *************************************************************************
     * @brief isUserValid tells if given userid and password is valid or not.
     * Main objective of it, is to prevent unauthorised access to the class.
     * All the public non static methods use this flag,
     * to determine wether current object is valid or not.
     * During construction, If user found in the database and password is valid,
     * then it will be set true, else this will set to false.
     ****************************************************************************/
    bool isUserValid;
    std::string id;
    std::string name;
    std::string mobile;
    std::string address;
    std::vector<std::string> bankAccounts;

public:
    // ---------- CONSTRUCTOR & DESTRUCTOR ----------
    AccountHolder(const std::string &accountHolderId, const std::string &password);

    // ---------- STATIC METHODS ----------
    static AccountHolder* login(const std::string &id, const std::string &password);
    static std::string createAccountHolder(const std::string &name,
                                           const std::string &mobile,
                                           const std::string &address,
                                           const std::string &password);

    // ---------- OTHER METHODS ----------
    bool isValid();
    void displayPanel();
};

#endif // ACCOUNTHOLDER_H
