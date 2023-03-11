#include "bankmanagement.h"

using namespace std;
using namespace utils;

using json = nlohmann::json;

// ------------ COMMON LOGIN FUNCTION ------------
void handleBankLogin(const BANK_USER_ROLES & role);

int main()
{
    try {
        short choice;
        BANK_USER_ROLES role;

        while(true){
            cout << "\n=-=-=-=-=-= LOGIN =-=-=-=-=-=\n" << endl;
            cout << "  1) Admin" << endl;
            cout << "  2) Staff" << endl;
            cout << "  3) Account Holder" << endl;
            cout << "  0) EXIT" << endl;
            scanNumber(choice, "  Enter Choice: ");

            switch (choice) {
                case 0: exit(0);
                case 1: role = ADMIN; break;
                case 2: role = STAFF; break;
                case 3: role = ACCOUNT_HOLDER; break;
                default:
                    cout << "\n  ERROR: INPUT IN RANGE 0-2" << endl;
                    continue;
            }
            handleBankLogin(role);
        }
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
        return 1;
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
        return 1;
    }
    return 0;
}


/** **************************************************************************************
 * @brief handles login operations.
 * It handles the login operation of the system.
 * Handles logins of Staff, Admin and Account Holder
 * @param role is the value of type BANK_USER_ROLES(enum) that defines login roles/options.
 * **************************************************************************************/
void handleBankLogin(const BANK_USER_ROLES &role)
{
    system("clear");

    Staff *user = NULL; // for staff & admin login
    short failedLoginCount = 1;

    try {
    while(1){
        // if inputs are invalid 3 times
        while(failedLoginCount > 3){
            short choice;
            cout << "\n ========== 3 Failed Login Attempts ==========" << endl;
            cout << "\n 1) Retry" << endl;
            cout << " 0) EXIT" << endl;
            scanNumber(choice, " Enter choice: ");

            switch (choice) {
                case 0: system("clear");
                    delete user;
                    return;
                case 1:
                    failedLoginCount = 1;
                    break;
                default:
                    cout << "\n => Invalid choice !" << endl;
            }
        }

        string userid, password;
        cout << "\n----------- "
             << (role == ADMIN ? "ADMIN": (role == STAFF ? "STAFF": "ACCOUNT HOLDER"))
             <<" LOGIN -----------\n" << endl;

        cout << " => Enter userid: ";
        cin >> userid;
        cout << " => Enter password: ";
        cin >> password;

        switch (role) {
            case ADMIN: user = Admin::login(userid, password); break;
            case STAFF: user = Staff::login(userid, password); break;
            case ACCOUNT_HOLDER:
                cout << "\n ERROR: work in progress..." << endl;
                delete user;
                return;
            default: cout << "\n ERROR: INVALID LOGIN CHOICE" << endl;
        }

        if(user == NULL){ // if login credentials are invalid
            failedLoginCount++;
            continue;   // retry
        }

        user->displayPanel();   // display panel(staff/admin) on successfull login
        delete user;            // delete pointer before leaving function
        break;                  // because operation is done and user is logged out
    }
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}
