#ifndef BANKMANAGEMENT_H
#define BANKMANAGEMENT_H

#include <iostream>
#include <cstdio>
#include <fstream>          // for file operations
#include <string>
#include <ctime>            // for get system time to write log
#include <regex>            // for pattern matching and validation
#include <bits/stdc++.h>    // for transform() for lowercase and uppercase strings
#include <nlohmann/json.hpp>// for working with json.
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// ========================= enum BANK_USER_ROLES CLASS =========================
/** ************************************************************
 * @enum BANK_USER_ROLES
 * @brief stores roles of users of system.
 * It stores different roles of users for bank management system.
 * It includes Admin, Staff and Account-holder.
 ************************************************************ */
enum BANK_USER_ROLES{ ADMIN, STAFF, ACCOUNT_HOLDER };

/** ******************************************************************
 * @enum BANK_LOG_TYPE
 * @brief stores types of logs in the program.
 * It stores different log types used in the program.
 * It includes withdraw, deposit, account create, account delete etc...
 ******************************************************************* */
enum BANK_LOG_TYPES{ WITHDRAW, DEPOSIT, STAFF_ADD, STAFF_REMOVE, BANK_ACC_ADD, BANK_ACC_REMOVE };

// ========================= UTIL CLASS =========================
class Util{
private:
    /** @brief filename stores the name of the user data json file. */
    const static std::string filename;
    /** @brief logFilename stores the name of the bank log's json file. */
    const static std::string logFilename;

public:
    virtual void pure_virtual_function_to_prevent_object_making_of_this_class() = 0;

    /** ****************************************************************************
     * @brief kbhit(keyboard hit) checks if any key is pressed from keyboard or not.
     * This function is basically used to exit or break from infinite loops.
     * Loop will be executed until user press any key.
     * @returns 1 if any key is pressed, else returns 0.
     * ****************************************************************************/
    static int kbhit(void);

    /** *************************************
     * @brief returns data file(json) name
     * @return name/path of the data/json file
     ************************************* */
    static std::string getFileName();

    /** *************************************
     * @brief returns log file name
     * @return name/path of the log json file
     ************************************* */
    static std::string getLogFileName();

    /** ***********************************************************************************************
     * @brief static method, used to get input numeric values only.
     * Scans the number from the input stream,
     * and if invalid input found then it display error message,
     * and asks for input until valid input not found.
     * @tparam input Reference of a variable in which we will store the input value given by the user.
     * @param message Message to be displayed
     *********************************************************************************************** */
    template <typename T>
    static void scanNumber(T &input, const std::string &message);

    /** ****************************************************************************
     * @brief trims string
     * Trims all leading and trailing spaces and space characters from given string.
     * @param str data from which the trimming will be done.
     **************************************************************************** */
    static void trim(std::string &str);

    //=================== Log Functions ========================
    /** *********************************************************************************************************
     * @brief writes the withdraw and deposit logs in the json file.
     * @param type is the type of log to be stored. Valid values are WITHDRAW & DEPOSIT from enum BANK_LOG_TYPES.
     * @param bankAccountId is the bank account number on which the transaction is done.
     * @param amount is the money of transcation.
     * @param staffId represents the staff who has done this operation.
     * *********************************************************************************************************/
    static void writeWithdrawDepositeLog(const BANK_LOG_TYPES &type,
                                         const long int &bankAccountId,
                                         const long int &amount,
                                         const std::string &staffId);

    //=================== input validation functions using RegEx ========================
    /** ****************************************************************************************
     * @brief validates the given value as the "fullname" of any member of the bank using regex.
     * @param name is the value to validate.
     * @return true if the given value is valid according to regex.
     * ****************************************************************************************/
    static bool isNameValid(std::string &name);
    /** ****************************************************************************************
     * @brief validates the given value as the "password" of any member of the bank using regex.
     * @param password is the value to validate.
     * @return true if the given value is valid according to regex.
     * ****************************************************************************************/
    static bool isPasswordValid(std::string &password);
    /** ***************************************************************
     * @brief validates the given value as the "email" of staff members.
     * @param email The value to validate.
     * @return true if the given value is valid according to regex.
     * ***************************************************************/
    static bool isEmailValid(std::string &email);
    /** *********************************************************************************************
     * @brief validates the given value as the "mobile number" of any member of the bank using regex.
     * @param mobile The value to validate.
     * @return true if the given value is valid according to regex.
     * *********************************************************************************************/
    static bool isMobileValid(std::string &mobile);
    /** ***************************************************************************************
     * @brief validates the given value as the "address" of any member of the bank using regex.
     * @param address The value to validate.
     * @return true if the given value is valid according to regex.
     * ***************************************************************************************/
    static bool isAddressValid(std::string &address);

