#include "bankmanagement.h"

using namespace std;
using json = nlohmann::json;

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
            case 1: Util::handleBankLogin(BANK_USER_ROLES::ADMIN); break;
            case 2: Util::handleBankLogin(BANK_USER_ROLES::STAFF); break;
            case 3: Util::handleBankLogin(BANK_USER_ROLES::ACCOUNT_HOLDER); break;
            default: cout << "\n  ERROR: INPUT IN RANGE 0-2" << endl;
        }
    }
    return 0;
}
