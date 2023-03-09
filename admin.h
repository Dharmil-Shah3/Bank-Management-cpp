#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <bits/stdc++.h>     // for transform() for lowercase and uppercase strings
#include "utils.h"
#include "staff.h"

class Admin : public Staff {
private:
    std::string selectDesignation(); /// to provide designation options for input to addStaff() and updateStaffDetails().
    void searchStaff(); /// gives options to search staff by id and name and handles that operation.
    void displayWithdrawDepositLogs(const nlohmann::json &logs, const std::string &month, const std::string &day);

public:
    // ---------- CONSTRUCTOR & DESTRUCTOR ----------
    Admin(const std::string &adminId, const std::string &password);

    // static methods
    static Admin* login(const std::string &userid, const std::string &password);
    static bool isAdmin(const std::string &id);
    static void displayStaffDetails(const std::string &staffId);

    // ---------- UTIL METHODS ----------
    void displayPanel();
    void searchStaffDetailsById(const std::string &staffId);
    void searchStaffDetailsByName(std::string &staffName);
    void updateStaffDetails(const std::string &staffId);
    void addStaff();
    void removeStaff();

    // ---------- LOG FUNCTIONS ----------
    void displayLogs();
    void displayWithdrawDepositLogsByDate();
    void displayWithdrawDepositLogsByMonth();
    void displayWithdrawDepositLogsByYear();
};

#endif // ADMIN_H
