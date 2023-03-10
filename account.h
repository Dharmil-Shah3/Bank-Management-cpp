#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "utils.h"

class Account{
private:
    // ====== normal members ======
    unsigned long int accountNumber;
    unsigned long int accountHolderId;
    unsigned long int balance;
    std::string type;

public:
    // ====== constructors ======
    Account(const unsigned long int &accountNumber);

    // ====== static methods ======
    static long int getLastAccountNumber();
    static void displayAccountDetails(const unsigned long int &accountNumber);
    static long int createNewAccount(const unsigned long int &accountHolderId, const unsigned long int &balance, const std::string &type);
    static void removeAccount(const unsigned long int &accountNumber, const std::string &staffId);

    // ====== getter methods ======
    long int getAccountNumber();
    long int getBalance();
    long int getAccountHolderId();
    std::string getType();

    // ====== util methods ======
    void withdraw(const unsigned long int &amount, const std::string &staffId="");
    void deposit(const unsigned long int &amount, const std::string &staffId="");
    void displayAccountDetails();
};

#endif // ACCOUNT_H
