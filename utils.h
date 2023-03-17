#include <regex>   // for pattern matching and validation
#include <iostream>
#include <string>
#include <fstream> // for file handling
#include <nlohmann/json.hpp>
#include <bits/stdc++.h> // for transform() for lowercase and uppercase strings
#include <ctime>
#include <limits>   // to use "numerical_limits<>" in cin.ignore();

// follwing 3 headers are used for function kbhit()
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef UTILS_H
#define UTILS_H

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
enum BANK_LOG_TYPES{ WITHDRAW, DEPOSIT, ADD_STAFF, REMOVE_STAFF, ADD_BANK_ACC, REMOVE_BANK_ACC };

namespace utils
{

    /** @brief _dataFilename_ stores the name of the user data json file. */
    const std::string _dataFilename_ = "bankdetails.json";
    /** @brief _withdrawDepositLogsFilename_ stores the name of the withdraw-deposit log's json file. */
    const std::string _withdrawDepositLogsFilename_ = "withdraw_deposit_logs.json";

    /** ****************************************************************************
     * @brief kbhit(keyboard hit) checks if any key is pressed from keyboard or not.
     * This function is basically used to exit or break from infinite loops.
     * Loop will be executed until user press any key.
     * @returns 1 if any key is pressed, else returns 0.
     * ****************************************************************************/
    int kbhit(void);

    /** ***********************************************************************************************
     * @brief static method, used to get input numeric values only.
     * Scans the number from the input stream,
     * and if invalid input found then it display error message,
     * and asks for input until valid input not found.
     * @tparam input Reference of a variable in which we will store the input value given by the user.
     * @param message Message to be displayed
     *********************************************************************************************** */
    template <typename T>
    void scanNumber(T &input, const std::string &message)
    {
        using namespace std;
        while(1){
            cout << endl << message;
            cin >> input;

            if(!cin){
                cout << "\n => ERROR: enter numbers only..." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
    }

    /** ************************************************************************************
     * @brief trims string byb given characters.
     * Trims all leading and trailing spaces and space characters from given string.
     * @param str data from which the trimming will be done.
     * @param charactersToRemove is the list of characters that would be trimed from string
     * Default value is all the whitespace chatacters
     ************************************************************************************ */
    const char* trim(std::string &str, const std::string &charactersToRemove=" \t\n\r\f\v");

    /** *********************************************************************************
     * @brief padLeft padds the given string with given character with specified length.
     * @param str is string to be padded.
     * @param totalLength is the total length of the string after padding.
     * @param paddingCharacter is the character to be padded at the start of the string.
     * @return padded string
     * *********************************************************************************/
    std::string padLeft(std::string &str,
                        const unsigned int &totalLength,
                        const char &paddingCharacter = ' ');

    /** *************************************************************************************
     * @brief displays the common & customized error message.
     * @param functionName contains the name of the function in which the error has occurred.
     * @param fileName is the name of the source file in which the exception has occurred.
     * @param message is the error description. Default value is "Unhandled Exception".
     * *************************************************************************************/
    void displayCustomErrorMessage(const std::string &functionName,
                                   const std::string &fileName,
                                   const std::string &message = "Unhandled Exception");

    //=================== Log Functions ========================
    /** *********************************************************************************************************
     * @brief writes the withdraw and deposit logs in the json file.
     * @param type is the type of log to be stored. Valid values are WITHDRAW & DEPOSIT from enum BANK_LOG_TYPES.
     * @param bankAccountId is the bank account number on which the transaction is done.
     * @param amount is the money of transcation.
     * @param staffId represents the staff who has done this operation.
     * *********************************************************************************************************/
    void writeWithdrawDepositeLog(const BANK_LOG_TYPES &type,
                                  const long int &bankAccountId,
                                  const long int &amount,
                                  const std::string &staffId = "");

    //=================== input validation functions using RegEx ========================
    /** ****************************************************************************************
     * @brief validates the given value as the "fullname" of any member of the bank using regex.
     * @param name is the value to validate.
     * @return true if the given value is valid according to regex.
     * ****************************************************************************************/
    bool isNameValid(std::string &name);
    /** ****************************************************************************************
     * @brief validates the given value as the "password" of any member of the bank using regex.
     * @param password is the value to validate.
     * @return true if the given value is valid according to regex.
     * ****************************************************************************************/
    bool isPasswordValid(std::string &password);
    /** ***************************************************************
     * @brief validates the given value as the "email" of staff members.
     * @param email The value to validate.
     * @return true if the given value is valid according to regex.
     * ***************************************************************/
    bool isEmailValid(std::string &email);
    /** *********************************************************************************************
     * @brief validates the given value as the "mobile number" of any member of the bank using regex.
     * @param mobile The value to validate.
     * @return true if the given value is valid according to regex.
     * *********************************************************************************************/
    bool isMobileValid(std::string &mobile);
    /** ***************************************************************************************
     * @brief validates the given value as the "address" of any member of the bank using regex.
     * @param address The value to validate.
     * @return true if the given value is valid according to regex.
     * ***************************************************************************************/
    bool isAddressValid(std::string &address);

    //======================== functions to read data from JSON file ========================
    /** ****************************************************************************************
     * @brief reads data from json file.
     * @param key1 to get specified object. Default value is "".
     * @param key2 to find object from key object. Default value is "".
     * @return json object with specified key & subkey. Returns whole data if no key is specified.
     * ****************************************************************************************/
    nlohmann::json readData(const std::string &key1="", const std::string &key2="");

    /** ****************************************************************************************
     * @brief reads logs from json file
     * @param key1 to get specified object. Default value is ""
     * @param key2 to get object from key object. Default value is ""
     * @param key3 to get sub-object of sub-object of object. Default value is ""
     * @param key4 to get sub-object of sub-object of sub-object of object. Default value is ""
     * @return json object with specified key & subkey. Returns whole data if no key is specified.
     * ****************************************************************************************/
    nlohmann::json readLogs(const unsigned short &key1=0,
                            const unsigned short &key2=0,
                            const unsigned short &key3=0,
                            const unsigned long int &key4=0);

    // ===================== functions that writes the new_data into the file =====================
    /** **********************************************
     * @brief overwrites the given data into json file.
     * @param json data to be written into json file.
     * **********************************************/
    void updateData(const nlohmann::json &data);

    /** ************************************************************************
     * @brief updates the existing data with given new value.
     * @param data is whole json data that needs to be written into json file.
     * @param dataToUpdate is a field/part of the data which needs to be updated.
     * @param newValue is the value that needs to be assigned to dataToUpdate.
     * ************************************************************************/
    template<typename T>
    void updateData(const nlohmann::json &data, nlohmann::json &dataToUpdate, const T &newValue)
    {
        using namespace std;
        ofstream fout;
        try {
            if(dataToUpdate != newValue){
                fout.open(_dataFilename_);
                dataToUpdate = newValue;
                fout << setw(4) << data << flush;
                short lineNumber = __LINE__-1; // just to show linenumber in exception

                if(fout.fail()){ // if any error in file operation
                    throw string("FILE OPERATION FAILED ! DATA IS NOT UPDATED SUCCESSFULLY..."
                                 "\n ON LINE NUMBER " + to_string(lineNumber));
                } else {
                    cout << "\n => Data is updated successfully !" << endl;
                }
            }
            else { // if new and existing values are same
                throw string("NO DATA UPDATED, NEW VALUE IS SIMILAR TO EXISTING !");
            }
        } catch (const string &error) {
            displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error);
        } catch (const exception &error){
            displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
        } catch (...) {
            displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
        }

        fout.close();
        getc(stdin);
    }
}

#endif // UTILS_H
