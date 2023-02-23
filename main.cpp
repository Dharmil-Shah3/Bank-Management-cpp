#include "bankmanagement.h"

using namespace std;
using json = nlohmann::json;

/** @brief LOGIN enum stores numerical data for different login options(Admin, Staff and Account holder) */
enum LOGIN{
    ADMIN = 1,
    STAFF = 2,
    ACCOUNT_HOLDER = 3
};

void handleBankLogin(short loginChoice);

int main()
{
    short choice = 1;
    while(choice != 0){
        cout << "\n=-=-=-=-=-= LOGIN =-=-=-=-=-=\n" << endl;
        cout << "  1) Admin" << endl;
        cout << "  2) Staff" << endl;
        cout << "  3) Account Holder" << endl;
        cout << "  0) EXIT" << endl;
        Util::scanNumber(choice, "  Enter Choice: ");

        switch (choice) {
            case 0: exit(0);
            case 1: handleBankLogin(LOGIN::ADMIN); break;
            case 2: handleBankLogin(LOGIN::STAFF); break;
            case 3: handleBankLogin(LOGIN::ACCOUNT_HOLDER); break;
            default: cout << "\n  => Enter input in range 0-2" << endl;
        }
    }
    return 0;
}

void handleBankLogin(short loginChoice){
    system("clear");

    Staff *user = NULL; // for staff & admin login
    short failedLoginCount = 1;

    while(1){

        while(failedLoginCount > 3){ // if inputs are invalid 3 times
            short choice;
            cout << "\n ========== 3 Failed Login Attempts ==========" << endl;
            cout << "\n 1) Retry" << endl;
            cout << " 0) EXIT" << endl;
            Util::scanNumber(choice, " Enter choice: ");

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

        string id, password;
        cout << "\n----------- "<< (loginChoice==1? "ADMIN":"STAFF") <<" LOGIN -----------\n" << endl;
        cout << " => Enter userid: ";
        cin >> id;
        cout << " => Enter password: ";
        cin >> password;

        switch (loginChoice) {
            case 1: user = Admin::login(id, password); break;
            case 2: user = Staff::login(id, password); break;
            case 3: cout << "\n ERROR: work in progress..." << endl;
                delete user;
                return;
            default: cout << "\n ERROR: invalid login choice..." << endl;
        }

        if(user == NULL || user == nullptr){ // if login credentials are invalid
            cout << "\n => ERROR: Invalid Userid and Password..." << endl;
            failedLoginCount++;
            continue;   // retry
        }
        user->displayPanel();   // display panel(staff/admin) on successfull login
        delete user;            // delete pointer before leaving function
        break;                  // because operation is done and user is logged out
    }
}
