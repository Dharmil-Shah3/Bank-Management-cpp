#include "bankmanagement.h"

using namespace std;
using json = nlohmann::json;


// ===================================== Utils =====================================
// ------- STATIC DATA -------
const string Utils::filename = "bankdetails.json";
const string Utils::logFilename = "banklogs.json";

// ------- STATIC METHODS -------
string Utils::getFileName(){ return filename; }
string Utils::getLogFileName(){ return logFilename; }

template <typename T>
void Utils::scanNumber(T &input, const string &message){
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

const char* Utils::trim(string &str, const string &characterstoRemove)
{
    str.erase(str.find_last_not_of(characterstoRemove) + 1);
    str.erase(0, str.find_first_not_of(characterstoRemove));
    return str.c_str();
}

bool Utils::isNameValid(string& name)
{
    // converting string into lowercase
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    trim(name);
    regex pattern{"^[a-z]{3,15}( [a-z]{3,15})?$"};
    if(regex_search(name, pattern))
        return true;
    cout << "\n => ERROR: name must contain alphabets only"
         << "\n => Valid name examples: [ram], [ram charan]" << endl;
    getc(stdin);
    return false;
}
bool Utils::isPasswordValid(string& password)
{
    trim(password);
    regex pattern{"^[a-zA-Z0-9!@#$_]{4,20}$"};
    if(regex_search(password, pattern))
        return true;
    cout << "\n => ERROR: invalid characters or length in password"
         << "\n -> valid characters are alphabets, numbers, !, @, #, $, _ (underscore)"
         << "\n -> length must be betweeen 4 to 20 characters" << endl;
    getc(stdin);
    return false;
}
bool Utils::isEmailValid(string& email)
{
    trim(email);
    transform(email.begin(), email.end(), email.begin(), ::tolower);
    regex pattern{"^[a-z][a-z0-9._]{2,20}@[a-z]{3,15}.[a-z]{2,4}$"};
    if(regex_search(email, pattern))
        return true;
    cout << "\n => ERROR: invalid email format" << endl;
    getc(stdin);
    return false;
}
bool Utils::isMobileValid(string& mobile)
{
    trim(mobile);
    regex pattern{"^[\\d]{10}$"};
    if(regex_search(mobile, pattern))
        return true;
    cout << "\n => ERROR: invalid mobile number"
         << "\n -> length must be 10 digits"
         << "\n -> only digits are allowed" << endl;
    getc(stdin);
    return false;
}
bool Utils::isAddressValid(string& address)
{
    trim(address);
    transform(address.begin(), address.end(), address.begin(), ::tolower);
    regex pattern{"^[a-z0-9][ a-z0-9/,\\-]{10,50}$"};
    if(regex_search(address, pattern))
        return true;
    cout << "\n => ERROR: invalid address"
         << "\n -> length must be between 10-50 characters" << endl;
    getc(stdin);
    return false;
}

int Utils::kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

json Utils::readData(const string &key1, const string &key2)
{
    /// @todo make an array of parameters [].
    try {
        ifstream fin(filename);
        if(!fin || fin.fail()){ // if file is not opened
            string error = "error in opening the file " + filename + " for reading,"
                         + " on line number " + to_string(__LINE__-3) + " in " __FILE__;
            throw error;
        }

        json data = json::parse(fin);
        fin.close();

        if(key1 == "") return data;
        else if(key2 == "") return data[key1];
        else return data[key1][key2];
    } catch (const string &error) {
        cout << "\n ERROR: " << endl;
    } catch (const exception &e) {
        cout << "\n ERROR: " << e.what() << endl;
    }
    return NULL;
}

json Utils::readLogs(const unsigned short &key1, const unsigned short &key2,
                     const unsigned short &key3, const unsigned long int &key4)
{
    try {
        ifstream fin(logFilename);
        if(!fin){ // if file is not opened
            string error = "error in opening the file " + logFilename + " for reading,"
                         + " on line number " + to_string(__LINE__-3) + " in " __FILE__;
            throw error;
        }
        json data = json::parse(fin);
        fin.close();

        if(key1 == 0) return data;
        else if(key2 == 0) return data[to_string(key1)];
        else if(key3 == 0) return data[to_string(key1)][to_string(key2)];
        else if(key4 == 0) return data[to_string(key1)][to_string(key2)][to_string(key3)];
        else return data[to_string(key1)][to_string(key2)][to_string(key3)][to_string(key4)];
    } catch (const string &error) {
        cout << "\n ERROR: " << endl;
    } catch (const exception &e) {
        cout << "\n ERROR: " << e.what() <<endl;
    }
    return NULL;
}

void Utils::writeWithdrawDepositeLog(const BANK_LOG_TYPES &type,
                                     const long &bankAccountId,
                                     const long &amount,
                                     const std::string &staffId = "")
{
    try {
        if(type != WITHDRAW && type != DEPOSIT){
            throw string("INVALID LOG TYPE PASSED AS PARAMETER IN FUNCTION writeWithdrawDepositLog() IN bankmanagement.cpp");
        }

        // variable declarations
        json transaction, data;
        ofstream fout;
        ifstream fin;
        string year, month, day;
        long int transactionId;
        short hour, minute, second;
        time_t now;
        tm *currentTime;

        now         = time(0);
        currentTime = localtime(&now);

        year   = to_string(currentTime->tm_year + 1900);
        month  = to_string(currentTime->tm_mon + 1);
        day    = to_string(currentTime->tm_mday);
        hour   = currentTime->tm_hour;
        minute = currentTime->tm_min;
        second = currentTime->tm_sec;

        transaction = { // json transaction object
            {"type", type == WITHDRAW? "withdraw": "deposit"},
            {"account_number", bankAccountId},
            {"amount", amount},
            {"staff_id", staffId},
            {"time",{
                 {"hour",hour},
                 {"minute", minute},
                 {"second", second}}
            }
        };

        fin.open(logFilename);
        transactionId = 0;
        data = json::parse(fin);

        /// @todo fix bug
        if(fin){ // if file exists, then read transactionId from file
            transactionId = data["total_transactions"];
        }
        fin.close();

        // update total transactions and get transaction id for current transaction
        data["total_transactions"] = ++transactionId;

        // update transactionId into json file quickly so other can get proper value of it
        fout.open(logFilename);
        fout << setw(4) << data << endl;
        fout.close();

        // making treansaction id with leading 0s.
        ostringstream transactionIdWithLeadindZeors;
        transactionIdWithLeadindZeors << setw(8) << setfill('0') << to_string(transactionId);

        // adding new transaction in data
        data[year][month][day] += json::object_t::value_type(transactionIdWithLeadindZeors.str(), transaction);

        // write the transatction object
        fout.open(logFilename);
        fout << setw(4) << data << endl;
        fout.close();
    }
    catch (const string &error){
        cout << "\n ERROR: " << error << endl;
    }
    catch (const exception &error) {
        cout << "\n ERROR: " << error.what() << endl;
    }
}

void Utils::updateData(const nlohmann::json &data)
{
    ofstream fout;
    try {
        fout.open(filename);
        fout << setw(4) << data << flush;
        if(fout.fail()){
            string error = "FILE OPERATION FAILED ! DATA IS NOT UPDATED SUCCESSFULLY..."
                           "\n ON LINE NUMBER " + to_string(__LINE__-1) + " IN FILE " + __FILE__;
            throw error;
        } else {
            cout << "\n => Data is updated successfully..." << endl;
        }
    } catch (const string &e){ // if any error in file operation
        cout << "\n ERROR: " << e << endl;
    } catch (const exception &e) {
        cout << "\n ERROR: " << e.what() << " in updateData(json) in bankmanagement.cpp file" << endl;
    }
    fout.close();
    getc(stdin);
}

template <typename T>
void Utils::updateData(const json &data, json &dataToUpdate, const T &newValue)
{
    ofstream fout;
    try {
        if(dataToUpdate != newValue){
            fout.open(filename);
            dataToUpdate = newValue;
            fout << setw(4) << data << flush;
            short lineNumber = __LINE__-1; // just to show linenumber in exception

            if(fout.fail()){ // if any error in file operation
                string error = "FILE OPERATION FAILED ! DATA IS NOT UPDATED SUCCESSFULLY..."
                               "\n ON LINE NUMBER " + to_string(lineNumber) + " IN FILE " + __FILE__;
                throw error;
            } else {
                cout << "\n => Data is updated successfully !" << endl;
            }
        }
        else { // if new and existing values are same
            string error = "NO DATA UPDATED, NEW VALUE IS SIMILAR TO EXISTING !";
            throw error;
        }
    } catch (const string &e) {
        cout << "\n ERROR: " << e << endl;
    } catch (const exception &e){
        cout << "\n ERROR: " << e.what() << " in updateData()" << endl;
    }
    fout.close();
    getc(stdin);
}

void Utils::handleBankLogin(const BANK_USER_ROLES &role)
{
    system("clear");

    Staff *user = NULL; // for staff & admin login
    short failedLoginCount = 1;

    while(1){
        // if inputs are invalid 3 times
        while(failedLoginCount > 3){
            short choice;
            cout << "\n ========== 3 Failed Login Attempts ==========" << endl;
            cout << "\n 1) Retry" << endl;
            cout << " 0) EXIT" << endl;
            Utils::scanNumber(choice, " Enter choice: ");

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
             << (role == BANK_USER_ROLES::ADMIN ? "ADMIN": (role == BANK_USER_ROLES::STAFF ? "STAFF": "ACCOUNT HOLDER"))
             <<" LOGIN -----------\n" << endl;

        cout << " => Enter userid: ";
        cin >> userid;
        cout << " => Enter password: ";
        cin >> password;

        switch (role) {
            case BANK_USER_ROLES::ADMIN: user = Admin::login(userid, password); break;
            case BANK_USER_ROLES::STAFF: user = Staff::login(userid, password); break;
            case BANK_USER_ROLES::ACCOUNT_HOLDER: cout << "\n ERROR: work in progress..." << endl;
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
}
// ======================================= END Utils ======================================



// ===================================== STAFF =====================================

// ------------ CONSTRUCTOR & DESTRUCTOR --------------
Staff::Staff(const string &id, const string &password)
{
    json staff = Utils::readData("staff");
    if (staff.contains(id)){ // staff found
        if(staff[id]["password"] != password){ // invalid password
            throw ERROR_STAFF::INVALID_PASSWORD;
        }
        string name = staff[id]["name"];
        string designation = staff[id]["designation"];
        string mobile = staff[id]["mobile"];
        string address = staff[id]["address"];
        string email = staff[id]["email"];

        this->id = id;
        this->name = Utils::trim(name, "\"");
        this->designation = Utils::trim(designation, "\"");
        this->branch_id = staff[id]["branch_id"];
        this->mobile = Utils::trim(mobile, "\"");
        this->email = Utils::trim(email, "\"");
        this->address = Utils::trim(address, "\"");
        this->salary = staff[id]["salary"];
    } else {
        throw ERROR_STAFF::STAFF_NOT_FOUND;
    }
}

// -------------- STATIC METHODS --------------
Staff* Staff::login(const string &userid, const string &password)
{
    try {
        Staff *staff = new Staff(userid, password);
        return staff;
    }
    catch (const ERROR_STAFF &error) {
        switch (error) {
            case ERROR_STAFF::STAFF_NOT_FOUND:
                cout << "\n ERROR: NO STAFF MEMBER FOUND WITH ID \""<< userid << "\"" << endl;
                break;
            case ERROR_STAFF::INVALID_PASSWORD:
                cout << "\n ERROR: INVALID PASSWORD. TRY AGAIN..." << endl;
                break;
        }
        return NULL;
    }
    catch (const std::exception & error) {
        perror("\n ERROR: ");
        return NULL;
    }
}

void Staff::displayStaffDetails(const std::string &staffId)
{
    json staff = Utils::readData("staff", staffId);
    if(!staff.empty()){
        Staff(staffId, staff["password"]).displayStaffDetails();
    } else {
        cout << "\n ERROR: NO STAFF MEMBER FOUND WITH ID \""<< staffId << "\"" << endl;
    }
}

// -------------- METHODS --------------
void Staff::displayPanel()
{
    short choice = 1;
    system("clear");
    while(choice != 0){
        cout << "\n ----------- STAFF PANEL -----------\n" << endl;
        cout << " 1) Display Your Details " << endl;
        cout << " 2) Update Your Info" << endl;
        cout << " 3) Create Bank Account" << endl;
        cout << " 4) Remove Bank Account" << endl;
        cout << " 5) Display Bank Account Details" << endl;
        cout << " 6) Withdraw Money" << endl;
        cout << " 7) Deposite Money" << endl;
        cout << " 0) Logout" << endl;
        Utils::scanNumber(choice, " => Enter Choice: ");

        switch (choice) {
            case 0: system("clear");
                return;
            case 1: this->displayStaffDetails();
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
            default:
                cout << "\n Invalid choice !" << endl;
        }
    }
}

void Staff::displayStaffDetails()
{
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
        Utils::scanNumber(choice, " Enter Choice: ");

        string newValue, fieldToUpdate;
        json data = Utils::readData();

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
        Utils::trim(newValue);

        // validating inputs
        if(newValue == "" ||
           (fieldToUpdate=="name" && !Utils::isNameValid(newValue)) ||
           (fieldToUpdate=="mobile" && !Utils::isMobileValid(newValue)) ||
           (fieldToUpdate=="email" && !Utils::isEmailValid(newValue)) ||
           (fieldToUpdate=="password" && !Utils::isPasswordValid(newValue))
        ){
            continue;
        }

        // updating data into JSON file as well as into current object
        Utils::updateData(data, data["staff"][this->id][fieldToUpdate], newValue);
        switch (choice) { // updating values in the current object
            case 1: this->name = newValue; break;
            case 2: this->mobile = newValue; break;
            case 3: this->email = newValue; break;
            case 4: this->address = newValue; break;
        }
    }
}

void Staff::displayBankAccountDetails()
{
    long int accountNumber;
    try {
        Utils::scanNumber(accountNumber, "\n => Enter bank account number: ");
        Account::displayAccountDetails(accountNumber);
    } catch (const string &error) {
        cout << "\n ERROR: "<< error << endl;
    } catch (...){
        cout << "\n ERROR: exception in Staff::displayBankAccountDetails()" << endl;
    }

}

long int Staff::createAccountHolder()
{
    /// @todo test this function
    string name, address, mobile;
    system("clear");
    cout << "====== ENTER ACCOUNT HOLDER DETAILS ======" << endl
         << " => Enter 0(zero) to cancle..." << endl;

    try {
        cin.ignore();

        do {
            cout << "\n Name: ";
            getline(cin, name);
            Utils::trim(name);
            if(name == "0") // operation cancel
                throw -1;
        } while (!Utils::isNameValid(name)); // until the name is not valid

        do {
            cout << "\n Mobile Number: ";
            getline(cin, mobile);
            Utils::trim(mobile);
            if(mobile == "0") // operation cancel
                throw -1;
        } while (Utils::isMobileValid(mobile)); // until the mobile is not valid

        do {
            cout << "\n Address: ";
            getline(cin, address);
            Utils::trim(address);
            if(address == "0") // operation cancel
                throw -1;
        } while (Utils::isAddressValid(address)); // until the address is not valid

        long int newAccountHolderId = AccountHolder::createAccountHolder(name, mobile, address);
        return newAccountHolderId;

    } catch (const int &error){
        if (error == -1){ // operation cancel
            cout << "\n => Operation cancelled: add new account holder..." <<  endl;
        }
    } catch (const char* error) {
        cout << error;
    } catch (...) {
        cout << "\n ERROR: EXCEPTION IN Staff::createAccountHolder()" << endl;
    }
    return 0; // if account is not created
}

void Staff::createBankAccount()
{
    long int accountHolderId, balance;
    short choice;

    try {
        // ---------- Getting Account Holder Id ----------
        while(1){
            Utils::scanNumber(accountHolderId, "\n Enter account holder id: ");
            json accountHolder = Utils::readData("account_holder", to_string(accountHolderId));

            // if account holder not found
            if(accountHolder.empty()){
                long int newAccountHolderId;
                while(1){
                    cout << "\n ============ Account Holder Not Found ============" << endl
                         << "\n 1) Add new Account Holder"
                         << "\n 2) Re-enter Account Holder Id "
                         << "\n 3) Cancel";
                    Utils::scanNumber(choice, "\n Enter choice: ");

                    if(choice == 1){
                        newAccountHolderId = this->createAccountHolder();
                        if(newAccountHolderId == 0){ // if account holder is not created properly
                            cout << "\n ERROR: ACCOUNR HOLDER IS NOT CREATED PROPERLY ! TRY AGAIN..." << endl;
                            continue;
                        }
                    } else if( choice == 2){
                        break;
                    } else if( choice == 3){
                        return;
                    } else {
                        cout << "\n => ERROR: invalid choice entered..." << endl;
                    }
                }
            }
        }

        // ---------- Getting Account Type ----------
        string accountType;
        while(1){
            cout << "\n => ACCOUNT TYPE <=" << endl
                 << "\n    1) Savings"
                    "\n    2) Current";
            Utils::scanNumber(choice, "\n Enter choice: ");

            if(choice == 1 || choice == 2){
                accountType = (choice == 1? "Saving": "Current");
                break;
            }
            else cout << "\n => ERROR: invalid choice entered..." << endl;
        }

        Utils::scanNumber(balance, " Enter Initial balance: ");

        while(balance < 1){
            Utils::scanNumber(balance, " Enter Initial balance: ");
        }

        Account acc = Account::createNewAccount(accountHolderId, balance, accountType);
        cout << "\n => New Account is created...";
        acc.displayAccountDetails();

    } catch (const string &error) {
        cout << "\n ERROR: " << error << endl;
    } catch (const exception &error) {
        cout << "\n => ERROR: " << error.what() << endl;
    }
}

void Staff::removeBankAccount()
{
    int account_number;
    try {
        Utils::scanNumber(account_number, "\n Enter Bank Account Number to Remove: ");
        Account::removeAccount(account_number, this->id);
    } catch (const string &error) {
        cout << "\n ERROR: " << error << endl;
    } catch (...) {
        cout << "\n ERROR: EXCEPTION IN Staff::removeBankAccount()" << endl;
    }
}

void Staff::withdraw()
{
    long int accountNumber, amount;
    Utils::scanNumber(accountNumber, " => Enter bank account number: ");

    try {
        // Account constructor will throw an error (string type), if account is not found
        Account acc(accountNumber);
        Utils::scanNumber(amount, " => Enter amount to Withdraw: Rs.");
        if(amount < 1) // if negative amount is entered
            throw amount;
        acc.withdraw(amount ,this->id);
    } catch (const long int &amount){
        cout << "\n ERROR: NEGATIVE AMOUNT (" << amount << ") IS ENTERED" << endl
             << " PLEASE ENTER VALID AMOUNT..." << endl;
    } catch (const string & error){
        cout << "\n ERROR: " << error << endl;
    } catch (const exception &e){
        cout << "\n ERROR: " << e.what() << " in Staff::withdraw()" << endl;
    }
}

void Staff::deposit()
{
    long int accountNumber, amount;
    Utils::scanNumber(accountNumber, " => Enter bank account number: ");

    try {
        // Account constructor will throw an error (string type), if account is not found
        Account acc(accountNumber);
        Utils::scanNumber(amount, " => Enter amount to Deposit: Rs.");
        if(amount < 1) // if negative amount is entered
            throw amount;
        acc.deposit(amount, this->id);
    } catch (const long int &amount){
        cout << "\n ERROR: NEGATIVE AMOUNT (" << amount << ") IS ENTERED" << endl
             << " PLEASE ENTER VALID AMOUNT..." << endl;
    } catch (const string & error){
        cout << "\n ERROR: " << error << endl;
    } catch (const exception &e){
        cout << "\n ERROR: " << e.what() << " in Staff::withdraw()" << endl;
    }
}
// =================================== END STAFF ======================================



// ===================================== ADMIN =====================================

// ------- CONSTRUCTOR -------
Admin::Admin(const string &adminId, const string &password) : Staff(adminId, password)
{
    try {
        if (!isAdmin(adminId)){ //  if its not an admin
            throw ERROR_STAFF::NOT_AN_ADMIN;
        }
    } catch (const ERROR_STAFF &error) {
        throw error;
    } catch (const std::exception &error){
        throw error;
    }
}

// ------- STATIC METHODS -------
Admin* Admin::login(const string &userid, const string &password)
{
    try {
        Admin *admin = new Admin(userid, password);
        return admin;
    }
    catch (const ERROR_STAFF & error) {
        switch (error) {
            case ERROR_STAFF::STAFF_NOT_FOUND:
                cout << "\n ERROR: NO STAFF MEMBER FOUND WITH ID \""<< userid << "\"" << endl;
                break;
            case ERROR_STAFF::INVALID_PASSWORD:
                cout << "\n ERROR: INVALID PASSWORD. TRY AGAIN..." << endl;
                break;
            case ERROR_STAFF::NOT_AN_ADMIN:
                cout << "\n ERROR: \""<< userid <<"\" IS NOT AN ADMIN"<< endl;
                break;
        }
        return NULL;
    }
    catch (const std::exception & error) {
        perror("\n ERROR: ");
        return NULL;
    }
}

bool Admin::isAdmin(const string &id)
{
    json admins = Utils::readData("admin");
    for(auto& admin: admins.items()){
        if(id == admin.value()) return true;
    }
    return false;
}

// ------- PRIVATE METHODS -------
string Admin::selectDesignation()
{
    short choice;
    while(1){
        cout << "\n ===== SELECT DESIGNATION =====" << endl
             << " 1) Branch Manager" << endl
             << " 2) Cashier" << endl
             << " 3) Loan officer" << endl
             << " 4) Security" << endl
             << " 5) PO" << endl;
        Utils::scanNumber(choice, " Enter Designation Choice: ");

        switch (choice) {
            case 1: return "Branch Manager";
            case 2: return "Cashier";
            case 3: return "Loan Officer";
            case 4: return "Security";
            case 5: return "PO";
            default: cout << "\n ERROR: invalid choice enterd..." << endl;
        }
    }
}

void Admin::searchStaff()
{
    short choice;
    string input;
    cout << "\n ======= SEARCH STAFF BY =======\n" << endl;
    cout << " 1) Id"<<endl;
    cout << " 2) Name"<<endl;
    Utils::scanNumber(choice, " Enter choice: ");

    switch (choice) {
        case 1:
            cout << "\n Enter Staff id: ";
            cin.ignore();
            getline(cin, input);
            this->Staff::displayStaffDetails(input);
            break;
        case 2:
            cout << "\n Enter Staff Name: ";
            cin.ignore();
            getline(cin, input);
            this->searchStaffDetailsByName(input);
            break;
        default:
            cout << "\n => Invalid choice !" << endl;
            choice = -1; // for preventing logout if '0' is pressed
    }
}

// ------- UTIL METHODS -------
void Admin::displayPanel()
{
    short choice = 1;
    string input; // for scan staff id and name
    system("clear");

    while(choice != 0){
        cout << "\n ----------- ADMIN PANEL -----------\n" << endl;
        cout << " 1) Display Your Details " << endl;
        cout << " 2) Update Your Info" << endl;
        cout << " 3) Search Staff" << endl;
        cout << " 4) Update Staff Details" << endl;
        cout << " 5) Add New Staff" << endl;
        cout << " 6) Remove Staff" << endl;
        cout << " 7) Display Staff Menu" << endl;
        cout << " 8) Display Logs" << endl;
        cout << " 0) Logout" << endl;
        Utils::scanNumber(choice, " => Enter Choice: ");

        switch (choice) {
            case 0: system("clear"); return;
            case 1:
                this->displayStaffDetails();
                getc(stdin);
                break;
            case 2: this->updateAccountDetails(); break;
            case 3: this->searchStaff(); break;
            case 4:
                cout << "\n Enter Staff id: ";
                cin.ignore();
                getline(cin, input);
                this->updateStaffDetails(input);
                break;
            case 5: this->addStaff(); break;
            case 6: this->removeStaff(); break;
            case 7: this->Staff::displayPanel(); break;
            case 8: this->displayLogsByMonth(); break;
            default: cout << "\n => ERROR: invalid choice entered..." << endl;
        }
        getc(stdin);
        system("clear");
    }
}

void Admin::displayLogsByMonth()
{
    json logs;
    unsigned short year, month;

    // getting the year and month from user to display logs of that month
    system("clear");
    cout << "===== Withdraw & Deposite logs by Month =====" << endl
         << "\n [ Enter 0 for current month ]" << endl;

    Utils::scanNumber(year ," Enter year: ");

    // if year == 0, means current year and month should be considered
    if(year == 0){
        //get the current year and month using time_t
        time_t now = time(0);
        tm *currentTime = localtime(&now);
        year = currentTime->tm_year + 1900;
        month = currentTime->tm_mon + 1;
    } else { // get the specific month from user
        Utils::scanNumber(month ," Enter month: ");
    }

    /*
     * getchar() is called here to clear the input stream for kbhit() used in next looping statement.
     * kbhit() checks for the keypress to stop infinate loop
     */
    getchar();

    while(! Utils::kbhit()){ // showing logs until any key is pressed from keyboard
        system("clear");

        logs = Utils::readLogs(year, month);
        if(logs.empty()){
            cout << "\n NO LOGS FOUND FOR "<<year<<"/"<<month << endl;
            return;
        }

        cout << "\n=-=-=-=-=-=-=-=-=-=-=-=-= Logs of "<<year<<"/"<<month<<" =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "\n| Trans_Id |   Desc   | Bank Account Id | Amount(Rs) | Staff id | Date |   Time   |" << endl
             << "-----------------------------------------------------------------------------------" << endl;

        string day, transactionType, staffId;

        for(auto &date: logs.items()){
            day = date.key();
            for(auto &transaction: date.value().items()){
                json value = transaction.value();
                transactionType = to_string(value["type"]);
                transactionType = transactionType.substr(1, transactionType.length()-2);
                staffId = to_string(transaction.value()["staff_id"]);
                int lastIndexOfZero = transaction.key().find_first_not_of('0');
                string transactionId = transaction.key().substr(lastIndexOfZero);

                staffId = staffId == "\"\""? "": staffId.substr(1,8);

                cout << "| " << setw(8) << transactionId << " "
                     << "| " << setw(8) << transactionType << " "
                     << "| " << setw(15) << to_string(value["account_number"]) << " "
                     << "| " << setw(10) << to_string(value["amount"]) << " "
                     << "| " << setw(8) << staffId << " "
                     << "|  " << setw(2) << setfill('0') << day << "  "
                     << "| " << setw(2) << to_string(value["time"]["hour"])
                     << ":" << setw(2) << to_string(value["time"]["minute"])
                     << ":" << setw(2) << to_string(value["time"]["second"]) << " |" << setfill(' ')<< endl;
            }
        }
        cout << "-----------------------------------------------------------------------------------" << endl
             << "\n=> Press any key to exit..." << endl;
        sleep(1);
    }
}

void Admin::searchStaffDetailsByName(string &inputName)
{
    json allStaff = Utils::readData("staff");
    Utils::trim(inputName);
    transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);
    unsigned short recordsFound = 0;

    string name;
    string designation;
    string email;
    string mobile;

    cout << "\n -------------------------------------------------------------------------------------------------------------" << endl;
    cout << " |    ID    |        NAME        |   DESIGNATION   | BRANCH ID |   MOBILE   |             EMAIL              |" << endl
         << " -------------------------------------------------------------------------------------------------------------" << endl;

    for(auto &staff: allStaff.items()){
        name = staff.value()["name"];
        Utils::trim(name, "\"");
        string surname = name.substr(name.find(" ", 3)+1); // to compare with surname too

        if(!name.rfind(inputName, 0) || !surname.rfind(inputName, 0)){
            json value = staff.value();

            designation = value["designation"];
            mobile = value["mobile"];
            email = value["email"];

            cout << " | "  << setw(8)  << std::left << staff.key()
                 << " | " << setw(18) << name
                 << " | " << setw(15) << Utils::trim(designation, "\"")
                 << " | " << setw(9)  << to_string(value["branch_id"])
                 << " | " << setw(10) << Utils::trim(mobile, "\"")
                 << " | " << setw(30) << Utils::trim(email, "\"") << " |" << endl;
            recordsFound++;
        }
    }
    cout << " -------------------------------------------------------------------------------------------------------------" << endl
         << "\n => TOTAL RECORDS FOUND: " << recordsFound << endl;
}

void Admin::updateStaffDetails(const string &staffId){
    json data = Utils::readData();

    if(data["staff"].contains(staffId)){
        short choice;
        while(1){
            Staff::displayStaffDetails(staffId);
            cout << "\n ================ WHAT TO UPDATE ==================\n" << endl
                 << " 1) Name" << endl
                 << " 2) Salary" << endl
                 << " 3) Designation" << endl
                 << " 4) Branch id" << endl
                 << " 5) Address" << endl
                 << " 6) Password" << endl
                 << " 7) Mobile" << endl
                 << " 8) Email" << endl
                 << " 0) Cancle" << endl;

            Utils::scanNumber(choice, " Enter Choice: ");

            string fieldToUpdate;
            switch (choice) {
                case 0: return;
                case 1: fieldToUpdate = "name"; break;
                case 2: fieldToUpdate = "salary"; break;
                case 3: fieldToUpdate = "designation"; break;
                case 4: fieldToUpdate = "branch_id"; break;
                case 5: fieldToUpdate = "address"; break;
                case 6: fieldToUpdate = "password"; break;
                case 7: fieldToUpdate = "mobile"; break;
                case 8: fieldToUpdate = "email"; break;
            }

            if (choice == 2 || choice == 4){ // if value to update is numerical
                int newValue;
                Utils::scanNumber(newValue, " Enter New Value: ");
                Utils::updateData(data, data["staff"][staffId][fieldToUpdate], newValue);
            }
            else if (choice >= 1 && choice <= 8){ // if value to update is string
                string newValue;
                if (choice == 3) // if designation is selected for update, then get new value from function.
                    newValue = selectDesignation();
                else { // else get it normally
                    cout << " Enter New Value: ";
                    cin.ignore();
                    getline(cin, newValue);
                }

                // Validating inputs
                if( ( fieldToUpdate=="name" && !Utils::isNameValid(newValue) ) ||
                    ( fieldToUpdate=="password" && !Utils::isPasswordValid(newValue) ) ||
                    ( fieldToUpdate=="mobile" && !Utils::isMobileValid(newValue) ) ||
                    ( fieldToUpdate=="email" && !Utils::isEmailValid(newValue) )
                ){ return; }
                Utils::updateData(data, data["staff"][staffId][fieldToUpdate], newValue);
            }
            else { // invalid menu choice
                cout << "\n => ERROR: invalid choice entered..." << endl;
                getc(stdin);
                continue;
            }
        } // end of while(1)
    }
    else {
        cout << "\n => ERROR: STAFF NOT FOUND !" << endl;
        getc(stdin);
    }
}

void Admin::addStaff(){
    string name, address, email, mobile, designation, password;
    int salary, branch_id;

    cout << "\n========== ENTER DETAILS OF STAFF ==========" << endl;
    cin.ignore();

    cout << "\n Enter Name: ";
    getline(cin, name);
    cout << "\n Enter Address: ";
    getline(cin, address);
    cout << "\n Enter Mobile: ";
    getline(cin, mobile);
    cout << "\n Enter Email: ";
    getline(cin, email);
    designation = selectDesignation();

    Utils::scanNumber(branch_id, " Enter Branch id: ");
    Utils::scanNumber(salary, " Enter Salary: ");
    cin.ignore();

    cout << "\n Enter password for staff's account: ";
    getline(cin, password);

    // validating inputs
    if( Utils::isNameValid(name) &&
        Utils::isPasswordValid(password) &&
        Utils::isEmailValid(email) &&
        Utils::isMobileValid(mobile) &&
        Utils::isAddressValid(address))
    {
        // push the data into JSON file
        json data = Utils::readData();
        json newStaff = {
            {"address", address},
            {"branch_id", branch_id},
            {"designation", designation},
            {"email", email},
            {"mobile", mobile},
            {"name", name},
            {"password", password},
            {"salary", salary},
        };

        // creating staff-id for new staff member.
        int staffNumber = data["last_staff_number"];
        staffNumber++;

        string staffId = "bank"; // adding prefix 'bank' to userid, so it will look like 'bank0001'

        ostringstream staffIdWithLeadindZeors;
        if(staffNumber <= 9999){ // adding leading zeros to new staff id
            staffId += to_string(staffNumber);
            staffIdWithLeadindZeors << setw(4) << setfill('0') << to_string(staffNumber);
        }
        else {
            cout << "\n ERROR: STAFF LIMIT OUT OF BOUND" << endl;
            return;
        }

        data["staff"].push_back(json::object_t::value_type(staffIdWithLeadindZeors.str(), newStaff));
        data["last_staff_number"] = staffNumber;
        Utils::updateData(data);
        cout << "\n => NEW STAFF ADDED SUCCESSFULLY..." << endl;
        Staff::displayStaffDetails(staffId);
    }
}

void Admin::removeStaff(){
    string staffIdToRemove;
    cout << "\n======== REMOVE STAFF ========" << endl;
    cout << "\n Enter Staff Id To Remove: ";
    cin >> staffIdToRemove;

    // ---------- VARIFICATION ----------
    if (staffIdToRemove == this->id){
        cout << "\n ERROR: CAN'T REMOVE STAFF, THIS IS YOUR ID" << endl;
        getc(stdin);
        return;
    }
    else if (Admin::isAdmin(staffIdToRemove)){
        cout << "\n ERROR: CAN'T REMOVE STAFF, GIVEN STAFF ID AS AN ADMIN" << endl;
        getc(stdin);
        return;
    }

    json data = Utils::readData();

    if(data["staff"].contains(staffIdToRemove)){
        string password = "";
        short count = 0;
        Staff::displayStaffDetails(staffIdToRemove); // Displaying details of staff that is going to be removed

        // for password varification, looping is used here. Asking password 3 times if incorrect.
        while(1){
            cout << "\n Enter your password to delete staff: ";
            cin >> password;

            if(data["staff"][this->id]["password"] != password){
                if(count == 3){
                    cout << "\n ERROR: 3 times incorrect password is entered..." << endl;
                    break;
                }
                cout << "\n ERROR: INCORRECT PASSWORD !" << endl;
                count++;
                continue;
            }
            data["staff"].erase(staffIdToRemove);
            Utils::updateData(data);
            cout << "\n => STAFF IS DELETED SUCCESSFULLY..." << endl;
            break;
        }
    }
    else {
        cout << "\n => No Staff found with id \"" << staffIdToRemove << "\"" << endl;
    }
    getc(stdin);
}
// ======================================= END ADMIN ======================================



// ===================================== ACCOUNT =====================================
// ------- CONSTRUCTOR -------
Account::Account(long int accountNumber){
    try {
        json data = Utils::readData("account", to_string(accountNumber));
        if(!data.empty()){
            // if account found, initializing data from json file
            this->accountNumber = accountNumber;
            this->balance = data["balance"];
            this->accountHolderId = data["account_holder_id"];
            this->type = data["type"] == "s"? 's': 'c';
        } else {
            // if account NOT found
            // setting accountNumber = 0
            // so we can identify that this object is not valid since account not found.
            // have to use this method because constructor can't return any value from we can verify.
            this->accountNumber = 0;
            throw string("BANK ACCOUNT NOT FOUND WITH ACCOUNT-NUMBER \""+ to_string(accountNumber) + "\" !");
        }
    } catch (const string &error) {
        throw error; // rethrowing error to caller function
    }
}

// ------- STATIC METHODS -------
void Account::displayAccountDetails(const long &accountNumber){
    json data = Utils::readData("account", to_string(accountNumber));

    try {
        if (data.empty()){ // if account not found
            string error = "BANK ACCOUNT NOT FOUND WITH ACCOUNT-NUMBER \""+ to_string(accountNumber) + "\" !";
            throw error;
        }
        else {
            cout << endl << "\n =============== ACCOUNT DETAILS ===============\n" << endl;
            cout << " Account Number        : " << accountNumber << endl;
            cout << " Account Holder Number : " << data["account_holder_id"] << endl;
            cout << " Balance               : Rs." << data["balance"] << endl;
            cout << " Account Type          : " << data["type"] << endl;
        }
    } catch (const string &error) {
        throw error;
    }
}

Account Account::createNewAccount(const long int &accountHolderId, const long int &balance, const string &type){
    json data = Utils::readData();

    // getting count of accounts to get new account id
    long int accountNumber = data["account"].size()+1;

    // creating new json Account object
    json newAccount = {
        {"account_holder_id",accountHolderId},
        {"balance",balance},
        {"type",type}
    };

    // adding new object in the 'account' object
    data["account"] += json::object_t::value_type(to_string(accountNumber), newAccount);

    // writing data to json file
    Utils::updateData(data);

    return Account(accountNumber);
}

void Account::removeAccount(const long &accountNumber, const string &staffId){
    json data = Utils::readData();

    try {
        if(data["account"].contains(to_string(accountNumber))){ // if account found
            Account::displayAccountDetails(accountNumber);
            cout << "\n ===== ARE YOU SURE YOU WANT TO PERMENETALY DELETE THIS BANK ACCOUNT? =====" << endl;
            cout << "\n       0) NO"
                    "\n       1) YES";
            short choice;

            Utils::scanNumber(choice, "\n Enter choice: ");
            string password;

            switch (choice) {
                case 0: cout << "\n => Opration Canceled..." << endl;
                    return;
                case 1:
                    // password validation of staff, before removing account
                    cout << "\n Enter your password: ";
                    cin >> password;
                    if (password == data["staff"][staffId]["password"]){
                        short removeCount = data["account"].erase(to_string(accountNumber)); // removing object from json
                        if (removeCount == 1){
                            Utils::updateData(data);
                            cout << "\n => Account is REMOVED Successfully !" << endl;
                        }
                        else
                            throw string("BANK ACCOUNT IS NOT REMOVED, TRY AGAIN...");
                    }
                    else {
                        throw string("Invalid Password !");
                    }
                    break;
                default: cout << "\n => Please enter 0 or 1" << endl;
            }
        }
        else { // if account not found
            throw string("BANK ACCOUNT NOT FOUND WITH ACCOUNT-NUMBER \"" +to_string(accountNumber)+ "\"");
        }
    } catch (const string &error) {
        throw error;
    } catch (...) {
        throw string("EXCEPTION IN Account::removeAccount()");
    }

}

long int Account::getLastAccountNumber(){ return Utils::readData("last_account_number"); }

// ------- GETTER METHODS -------
long int Account::getAccountNumber(){ return this->accountNumber; }
long int Account::getBalance(){ return this->balance; }
long int Account::getAccountHolderId(){ return this->accountHolderId; }
string Account::getType(){ return this->type; }

// ------- Util METHODS -------
void Account::displayAccountDetails(){
    cout << endl << "\n =============== ACCOUNT DETAILS ===============\n" << endl;
    cout << "\tAccount Number: " << accountNumber << endl;
    cout << "\tAccount Holder: " << accountHolderId << endl;
    cout << "\t       Balance: Rs." << balance << endl;
    cout << "\t  Account Type: " << type << endl;
}

void Account::withdraw(const long int &amount, const std::string &staffId=""){
    if(this->balance < amount){
        cout << "\n => ERROR: Insufficient Balance !" << endl;
        cout << "\n => Available Balance: " << this->balance << endl;
        return;
    }
    try {
        // reading data from json file
        json data = Utils::readData();
        data["account"][to_string(accountNumber)]["balance"] = (this->balance - amount);

        // Wrtiting updated data into json file
        Utils::updateData(data);

        // writing log of the transaction
        string type = "withdraw";
        Utils::writeWithdrawDepositeLog(BANK_LOG_TYPES::WITHDRAW, this->accountNumber, amount, staffId);

        this->balance -= amount;
        cout << "\n => Rs." << amount << " is Debited from account_number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (const exception &e) {
        cout << " ERROR: " << e.what() << endl;
    }
}

void Account::deposit(const long int &amount, const std::string &staffId=""){
    try {
        // reading data from json file
        json data = Utils::readData();
        data["account"][to_string(accountNumber)]["balance"] = this->balance + amount;

        // Wrtiting updated data into json file
        Utils::updateData(data);
        string type = "deposit";
        Utils::writeWithdrawDepositeLog(BANK_LOG_TYPES::DEPOSIT, this->accountNumber, amount, staffId);

        this->balance += amount;
        cout << "\n => Rs." << amount << " is Credited to account_number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (const exception &e) {
        cout << " ERROR: " << e.what() << endl;
    }
}
// ======================================= END ACCOUNT ======================================



// ===================================== ACCOUNT_HOLDER =====================================
// ------- CONSTRUCTOR -------
AccountHolder::AccountHolder(const long int &accountHolderId){
    try {
        json accountHolder = Utils::readData("account_holder", to_string(accountHolderId));
        if(accountHolder.empty()){ // if account holder not found
            this->id = 0;
            throw string("NO ACCOUNT HOLDER FOUND WITH ID \""+to_string(accountHolderId)+"\" !");
        } else {
            // initializing data from json file
            this->id = accountHolderId;
            this->name = accountHolder["name"];
            this->mobile = accountHolder["mobile"];
            this->address = accountHolder["address"];
        }
    } catch (const string &error) {
        throw error;
    } catch (const std::exception &error) {
        throw error;
    }
}

// ------- STATIC METHODS -------
long int AccountHolder::createAccountHolder(const std::string &name, const std::string &mobile, const std::string &address){
    json data = Utils::readData();
    json newAccountHolder = {
        {"name", name},
        {"mobile", mobile},
        {"address", address}
    };

    try {
        long int accountHolderId = data["last_account_holder_number"];
        accountHolderId++;
        data["account_holder"].push_back(json::object_t::value_type(to_string(accountHolderId), newAccountHolder));
        Utils::updateData(data);
        return accountHolderId; // return the id of new account holder
    } catch (const json::exception &error) {
        cout << "ERROR: " << error.what() << endl;
    } catch (const std::exception &error) {
        cout << "ERROR: " << error.what() << endl;
    }
    return 0; // if account is not created
}

// ------- GETTER METHODS -------
long int AccountHolder::getId(){ return this->id; }
// =================================== END ACCOUNT_HOLDER ======================================
