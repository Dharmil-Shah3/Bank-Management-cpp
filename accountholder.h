#ifndef ACCOUNTHOLDER_H
#define ACCOUNTHOLDER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "utils.h"

class AccountHolder
{
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
    static long int createAccountHolder(const std::string &name, const std::string &mobile, const std::string &address);

    // getter methods
    long int getId();
    std::string getName();
    std::string getAddress();
    std::string getMobile();

    // util methods
    void displayAccountHolderPanel();
};

#endif // ACCOUNTHOLDER_H
