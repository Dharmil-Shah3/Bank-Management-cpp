#include "bankmanagement.h"

using namespace std;
using json = nlohmann::json;

/** @brief LOGIN enum stores numerical data for different login options(Admin, Staff and Account holder) */
enum LOGIN{
    ADMIN = 1,
    STAFF = 2,
    ACCOUNT_HOLDER = 3
};

void handle_staff_and_admin_login(short admin_or_staff);

int main()
{
    short choice = 1;
    while(choice != 0){
        cout << "\n=-=-=-=-=-= LOGIN =-=-=-=-=-=\n" << endl;
        cout << "  1) Admin" << endl;
        cout << "  2) Staff" << endl;
        cout << "  3) Account Holder" << endl;
        cout << "  0) EXIT" << endl;
        cout << "\n  Enter Choice: ";
        cin >> choice;

        if(!cin){ // handling invalid number input
            cin.clear();
            cout <<  "\n  => ERROR: enter numbers only" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 1;
        }
        else if (choice == 0) exit(0);
        else if (choice == 1) handle_staff_and_admin_login(LOGIN::ADMIN);
        else if (choice == 2) handle_staff_and_admin_login(LOGIN::STAFF);
        else if (choice == 3) cout << "\n  Work in progress..." << endl;
        else cout << "\n  => Enter input in range 0-2" << endl;
    }
    return 0;
}

void handle_staff_and_admin_login(short admin_or_staff){
    system("clear");

    Staff *user = NULL;
    short failedLoginCount = 1;

    while(1){
        failedLoginCount = 1;
        while(failedLoginCount > 3){ // if inputs are invalid 3 times
            short choice;
            cout << "\n ========== 3 Failed Login Attempts ==========" << endl;
            cout << "\n 1) Retry" << endl;
            cout << " 0) EXIT" << endl;
            cout << "\n => Enter choice: ";
            cin >> choice;

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
        cout << "\n----------- "<< (admin_or_staff==1? "ADMIN":"STAFF") <<" LOGIN -----------\n" << endl;
        cout << " => Enter userid: ";
        cin >> id;
        cout << " => Enter password: ";
        cin >> password;

        if(admin_or_staff == 1){
            user = Admin::login(id, password);
        }
        else{
            user = Staff::login(id, password);
        }

        if(user == NULL || user == nullptr){ // if login credentials are invalid
            cout << "\n => Invalid Userid and Password...!" << endl;
            failedLoginCount++;
            continue;   // retry
        }
        user->displayPanel();   // display panel on successfull login
        delete user;            // delete pointer before leaving function
        break;                  // because operation is done and user is logged out
    }
}
