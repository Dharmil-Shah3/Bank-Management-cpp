#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "utils.h"

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
    static long int createNewAccount(const long int &accountHolderId, const long int &balance, const std::string &type);
    static void removeAccount(const long int &accountNumber, const std::string &staffId);

    // ====== getter methods ======
    long int getAccountNumber();
    long int getBalance();
    long int getAccountHolderId();
    std::string getType();

    // ====== util methods ======
    void withdraw(const long int &amount, const std::string &staffId="");
    void deposit(const long int &amount, const std::string &staffId="");
    void displayAccountDetails();
};

#endif // ACCOUNT_H
