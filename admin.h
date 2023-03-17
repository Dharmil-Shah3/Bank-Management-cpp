#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <bits/stdc++.h>     // for transform() for lowercase and uppercase strings
#include "utils.h"
#include "bankerror.h"
#include "staff.h"

class Admin : public Staff {
private:
    /** **********************************************************************
     * @brief provides designation options for adding a new staff member.
     * addStaff() uses this functions to get designatioon input for new staff.
     * @return selected designation as string
     * **********************************************************************/
    std::string selectDesignation();

    /** *******************************************************************
     * @brief provides utility to search staff members.
     * It provides the interface/options to search staff using id and name.
     * *******************************************************************/
    void searchStaff();

    /** ************************************************************************
     * @brief displays the withdraw and deposit logs.
     * it will display logs for only one day,
     * because only one(inner most) and common loop is written inside.
     * so the first argument contains json object of a particular date only.
     * date and month are provided as arguments to display it in result table.
     * @param logs is the json object containig logs that is gonna be displayed.
     * @param month is the month of the logs.
     * @param day is the date of the logs.
     * ************************************************************************/
    void displayWithdrawDepositLogs(const nlohmann::json &dateLogs, const std::string &month, const std::string &day);

public:
    // ---------- CONSTRUCTOR & DESTRUCTOR ----------
    /** ******************************************
     * @brief parameterised constructor.
     * @param id is the username/staffid of admin.
     * @param password of admin.
     * ******************************************/
    Admin(const std::string &id, const std::string &password);

    ~Admin(){ /*std::cout << "~Admin()";*/ }


    // ---------- STATIC METHODS ----------
    /** **************************************************************
     * @brief handles the login operation of the admin.
     * @param userid of the admin.
     * @param password of the admin.
     * @return pointer to Admin's object, and NULL if admin not found.
     * **************************************************************/
    static Admin* login(const std::string &userid, const std::string &password);

    /** ******************************************************
     * @brief isAdmin checks if given staffId is admin or not.
     * @param id is the staff id to search for admin.
     * @return true if id is admin, or false.
     * ******************************************************/
    static bool isAdmin(const std::string &id);


    // ---------- OTHER METHODS ----------
    /** @brief displays admin interface. */
    void displayPanel();

    /** ****************************************************************
     * @brief displays the staff details if satff found by the given id.
     * @param staffId of staff to be displayed.
     * ****************************************************************/
    void displayStaffDetails(const std::string &staffId);

    /** **************************************************
     * @brief searches staff using given name and surname.
     * @param staffName to be searched.
     * **************************************************/
    void searchStaffDetailsByName(std::string &staffName);

    /** *********************************************
     * @brief updates few details of specific staff.
     * @param staffId of the staff to update data of.
     * *********************************************/
    void updateStaffDetails(const std::string &staffId);

    /** @brief addStaff adds new staff into database. */
    void addStaff();
    /** @brief removeStaff removes staff from the database. */
    void removeStaff();

    // ---------- LOG FUNCTIONS ----------
    /** @brief provides the interface/menu/options for different type of logs to display */
    void displayLogs();
    /** @brief takes particular date from user and calls displayWithdrawDepositLogs() to display logs */
    void displayWithdrawDepositLogsByDate();
    /** @brief takes particular month from user and calls displayWithdrawDepositLogs() to display logs */
    void displayWithdrawDepositLogsByMonth();
    /** @brief takes particular year from user and calls displayWithdrawDepositLogs() to display logs */
    void displayWithdrawDepositLogsByYear();
};

#endif // ADMIN_H
