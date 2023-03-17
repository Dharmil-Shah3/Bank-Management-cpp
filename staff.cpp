#include "staff.h"
#include <memory.h>

using namespace std;
using namespace utils;
using namespace bankerror;

using json = nlohmann::json;

// ------------ CONSTRUCTOR & DESTRUCTOR --------------
Staff::Staff(const string &id, const string &password)
{
    isUserValid = false;
    try {
        json staff = readData("staff", id);
        if (! staff.empty() && staff["password"] == password) // valid inputs
        {
            string name        = staff["name"];
            string designation = staff["designation"];
            string mobile      = staff["mobile"];
            string address     = staff["address"];
            string email       = staff["email"];

            this->id          = id;
            this->name        = trim(name, "\"");
            this->designation = trim(designation, "\"");
            this->branch_id   = staff["branch_id"];
            this->mobile      = trim(mobile, "\"");
            this->email       = trim(email, "\"");
            this->address     = trim(address, "\"");
            this->salary      = staff["salary"];

            isUserValid = true; // staff is valid
        }
    } catch (const exception &error){
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

// -------------- STATIC METHODS --------------
Staff* Staff::login(const string &userid, const string &password)
{
    try {
        json staff = readData("staff", userid);
        if(staff.empty()){
            throw USER_NOT_FOUND;
        } else if (password != staff["password"]){
            throw INVALID_PASSWORD;
        } else { // all ok then return staff
            return new Staff(userid, password);
        }
    }
    catch (const ERROR_USER &error) {
        if(error == USER_NOT_FOUND) cerr << "\n ERROR: " << errmsg::USER_NOT_FOUND << endl;
        else if(error == INVALID_PASSWORD) cerr << "\n ERROR: " << errmsg::INVALID_PASSWORD << endl;
    }
    catch (const exception & error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    return NULL;
}

// -------------- OTHER METHODS --------------
void Staff::displayPanel()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    short choice = 1;
    system("clear");
    while(choice != 0){
        cout << "\n ----------- STAFF PANEL -----------\n" << endl
             << " 1) Display Your Details " << endl
             << " 2) Update Your Info" << endl << endl
             << " 3) Create Bank Account" << endl
             << " 4) Remove Bank Account" << endl
             << " 5) Display Bank Account Details" << endl
             << " 6) Withdraw Money" << endl
             << " 7) Deposite Money" << endl << endl
             << " 8) Add New Account Holder" << endl
             << " 9) Display Account Holder Details" << endl << endl
             << " 0) Logout" << endl;
        scanNumber(choice, " => Enter Choice: ");

        switch (choice) {
            case 0: system("clear");
                return;
            case 1: this->displayStaffDetails();
                getchar();
                getc(stdin);
                system("clear");
                break;
            case 2: this->updateAccountDetails();
                break;
            case 3: this->createBankAccount();
                break;
            case 4: this->removeBankAccount();
                break;
            case 5: this->displayBankAccountDetails();
                break;
            case 6: this->withdraw();
                break;
            case 7: this->deposit();
                break;
            case 8: this->createAccountHolder();
                break;
            case 9: this->displayAccountHolderDetails();
                break;
            default:
                cout << "\n Invalid choice !" << endl;
        }
    }
}

void Staff::displayStaffDetails()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    cout << "\n\n ================ DETAILS ================\n" << endl;
    cout << " Id          : " << this->id << endl;
    cout << " Name        : " << this->name << endl;
    cout << " Designation : " << this->designation << endl;
    cout << " Branch id   : " << this->branch_id << endl;
    cout << " Mobile      : " << this->mobile << endl;
    cout << " Email       : " << this->email << endl;
    cout << " Address     : " << this->address << endl;
    cout << " Salary      : Rs." << this->salary << endl;
}

void Staff::updateAccountDetails()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    this->displayStaffDetails();
    while(1){
        cout << "\n ===== SELECT WHAT TO UPDATE =====" << endl;
        cout << "\n 1) Name" <<endl;
        cout << " 2) Mobile" <<endl;
        cout << " 3) Email" <<endl;
        cout << " 4) Address" <<endl;
        cout << " 5) Password" <<endl;
        cout << " 0) Cancel" <<endl;

        short choice;

        try {
            scanNumber(choice, " Enter Choice: ");

            string newValue, fieldToUpdate;
            json data = readData();

            if(choice == 0) return;
            else if(choice == 1) fieldToUpdate = "name";
            else if(choice == 2) fieldToUpdate = "mobile";
            else if(choice == 3) fieldToUpdate = "email";
            else if(choice == 4) fieldToUpdate = "address";
            else if(choice == 5){ // updating password requires password verification
                cout << "\n Enter current password: ";
                cin >> newValue;
                if(newValue != data["staff"][this->id]["password"]){
                    cout << "\n => Password is incorrect..." << endl;
                    continue;
                }
                fieldToUpdate = "password";
            }
            else {
                cout << "\n => Invalid Choice..." << endl;
                continue;
            }

            // getting new value from user
            cin.ignore();
            cout << " Enter New "<< fieldToUpdate <<": ";
            getline(cin, newValue);
            trim(newValue);

            // validating inputs
            if(newValue == "" ||
               (fieldToUpdate=="name"     && ! isNameValid(newValue))   ||
               (fieldToUpdate=="mobile"   && ! isMobileValid(newValue)) ||
               (fieldToUpdate=="email"    && ! isEmailValid(newValue))  ||
               (fieldToUpdate=="password" && ! isPasswordValid(newValue)))
            {
                continue;
            }

            // updating data into JSON file as well as into current object
            updateData(data, data["staff"][this->id][fieldToUpdate], newValue);
            switch (choice) { // updating values in the current object
                case 1: this->name = newValue; break;
                case 2: this->mobile = newValue; break;
                case 3: this->email = newValue; break;
                case 4: this->address = newValue; break;
            }
        } catch (const exception &error) {
            displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
        } catch (...) {
            displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
        } // end of try
    } // end of while
}

