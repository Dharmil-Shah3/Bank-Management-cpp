#ifndef STAFF_H
#define STAFF_H

#include <iostream>
#include "utils.h"
#include "bankerror.h"
#include "account.h"
#include "accountholder.h"

class Staff {
protected:
    /** *****************************************************************************
     * @brief isUserValid tells if given staff id is valid or not.
     * Main objective of it, is to prevent unauthorised access to class.
     * All the methods of Staff and Admin class will use this flag,
     * to determine wether current object of staff is valid or not.
     * During construction, If staff found in the database, then it will be set true.
     * else this will set to false.
    ********************************************************************************/
    bool isUserValid;
    std::string id;
    std::string name;
    std::string address;
    std::string designation;
    std::string mobile;
    std::string email;
    long salary;
    long branch_id;

public:
    // ---------- CONSTRUCTOR & DESTRUCTOR ----------
    Staff(const std::string &id, const std::string &password);

    virtual ~Staff(){ /*std::cout << "~Staff()";*/ }

    // ---------- STATIC METHODS ----------
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

    // ---------- OTHER METHODS ----------
    bool isValid();

    virtual void displayPanel();
    void displayStaffDetails();
    void updateAccountDetails();

    std::string createAccountHolder();
    void createBankAccount();
    void removeBankAccount();
    void displayBankAccountDetails();
    void displayAccountHolderDetails();
    void withdraw();
    void deposit();
};

#endif // STAFF_H
