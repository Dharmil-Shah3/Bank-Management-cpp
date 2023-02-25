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


// ========================= enum BANK_USER_ROLES CLASS =========================
/**
 * @enum BANK_USER_ROLES
 * @brief stores roles of users of system.
 * It stores different roles of users for bank management system.
 * Roles includes Admin, Staff and Account-holder.
 */
enum class BANK_USER_ROLES{
    ADMIN, STAFF, ACCOUNT_HOLDER
};

// ========================= UTIL CLASS =========================
class Util{
private:
    static std::string filename;
    static std::string logFilename;

public:
    virtual void pure_virtual_function_to_prevent_object_making_of_this_class() = 0;

    // Static Constructor
    static class StaticConstructor{
        public:
        StaticConstructor(){
            filename = "bankdetails.json";
            logFilename = "banklogs.json";
        }
    } _staticConstructor;

    /**
     * @brief returns data file name
     * @return name/path of the data/json file
     */
    static std::string getFileName();

    /**
     * @brief returns log file name
     * @return name/path of the log json file
     */
    static std::string getLogFileName();

    /**
     * @brief static function
     * @fn scans the number from the input stream and if invalid input found then it display error message and asks for input until valid input not found
     * @tparam &input Reference of a variable in which we will store the input value from user.
     * @param &message Message to be displayed
     */
    template <typename T>
    static void scanNumber(T &input, const std::string &message);

    /**
     * @brief trims string
     * @param str Leading and Trailing white spaces will be removed from str
     */
    static void trim(std::string& str);

    //=================== Log Functions ========================
    static void writeWithdrawDepositeLog(std::string &type, const long int &bankAccountId, const long int &amount, const std::string &staffId);

    //=================== input validation functions using RegEx ========================
    /**
     * @brief isNameValid static function
     * @param name The value to validate
     * @return true if the given value is valid according to regex
     */
    static bool isNameValid(std::string &name);
    /**
     * @brief isPasswordValid static function
     * @param password The value to validate
     * @return true if the given value is valid according to regex
     */
    static bool isPasswordValid(std::string &password);
    /**
     * @brief isEmailValid static function
     * @param email The value to validate
     * @return true if the given value is valid according to regex
     */
    static bool isEmailValid(std::string &email);
    /**
     * @brief isMobileValid static function
     * @param mobile The value to validate
     * @return true if the given value is valid according to regex
     */
    static bool isMobileValid(std::string &mobile);
    /**
     * @brief isAddressValid static function
     * @param address The value to validate
     * @return true if the given value is valid according to regex
     */
    static bool isAddressValid(std::string &address);

    //=================== generalized read function to read data from JSON file ===================
    /**
     * @brief reads data from json file
     * @param key1 to get specified object. Default value is ""
     * @param key2 to find object from key object. Default value is ""
     * @return json object with specified key & subkey. Returns whole data if no key is specified.
     */
    static nlohmann::json readData(const std::string &key1, const std::string &key2);

    /**
     * @brief reads logs from json file
     * @param key1 to get specified object. Default value is ""
     * @param key2 to get object from key object. Default value is ""
     * @param key3 to get sub-object of sub-object of object. Default value is ""
     * @param key4 to get sub-object of sub-object of sub-object of object. Default value is ""
     * @return json object with specified key & subkey. Returns whole data if no key is specified.
     */
    static nlohmann::json readLogs(const unsigned short &key1, const unsigned short &key2, const unsigned short &key3, const unsigned long int &key4);

    // generalized update function that writes the new_data into the file.
    /**
     * @brief updateData
     * @param data
     */
    static void updateData(const nlohmann::json &data);

    template<typename T>
    /**
     * @brief updateData
     * @param data
     * @param dataToUpdate
     * @tparam newValue
     */
    static void updateData(const nlohmann::json &data, nlohmann::json &dataToUpdate, const T& newValue);

    /**
     * @brief handles login operation.
     * It handles the login operation of the system.
     * @param role is the member of enum BANK_USER_ROLES that defines login role/option
     */
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
    // static methods
    /**
     * @brief login static method
     * @fn validates id password of staff from Json file,
     * and returns pointer to a Staff object if credentials are correct,
     * else returns NULL.
     * Returns type is pointer to an object because incase of invalid login credentials, we are returning NULL.
     * @param id of staff to check
     * @param password of staff to check
     * @return Pointer of a new Staff object OR NULL
     */
    static Staff* login(std::string id, std::string password);

    // constructor & destructor
    Staff(std::string id);
    virtual ~Staff();

    // util methods
    virtual void displayPanel();
    void displayAccountDetails();
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
    std::string selectDesignation(); // to provide designation options for input to addStaff() and updateStaffDetails().
    void searchStaff();     // gives options to search staff by id and name and handles that operation.
public:
    // constructor & destructor
    Admin(std::string id);
    ~Admin();

    // static methods
    static Admin* login(std::string userid, std::string password);
    static bool isAdmin(std::string& id);
    static void test(int limit);

    // util methods
    void displayPanel();
    void searchStaffDetailsById(std::string);
    void searchStaffDetailsByName(std::string);
    void updateStaffDetails(std::string);
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
    // ====== static methods ======
    static long int getLastAccountNumber();
    static void displayAccountDetails(const long int &accountNumber);
    static Account createNewAccount(const long int &accountHolderId, const long int &balance, const std::string &type);
    static void removeAccount(const long int &accountNumber, const std::string &staffId);

    // ====== constructors ======
    Account(long int accountNumber);

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
    ~AccountHolder();

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
