#ifndef BANKERROR_H
#define BANKERROR_H

#include <iostream>

/** ***********************************************************************************************
 * @namespace bankerror
 * @brief it contains all the enums and string constants related to bank-management related errors.
 * ***********************************************************************************************/
namespace bankerror
{
    /** ***********************************************************************************
     * @namespace errmsg
     * @brief This stores the string constants that contains message for custom exceptions.
     * ***********************************************************************************/
    namespace errmsg {
        const std::string INVALID_USER_OBJECT  = "INVALID OBJECT: INVALID USERID OR PASSWORD";
        const std::string USER_NOT_FOUND       = "NO USER FOUND WITH THE GIVEN USERID";
        const std::string BANK_ACC_NOT_FOUND   = "NO BANK ACCOUNT FOUND WITH GIVEN ACCOUNT NUMBER";
        const std::string INVALID_PASSWORD     = "INVALID PASSWORD IS ENTERED";
        const std::string INSUFFICIENT_BALANCE = "INSUFFICIENT BALANCE IN BANK ACCOUNT TO WITHDRAW !";
        const std::string NOT_AN_ADMIN         = "USER IS NOT AN ADMIN";
    }

    /** *********************************************************************
     * @enum ERROR_USER
     * @brief Types of error related to class Staff and Admin Account_Holder
     * *********************************************************************/
    enum ERROR_USER{ USER_NOT_FOUND, INVALID_PASSWORD, NOT_AN_ADMIN, INVALID_USER_OBJECT };

    /** ************************************************************
     * @enum ERROR_BANK_ACCOUNT
     * @brief Types of error related to class Account(bank account).
     * ************************************************************/
    enum ERROR_BANK_ACCOUNT{ BANK_ACC_NOT_FOUND, INSUFFICIENT_BALANCE };
}

#endif // BANKERROR_H