void Staff::displayBankAccountDetails()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    long int accountNumber;
    try {
        scanNumber(accountNumber, "\n => Enter bank account number: ");
        Account::displayAccountDetails(accountNumber);
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        if(error == BANK_ACC_NOT_FOUND)
            cerr << "\n ERROR: " << errmsg::BANK_ACC_NOT_FOUND << endl;
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

void Staff::displayAccountHolderDetails()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    long int accountHolderId;
    cout << "\n ======== ACCOUNT HOLDER DEATILS ========" << endl;
    scanNumber(accountHolderId, " Enter Account Holder Id: ");

    try {
        json accountHolder = readData("account_holder", to_string(accountHolderId));

        if(accountHolder.empty()){
            throw USER_NOT_FOUND;
        } else {
            string name = accountHolder["name"];
            string mobile = accountHolder["mobile"];
            string address = accountHolder["address"];

            cout << "\n Id            : " << accountHolderId << endl
                 << " Name          : " << trim(name, "\"") << endl
                 << " Mobile Number : " << trim(mobile, "\"") << endl
                 << " Address       : " << trim(address, "\"") << endl
                 << " Bank Accounts : " << accountHolder["bank_accounts"] << endl;
        }
    } catch (const ERROR_USER &error) {
        if(error == USER_NOT_FOUND)
            cout << "\n ERROR: " << errmsg::USER_NOT_FOUND<< endl;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }

    getchar();
    getc(stdin);
    system("clear");
}

string Staff::createAccountHolder()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;
    /// @todo test this function
    string name, address, mobile, password;
    system("clear");
    cout << "====== ENTER ACCOUNT HOLDER DETAILS ======" << endl
         << " => Enter Zero(0) to cancle..." << endl;

    try {
        cin.ignore();

        do {
            cout << "\n Name: ";
            getline(cin, name);
            trim(name);
            if(name == "0") // operation cancel
                throw -1;
        } while (!isNameValid(name)); // until the name is not valid

        do {
            cout << "\n Mobile Number: ";
            getline(cin, mobile);
            trim(mobile);
            if(mobile == "0") // operation cancel
                throw -1;
        } while (!isMobileValid(mobile)); // until the mobile is not valid

        do {
            cout << "\n Address: ";
            getline(cin, address);
            trim(address);
            if(address == "0") // operation cancel
                throw -1;
        } while (!isAddressValid(address)); // until the address is not valid

        do {
            cout << "\n Password: ";
            getline(cin, password);
            trim(password);
            if(password == "0") // operation cancel
                throw -1;
        } while (!isPasswordValid(password)); // until the address is not valid

        string newAccountHolderId = AccountHolder::createAccountHolder(name, mobile, address, password);
        return newAccountHolderId;

    } catch (const int &error){
        if (error == -1) // operation cancel
            cerr << "\n => Operation cancelled: add new account holder..." <<  endl;
    } catch (const char* error) {
        cerr << error;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    return NULL; // if account is not created
}

