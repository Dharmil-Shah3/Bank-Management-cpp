#include "bankmanagement.h"

using namespace std;
using namespace utils;

using json = nlohmann::json;

// ------------ COMMON LOGIN FUNCTION ------------
void handleBankLogin(const BANK_USER_ROLES & role);

int main()
{
    try {
        unsigned short choice;
        BANK_USER_ROLES role;

        while(true){
            system("clear");
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
                    getchar(); getchar();
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
    try {
        unique_ptr <Staff>         staff     = NULL;
        unique_ptr <AccountHolder> accHolder = NULL;

        string userid, password;
        cout << "\n----------- "
             << (role == ADMIN ? "ADMIN": (role == STAFF ? "STAFF": "ACCOUNT HOLDER"))
             <<" LOGIN -----------\n" << endl;

        cout << " => Enter userid: ";
        cin >> userid;
        cout << " => Enter password: ";
        cin >> password;

        switch (role) {
            case ADMIN: staff.reset(Admin::login(userid, password)); break;
            case STAFF: staff.reset(Staff::login(userid, password)); break;
            case ACCOUNT_HOLDER: accHolder.reset(AccountHolder::login(userid, password)); break;
        }
        if(staff){
            staff->displayPanel();   // display panel(staff/admin) on successfull login
            return;
        } else if (accHolder){
            accHolder->displayPanel();
            return;
        }
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    getc(stdin);
    getc(stdin);
}