    //======================== functions to read data from JSON file ========================
    /** ****************************************************************************************
     * @brief reads data from json file.
     * @param key1 to get specified object. Default value is "".
     * @param key2 to find object from key object. Default value is "".
     * @return json object with specified key & subkey. Returns whole data if no key is specified.
     * ****************************************************************************************/
    static nlohmann::json readData(const std::string &key1="", const std::string &key2="");

    /** ****************************************************************************************
     * @brief reads logs from json file
     * @param key1 to get specified object. Default value is ""
     * @param key2 to get object from key object. Default value is ""
     * @param key3 to get sub-object of sub-object of object. Default value is ""
     * @param key4 to get sub-object of sub-object of sub-object of object. Default value is ""
     * @return json object with specified key & subkey. Returns whole data if no key is specified.
     * ****************************************************************************************/
    static nlohmann::json readLogs(const unsigned short &key1=0,
                                   const unsigned short &key2=0,
                                   const unsigned short &key3=0,
                                   const unsigned long int &key4=0);

    // ===================== functions that writes the new_data into the file =====================
    /** **********************************************
     * @brief overwrites the given data into json file.
     * @param json data to be written into json file.
     * **********************************************/
    static void updateData(const nlohmann::json &data);

    /** ************************************************************************
     * @brief updates the existing data with given new value.
     * @param data is whole json data that needs to be written into json file.
     * @param dataToUpdate is a field/part of the data which needs to be updated.
     * @param newValue is the value that needs to be assigned to dataToUpdate.
     * ************************************************************************/
    template<typename T>
    static void updateData(const nlohmann::json &data, nlohmann::json &dataToUpdate, const T& newValue);

    /** **************************************************************************************
     * @brief handles login operations.
     * It handles the login operation of the system.
     * Handles logins of Staff, Admin and Account Holder
     * @param role is the value of type BANK_USER_ROLES(enum) that defines login roles/options.
     * **************************************************************************************/
    static void handleBankLogin(const BANK_USER_ROLES &role);
};


// ========================= STAFF CLASS =========================
class Staff{
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

    void createBankAccount();
    void removeBankAccount();
    void displayBankAccountDetails();
    void withdraw();
    void deposit();
};


// ========================= ADMIN CLASS =========================
class Admin : public Staff{
private:
    std::string selectDesignation(); /// to provide designation options for input to addStaff() and updateStaffDetails().
    void searchStaff();     /// gives options to search staff by id and name and handles that operation.
public:
    // ---------- constructor & destructor ----------
    Admin(const std::string &adminId, const std::string &password);

    // static methods
    static Admin* login(const std::string &userid, const std::string &password);
    static bool isAdmin(const std::string &id);

    // util methods
    void displayPanel();
    void searchStaffDetailsById(const std::string &staffId);
    void searchStaffDetailsByName(std::string &staffName);
    void updateStaffDetails(const std::string &staffId);
    void addStaff();
    void removeStaff();
    void displayLogsByMonth();
};


// ========================= ACCOUNT CLASS =========================
class Account{
private:
    // ====== normal members ======
    long int accountNumber;
    long int accountHolderId;
    long int balance;
    std::string type;

public:
    // ====== constructors ======
    Account(long int accountNumber);

    // ====== static methods ======
    static long int getLastAccountNumber();
    static void displayAccountDetails(const long int &accountNumber);
    static Account createNewAccount(const long int &accountHolderId, const long int &balance, const std::string &type);
    static void removeAccount(const long int &accountNumber, const std::string &staffId);

    // ====== getter methods ======
    long int getAccountNumber();
    long int getBalance();
    long int getAccountHolderId();
    std::string getType();

    // ====== util methods ======
    void withdraw(const long int &amount, const std::string &staffId);
    void deposit(const long int &amount, const std::string &staffId);
    void displayAccountDetails();
};


// ========================= ACCOUNT_HOLDER CLASS =========================
class AccountHolder{
private:
    // data members
    long int id;
    std::string name;
    std::string mobile;
    std::string address;

public:
    // constructor & destructor
    AccountHolder(const long int &accountHolderId);

    // static methods
    static long int getLastAccountHolderNumber();
    static void createAccountHolder(const std::string &name, const std::string &mobile, const std::string &address);

    // getter methods
    long int getId();
    std::string getName();
    std::string getAddress();
    std::string getMobile();

    // setter methods
    void setName(const std::string &name);
    void setAddress(const std::string &address);
    void setMobile(const std::string &mobile);

    // util methods
    void displayAccountHolderPanel();
};

#endif // BANKMANAGEMENT_H