void Staff::createBankAccount()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    unsigned long int balance;
    short choice;
    string accountType, accountHolderId;

    try {
        // ---------- Getting Account Holder Id ----------
        while(1){ // iterate until valid account holder id is entered
            cout << " => Enter account holder id for new bank account: ";
            cin >> accountHolderId;
            json accountHolder = readData("account_holder", accountHolderId);

            // if account holder not found
            if(accountHolder.empty()){
                while(1){
                    cout << "\n ============ Account Holder Not Found ============" << endl
                         << "\n 1) Add new Account Holder"
                         << "\n 2) Re-enter Account Holder Id "
                         << "\n 3) Cancel";
                    scanNumber(choice, "\n Enter choice: ");

                    if(choice == 1){
                        accountHolderId = this->createAccountHolder();
                        if(accountHolderId.empty()){ // if account holder is not created properly
                            cout << "\n ERROR: ACCOUNR HOLDER IS NOT CREATED PROPERLY ! TRY AGAIN..." << endl;
                            getc(stdin);
                            continue;
                        }
                        break;
                    } else if(choice == 2){
                        break;
                    } else if(choice == 3){
                        return;
                    } else {
                        cout << "\n => ERROR: invalid choice entered..." << endl;
                    }
                }
            }
            else { // valid account holder is found
                break;
            }
        }

        // ---------- Getting Account Type ----------
        while(1){
            cout << "\n => ACCOUNT TYPE <=" << endl
                 << "\n    1) Savings"
                    "\n    2) Current"
                    "\n    0) Cancle";
            scanNumber(choice, "\n Enter choice: ");

            if(choice == 0){
                cout << "\n => Operation cancelled : add new account holder" << endl;
                getc(stdin);
                return;
            }
            else if(choice == 1 || choice == 2){
                accountType = (choice == 1? "Saving": "Current");
                break;
            }
            else cout << "\n => ERROR: invalid choice entered..." << endl;
        }

        // -------- initial bank balance --------
        scanNumber(balance, " Enter Initial balance: ");

        while(balance < 1){ // if invalid amount is entered
            cout << "\n ERROR: negative or zero amount is entered. please enter valid amount..." << endl;
            scanNumber(balance, " Enter Initial balance: ");
        }

        long int bankAccountId = Account::createNewAccount(accountHolderId, balance, accountType);
        cout << "\n => New Account is created...";
        Account(bankAccountId).displayAccountDetails();
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        if(error == BANK_ACC_NOT_FOUND)
            cout << "\n ERROR: BANK ACCOUNT MAY NOT CREATED CORRECTLY" << error << endl;
    }
    catch (const ERROR_USER &error) {
        if(error == USER_NOT_FOUND)
            cout << "\n ERROR: " << errmsg::USER_NOT_FOUND << endl;
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

void Staff::removeBankAccount()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    int account_number;
    try {
        scanNumber(account_number, "\n Enter Bank Account Number to Remove: ");
        Account::removeAccount(account_number, this->id);
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        if(error == BANK_ACC_NOT_FOUND)
            cerr << "\n ERROR: " << errmsg::BANK_ACC_NOT_FOUND << endl;
    }
    catch (const ERROR_USER &error) {
        if(error == INVALID_PASSWORD)
            cerr << "\n ERROR: " << errmsg::INVALID_PASSWORD << endl;
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

void Staff::withdraw()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    long int accountNumber, amount;
    try {
        scanNumber(accountNumber, " => Enter bank account number: ");

        Account acc(accountNumber);
        scanNumber(amount, " => Enter amount to Withdraw: Rs.");
        if(amount < 1) // if negative amount is entered
            throw amount;
        acc.withdraw(amount ,this->id);
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        if(error == ERROR_BANK_ACCOUNT::BANK_ACC_NOT_FOUND)
            cerr << "\n ERROR: " << errmsg::BANK_ACC_NOT_FOUND << endl;
        else if(error == ERROR_BANK_ACCOUNT::INSUFFICIENT_BALANCE)
            cerr << "\n ERROR: " << errmsg::INSUFFICIENT_BALANCE << endl;
    }
    catch (const long int &amount){
        cerr << "\n ERROR: NEGATIVE AMOUNT (" << amount << ") IS ENTERED."
             << " PLEASE ENTER VALID AMOUNT..." << endl;
    } catch (const string &error){
        cerr << "\n ERROR: " << error << endl;
    } catch (const exception &error){
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...){
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

void Staff::deposit()
{
    if(!isUserValid) throw INVALID_USER_OBJECT;

    long int accountNumber, amount;
    scanNumber(accountNumber, " => Enter bank account number: ");

    try {
        // Account constructor will throw an error (string type), if account is not found
        Account acc(accountNumber);
        scanNumber(amount, " => Enter amount to Deposit: Rs.");
        if(amount < 1) // if negative amount is entered
            throw amount;
        acc.deposit(amount, this->id);
    }
    catch (const ERROR_BANK_ACCOUNT &error) {
        if(error == ERROR_BANK_ACCOUNT::BANK_ACC_NOT_FOUND)
            cout << "\n ERROR: " << errmsg::BANK_ACC_NOT_FOUND << endl;
    }
    catch (const long int &amount){
        cerr << "\n ERROR: NEGATIVE OR ZERO AMOUNT (" << amount << ") IS ENTERED" << endl
             << " PLEASE ENTER VALID AMOUNT..." << endl;
    } catch (const exception &error){
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}
// =================================== END STAFF ======================================
