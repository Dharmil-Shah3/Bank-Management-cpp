#ifndef STAFF_H
#define STAFF_H

#include <iostream>
#include "utils.h"
#include "account.h"
#include "accountholder.h"

class Staff {
protected:
    std::string id;
    std::string name;
    std::string address;
    std::string designation;
    std::string mobile;
    std::string email;
    long salary;
    long branch_id;

public:
    // ---------- constructor & destructor ----------
    Staff(const std::string &id, const std::string &password);
    virtual ~Staff(){};

    // ---------- static methods ----------
    /** *******************************************************************************************************
     * @brief staff login operation.
     * It validates id password of staff from json file,
     * and returns pointer to a Staff object if credentials are correct, else returns NULL.
     * Returns type is pointer to an object because incase of invalid login credentials, we are returning NULL.
     * @param id of staff to check.
     * @param password of staff to check.
     * @return Pointer of a new Staff object OR NULL.
     ******************************************************************************************************* */
    static Staff* login(const std::string &id, const std::string &password);
    static void displayStaffDetails(const std::string &staffId);

    // ---------- normal methods ----------
    virtual void displayPanel();
    void displayStaffDetails();
    void updateAccountDetails();

    long int createAccountHolder();
    void createBankAccount();
    void removeBankAccount();
    void displayBankAccountDetails();
    void displayAccountHolderDetails();
    void withdraw();
    void deposit();
};

#endif // STAFF_H
