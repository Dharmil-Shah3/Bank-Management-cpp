#include "bankmanagement.h"

using namespace std;
using json = nlohmann::json;


// ===================================== Util =====================================
// ------- STATIC DATA -------
string Util::filename;
string Util::logFilename;

// ------- STATIC METHODS -------
Util::StaticConstructor Util::_staticConstructor;

string Util::getFileName(){ return filename; }
string Util::getLogFileName(){ return logFilename; }

template <typename T>
void Util::scanNumber(T &input, const string &message){
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
    input = abs(input);
}

void Util::trim(string &str){
    const char* typeOfWhitespaces = " \t\n\r\f\v";
    str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
    str.erase(0, str.find_first_not_of(typeOfWhitespaces));
}

bool Util::isNameValid(string& name){
    // converting string into lowercase
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    trim(name);
    regex pattern{"^[a-z]{3,15}( [a-z]{3,15})?$"};
    if(regex_search(name, pattern))
        return true;
    cout << "\n => ERROR: name must contain alphabets only"
         << "\n -> valid name examples: [ram], [ram charan]" << endl;
    getc(stdin);
    return false;
}
bool Util::isPasswordValid(string& password){
    trim(password);
    regex pattern{"^[a-zA-Z0-9!@#$_]{4,20}$"};
    if(regex_search(password, pattern))
        return true;
    cout << "\n => ERROR: invalid characters or length in password"
         << "\n -> valid characters are alphabets, numbers, !, @, #, $, _ (underscore)"
         << "\n -> length must be betweeen 4 to 20" << endl;
    getc(stdin);
    return false;
}
bool Util::isEmailValid(string& email){
    trim(email);
    transform(email.begin(), email.end(), email.begin(), ::tolower);
    regex pattern{"^[a-z][a-z0-9._]{2,20}@[a-z]{3,15}.[a-z]{2,4}$"};
    if(regex_search(email, pattern))
        return true;
    cout << "\n => ERROR: invalid email format" << endl;
    getc(stdin);
    return false;
}
bool Util::isMobileValid(string& mobile){
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
bool Util::isAddressValid(string& address){
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

json Util::readData(const string &key1="", const string &key2=""){
    ifstream fin(filename);
    json data = json::parse(fin);
    fin.close();

    if(key1 == "") return data;
    else if(key2 == "") return data[key1];
    else return data[key1][key2];
}

json Util::readLogs(const unsigned short &key1=0, const unsigned short &key2=0, const unsigned short &key3=0, const unsigned long int &key4=0){
    ifstream fin(logFilename);
    json data = json::parse(fin);
    fin.close();

    if(key1 == 0) return data;
    else if(key2 == 0) return data[to_string(key1)];
    else if(key3 == 0) return data[to_string(key1)][to_string(key2)];
    else if(key4 == 0) return data[to_string(key1)][to_string(key2)][to_string(key3)];
    else return data[to_string(key1)][to_string(key2)][to_string(key3)][to_string(key4)];
}

void Util::writeWithdrawDepositeLog(string &type, const long &bankAccountId, const long &amount, const std::string &staffId = ""){
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    if(type != "withdraw" && type != "deposit"){
        cout << "\n ERROR: transaction type is invalid. Valid values are \"withdraw\" & \"d\"" << endl;
        return;
    }

    time_t now = time(0);
    tm *currentTime = localtime(&now);

    string year = to_string(currentTime->tm_year + 1900);
    string month = to_string(currentTime->tm_mon + 1);
    string day = to_string(currentTime->tm_mday);
    short hour = currentTime->tm_hour;
    short minute = currentTime->tm_min;
    short second = currentTime->tm_sec;

    cout << "";

    json transaction = {
        {"type", type},
        {"account_number", bankAccountId},
        {"amount", amount},
        {"staff_id", staffId},
        {"time",{
             {"hour",hour},
             {"minute", minute},
             {"second", second}}
        }
    };

    cout << "\n\n TRANSACTION: " << setw(4) << transaction << endl;
    ifstream fin(logFilename);
    json data = json::parse(fin);
    fin.close();

    // read total transactions
    long int transactionId = data["total_transactions"];

    // update total transactions and get transaction id for current transaction
    data["total_transactions"] = ++transactionId;

    // update transactionId into json file quickly so other can get proper value of it
    ofstream fout(logFilename);
    fout << setw(4) << data << endl;
    fout.close();

    // adding new transaction in data
    data[year][month][day] += json::object_t::value_type(to_string(transactionId), transaction);

    // write the transatction object
    fout.open(logFilename);
    fout << setw(4) << data << endl;
    fout.close();
}

void Util::updateData(const nlohmann::json &data){
    ofstream fout(filename);
    fout << setw(4) << data << endl;
    fout.close();
}
template <typename T>
void Util::updateData(const json &data, json &dataToUpdate, const T &newValue){
    if(dataToUpdate != newValue){
        ofstream fout(filename);
        dataToUpdate = newValue;
        fout << setw(4) << data << endl;
        cout << "\n => Data is updated successfully !" << endl;
        fout.close();
    }
    else cout << "\n => ERROR: NO DATA UPDATED, NEW VALUE IS SIMILAR TO EXISTING !" << endl;
    getc(stdin);
}
// ======================================= END Util ======================================



// ===================================== STAFF =====================================
// ------------ CONSTRUCTOR --------------
Staff::Staff(string id){
    json staff = Util::readData("staff");

    if (staff.contains(id)){
        this->id = id;
        this->name = staff[id]["name"];
        this->designation = staff[id]["designation"];
        this->branch_id = staff[id]["branch_id"];
        this->mobile = staff[id]["mobile"];
        this->email = staff[id]["email"];
        this->address = staff[id]["address"];
        this->salary = staff[id]["salary"];
    } else {
        cout << "\n No Staff member found with id \""<< id << "\"" << endl;
        this->id = nullptr;
    }
}

// -------------- STATIC METHODS --------------
Staff* Staff::login(string userid, string password){
    json staff = Util::readData("staff");

    if(staff.contains(userid) && staff[userid]["password"] == password){
        return new Staff(userid);
    }
    return NULL;
}

// -------------- METHODS --------------
void Staff::displayPanel(){
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
        Util::scanNumber(choice, " => Enter Choice: ");

        switch (choice) {
            case 0: system("clear");
                return;
            case 1: this->displayAccountDetails();
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

void Staff::displayAccountDetails(){
    cout << "\n\n ================ DETAILS ================\n" << endl;
    cout << "          Id: " << this->id << endl;
    cout << "        Name: " << this->name << endl;
    cout << " Designation: " << this->designation << endl;
    cout << "   Branch id: " << this->branch_id << endl;
    cout << "      Mobile: " << this->mobile << endl;
    cout << "       Email: " << this->email << endl;
    cout << "     Address: " << this->  address << endl;
    cout << "      Salary: Rs." << this->salary << endl;
}

void Staff::updateAccountDetails(){
    this->displayAccountDetails();
    while(1){
        cout << "\n ===== SELECT WHAT TO UPDATE =====" << endl;
        cout << "\n 1) Name" <<endl;
        cout << " 2) Mobile" <<endl;
        cout << " 3) Email" <<endl;
        cout << " 4) Address" <<endl;
        cout << " 5) Password" <<endl;
        cout << " 0) Cancel" <<endl;

        short choice;
        Util::scanNumber(choice, " Enter Choice: ");

        string newValue, fieldToUpdate;
        json data = Util::readData();

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
        Util::trim(newValue);

        // validating inputs
        if(newValue == "" ||
           (fieldToUpdate=="name" && !Util::isNameValid(newValue)) ||
           (fieldToUpdate=="mobile" && !Util::isMobileValid(newValue)) ||
           (fieldToUpdate=="email" && !Util::isEmailValid(newValue)) ||
           (fieldToUpdate=="password" && !Util::isPasswordValid(newValue))
        ){
            continue;
        }

        // updating data into JSON file as well as into current object
        Util::updateData(data, data["staff"][this->id][fieldToUpdate], newValue);
        switch (choice) { // updating values in the current object
            case 1: this->name = newValue; break;
            case 2: this->mobile = newValue; break;
            case 3: this->email = newValue; break;
            case 4: this->address = newValue; break;
        }
    }
}

void Staff::displayBankAccountDetails(){
    long int accountNumber;
    Util::scanNumber(accountNumber, "\n => Enter account number: ");
    Account::displayAccountDetails(accountNumber);
}

void Staff::createBankAccount(){
    long int accountHolderId, balance;
    short choice;

    try {
        // Getting Account Holder Id
        while(1){
            Util::scanNumber(accountHolderId, "\n Enter account holder id: ");
            AccountHolder *accountHolder = new AccountHolder(accountHolderId);

            // if account holder is not found
            if(accountHolder->getId() == 0){
                while(1){
                    cout << "\n ============ Account Holder Not Found ============" << endl
                         << "\n 1) Add new Account Holder"
                         << "\n 2) Re-enter Account Holder Id "
                         << "\n 3) Cancel";
                    Util::scanNumber(choice, "\n Enter choice: ");

                    switch (choice) {
                        case 1: // add functionality
                            break;
                        case 2: continue;
                        case 3: return;
                        default: cout << "\n => ERROR: invalid choice entered..." << endl;
                    }
                }
            }
        }

        // Getting Account Type
        string accountType;
        while(1){
            cout << "\n => ACCOUNT TYPE <=" << endl
                 << "\n    1) Savings\n    2) Current";
            Util::scanNumber(choice, "\n Enter choice: ");

            if(choice == 1 || choice == 2){
                accountType = (choice == 1? "Saving": "Current");
                break;
            }
            else cout << "\n => ERROR: invalid choice entered..." << endl;
        }

        Util::scanNumber(balance, " Enter Initial balance: ");
        Account acc = Account::createNewAccount(accountHolderId, balance, accountType);
        cout << "\n => New Account is created...";
        acc.displayAccountDetails();
    }
    catch (exception e) {
        cout << "\n => ERROR: " << e.what() << endl;
    }
}

void Staff::removeBankAccount(){
    int account_number;
    Util::scanNumber(account_number, "\n Enter Account Number to Remove: ");
    Account::removeAccount(account_number, this->id);
}

void Staff::withdraw(){
    long int accountNumber, amount;
    Util::scanNumber(accountNumber, " => Enter account number: ");

    Account acc(accountNumber);
    try {
        if(acc.getAccountNumber() == 0) return;
        Util::scanNumber(amount, " => Enter amount to Withdraw: Rs.");
        acc.withdraw(amount ,this->id);
    } catch (exception e) {
        cout << "\n ERROR: " << e.what() << endl;
    }
}

void Staff::deposit(){
    long int accountNumber, amount;
    Util::scanNumber(accountNumber, " => Enter account number: ");

    Account acc(accountNumber);
    try {
        if(acc.getAccountNumber() == 0) return;
        Util::scanNumber(amount, " => Enter amount to Deposite: Rs.");
        acc.deposit(amount, this->id);
    } catch (exception e) {
        cout << "\n ERROR: " << e.what() << endl;
    }
}
// =================================== END STAFF ======================================



// ===================================== ADMIN =====================================
// ------- CONSTRUCTOR -------
Admin::Admin(string adminId) : Staff(adminId){
    // if no admin exist with this id, set id to nullptr
    if (!Admin::isAdmin(adminId)) {
        cout << "\n No Admin found with id \""<< adminId << "\"" << endl;
        this->id = nullptr;
    }
}

// ------- STATIC METHODS -------
Admin* Admin::login(string userid, string password){
    json staff = Util::readData("staff");

    if(Admin::isAdmin(userid) && staff[userid]["password"] == password)
        return new Admin(userid);
    else
        return NULL;
}

bool Admin::isAdmin(string &id){
    json admins = Util::readData("admin");

    for(auto& admin: admins.items()){
        if(id == admin.value()) return true;
    }
    return false;
}

// ------- PRIVATE METHODS -------
string Admin::selectDesignation(){
    short choice;
    while(1){
        cout << "\n ===== SELECT DESIGNATION =====" << endl
             << " 1) Branch Manager" << endl
             << " 2) Cashier" << endl
             << " 3) Loan officer" << endl
             << " 4) Security" << endl
             << " 5) PO" << endl;
        Util::scanNumber(choice, " Enter Designation Choice: ");
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

void Admin::searchStaff(){
    short choice;
    string input;
    cout << "\n ======= SEARCH STAFF BY =======\n" << endl;
    cout << " 1) Id"<<endl;
    cout << " 2) Name"<<endl;
    Util::scanNumber(choice, " Enter choice: ");

    switch (choice) {
        case 1:
            cout << "\n Enter Staff id: ";
            cin.ignore();
            getline(cin, input);
            this->searchStaffDetailsById(input);
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
void Admin::displayPanel(){
    short choice = 1;
    string input; // for scan staff id and name
    system("clear");

    while(choice != 0){
        cout << "\n ----------- ADMIN PANEL -----------\n" << endl;
        cout << " 1) Display Your Details " << endl;
        cout << " 2) Update Your Info" << endl;
        cout << " 3) Search Staff Details" << endl;
        cout << " 4) Update Staff Details" << endl;
        cout << " 5) Add New Staff" << endl;
        cout << " 6) Remove Staff" << endl;
        cout << " 7) Display Staff Menu" << endl;
        cout << " 8) Display Logs" << endl;
        cout << " 0) Logout" << endl;
        Util::scanNumber(choice, " => Enter Choice: ");

        switch (choice) {
            case 0: system("clear"); return;
            case 1:
                this->displayAccountDetails();
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

void Admin::displayLogsByMonth(){
    unsigned short year, month;
    cout << "===== Withdraw & Deposite logs by Month =====" << endl;
    Util::scanNumber(year ," => Enter year: ");
    Util::scanNumber(month ," => Enter month: ");

    json logs = Util::readLogs(year, month);

    if(logs.empty()){
        cout << "\n No logs found for "<<year<<"/"<<month << endl;
        getc(stdin);
        return;
    }

    system("clear");
    cout << "\n=-=-=-=-=-=-=-=-=-=-=-=-= Logs of "<<year<<"/"<<month<<" =-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "\n| Trans_Id |   Desc   | Bank Account Id | Amount(Rs) | Staff id | Date |   Time   |" << endl
         << "-----------------------------------------------------------------------------------" << endl;

    string day, type, staffId;

    for(auto &date: logs.items()){
        day = date.key();
        for(auto &transaction: date.value().items()){
            json value = transaction.value();
            type = to_string(value["type"]);
            type = type.substr(1, type.length()-2);
            staffId = to_string(transaction.value()["staff_id"]);

            staffId = staffId == "\"\""? "": staffId.substr(1,8);

            cout << "| " << setw(8) << transaction.key() << " "
                 << "| " << setw(8) << type << " "
                 << "| " << setw(15) << to_string(value["account_number"]) << " "
                 << "| " << setw(10) << to_string(value["amount"]) << " "
                 << "| " << setw(8) << staffId << " "
                 << "|  " << setw(2) << setfill('0') << day << "  "
                 << "| " << setw(2) << to_string(value["time"]["hour"])
                 << ":" << setw(2) << to_string(value["time"]["minute"])
                 << ":" << setw(2) << to_string(value["time"]["second"]) << " |" << setfill(' ')<< endl;
        }
    }
    cout << "-----------------------------------------------------------------------------------" << endl;
    getc(stdin);
}

void Admin::searchStaffDetailsById(string staffId) {
    json staff = Util::readData("staff");

    if (staff.contains(staffId)) Staff(staffId).displayAccountDetails();
    else cout << "\n => Staff with id \"" << staffId << "\" not found !" << endl;
}

void Admin::searchStaffDetailsByName(string staffName){
    json allStaff = Util::readData("staff");
    Util::trim(staffName);
    transform(staffName.begin(), staffName.end(), staffName.begin(), ::tolower);
    short recordsFound = 0;

    for(auto &staff: allStaff.items()){
        string staffName2 = staff.value()["name"];
        transform(staffName2.begin(), staffName2.end(), staffName2.begin(), ::tolower);
        if(staffName == staffName2){
            Staff temp = Staff(staff.key());
            temp.displayAccountDetails();
            recordsFound++;
        }
    }
    cout << "\n===========================================" << endl
         << " => Total Records Found: " << recordsFound << endl
         << "===========================================" << endl;
}

void Admin::updateStaffDetails(string staffId){
    json data = Util::readData();

    if(data["staff"].contains(staffId)){
        short choice;
        while(1){
            Staff(staffId).displayAccountDetails();
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

            Util::scanNumber(choice, " Enter Choice: ");

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
                Util::scanNumber(newValue, " Enter New Value: ");
                Util::updateData(data, data["staff"][staffId][fieldToUpdate], newValue);
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
                if( ( fieldToUpdate=="name" && !Util::isNameValid(newValue) ) ||
                    ( fieldToUpdate=="password" && !Util::isPasswordValid(newValue) ) ||
                    ( fieldToUpdate=="mobile" && !Util::isMobileValid(newValue) ) ||
                    ( fieldToUpdate=="email" && !Util::isEmailValid(newValue) )
                ){ return; }
                Util::updateData(data, data["staff"][staffId][fieldToUpdate], newValue);
            }
            else { // invalid menu choice
                cout << "\n => ERROR: invalid choice entered..." << endl;
                getc(stdin);
                continue;
            }
        } // end of while(1)
    }
    else {
        cout << "\n => ERROR: Staff Not Found !" << endl;
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
    Util::scanNumber(branch_id, " Enter Branch id: ");
    Util::scanNumber(salary, " Enter Salary: ");
    cin.ignore();
    cout << "\n Enter password for staff's account: ";
    getline(cin, password);

    // validating inputs
    if( Util::isNameValid(name) &&
        Util::isPasswordValid(password) &&
        Util::isEmailValid(email) &&
        Util::isMobileValid(mobile) &&
        Util::isAddressValid(address))
    {   // push the data into JSON file
        json data = Util::readData();
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
        if(staffNumber <= 9) staffId += "000" + to_string(staffNumber);
        else if(staffNumber <= 99) staffId += "00" + to_string(staffNumber);
        else if(staffNumber <= 999) staffId += "0" + to_string(staffNumber);
        else if(staffNumber <= 9999) staffId += to_string(staffNumber);
        else {
            cout << "\n => Staff limit out of bound...!" << endl;
            return;
        }

        data["staff"].push_back(json::object_t::value_type(staffId, newStaff));
        data["last_staff_number"] = staffNumber;
        Util::updateData(data);
        cout << "\n => New staff data added successfully..." << endl;
        Staff new_staff = Staff(staffId);
        new_staff.displayAccountDetails();
    }
}

void Admin::removeStaff(){
    string staffIdToRemove;
    cout << "\n======== REMOVE STAFF ========" << endl;
    cout << "\n Enter Staff Id To Remove: ";
    cin >> staffIdToRemove;

    // VARIFICATION
    if (staffIdToRemove == this->id){
        cout << "\n => ERROR: CAN NOT REMOVE STAFF, THIS IS YOUR ID" << endl;
        getc(stdin);
        return;
    }
    else if (Admin::isAdmin(staffIdToRemove)){
        cout << "\n => ERROR: CAN NOT REMOVE STAFF, GIVEN STAFF ID AS AN ADMIN" << endl;
        getc(stdin);
        return;
    }

    json data = Util::readData();

    if(data["staff"].contains(staffIdToRemove)){
        string password = "";
        short count = 0;
        Staff(staffIdToRemove).displayAccountDetails(); // Displaying details of staff that is going to be removed

        // for password varification, looping is used here. Asking password 3 times if incorrect.
        while(1){
            cout << "\n Enter your password to delete staff: ";
            cin >> password;

            if(data["staff"][this->id]["password"] != password){
                cout << "\n => Incorrect password..." << endl;
                count++;
                if(count == 3){
                    cout << "\n ERROR: 3 times incorrect password is entered..." << endl;
                    break;
                }
                continue;
            }
            data["staff"].erase(staffIdToRemove);
            Util::updateData(data);
            cout << "\n Staff is deleted successfully !" << endl;
            break;
        }
    }
    else cout << "\n => No Staff found with id \"" << staffIdToRemove << "\"" << endl;
    getc(stdin);
}
// ======================================= END ADMIN ======================================



// ===================================== ACCOUNT =====================================
// ------- CONSTRUCTOR -------
Account::Account(long int accountNumber){
    json data = Util::readData("account", to_string(accountNumber));

    if(!data.empty()){
        // initializing data from json file
        this->accountNumber = accountNumber;
        this->balance = data["balance"];
        this->accountHolderId = data["account_holder_id"];
        this->type = data["type"] == "s"? 's': 'c';
    } else {
        // display error messsage and delete current object if account number not found.
        this->accountNumber = 0;
        cout << "\n ERROR: No account found with id \"" << accountNumber << "\" !" << endl;
    }
}

// ------- STATIC METHODS -------
void Account::displayAccountDetails(const long &accountNumber){
    json data = Util::readData("account", to_string(accountNumber));

    if (!data.empty()){
        cout << endl << "\n =============== ACCOUNT DETAILS ===============\n" << endl;
        cout << "        Account Number: " << accountNumber << endl;
        cout << " Account Holder Number: " << data["account_holder_id"] << endl;
        cout << "               Balance: Rs." << data["balance"] << endl;
        cout << "          Account Type: " << data["type"] << endl;
    }
    else cout << "\n => ERROR: No account found with account_number \"" << accountNumber << "\" !" << endl;
}

Account Account::createNewAccount(const long int &accountHolderId, const long int &balance, const string &type){
    json data = Util::readData();

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
    Util::updateData(data);

    return Account(accountNumber);
}

void Account::removeAccount(const long &accountNumber, const string &staffId){
    json data = Util::readData();

    if(! data["account"].contains(to_string(accountNumber))){
        cout << "\n ERROR: Account NOT found with account_number " << accountNumber << endl;
    } else {
        Account::displayAccountDetails(accountNumber);
        cout << "\n ===== ARE YOU SURE YOU WANT TO PERMENETALY DELETE THIS BANK ACCOUNT? =====" << endl;
        cout << "\n       0) NO\n       1) YES";
        short choice;

        Util::scanNumber(choice, "\n Enter choice: ");
        string password;

        switch (choice) {
            case 0: cout << "\n => Opration Canceled..." << endl;
                return;
            case 1:
                cout << "\n Enter your password: ";
                cin >> password;
                if (password == data["staff"][staffId]["password"]){
                    short removeCount = data["account"].erase(to_string(accountNumber)); // removing object from json
                    if (removeCount == 1){
                        Util::updateData(data);
                        cout << "\n => Account is REMOVED Successfully !" << endl;
                    }
                    else cout << "\n => Account is NOT REMOVED" << endl;
                }
                else {
                    cout << "\n => Invalid Password !" << endl;
                }
                break;
            default: cout << "\n => Please enter 0 or 1" << endl;
        }
    }
}

long int Account::getLastAccountNumber(){ return Util::readData("last_account_number"); }

// ------- GETTER METHODS -------
long int Account::getAccountNumber(){ return this->accountNumber; }
long int Account::getBalance(){ return this->balance; }
long int Account::getAccountHolderId(){ return this->accountHolderId; }
string Account::getType(){ return this->type; }

// ------- UTIL METHODS -------
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
        json data = Util::readData();
        data["account"][to_string(accountNumber)]["balance"] = (this->balance - amount);

        // Wrtiting updated data into json file
        Util::updateData(data);

        // writing log of the transaction
        string type = "withdraw";
        Util::writeWithdrawDepositeLog(type, this->accountNumber, amount, staffId);

        this->balance -= amount;
        cout << "\n => Rs." << amount << " is Debited from account_number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (exception e) {
        cout << " ERROR: " << e.what() << endl;
    }
}

void Account::deposit(const long int &amount, const std::string &staffId=""){
    try {
        // reading data from json file
        json data = Util::readData();
        data["account"][to_string(accountNumber)]["balance"] = this->balance + amount;

        // Wrtiting updated data into json file
        Util::updateData(data);
        string type = "deposit";
        Util::writeWithdrawDepositeLog(type, this->accountNumber, amount, staffId);

        this->balance += amount;
        cout << "\n => Rs." << amount << " is Credited to account_number " << this->accountNumber << endl
             << " => Current Available balance is Rs." << this->balance << endl;
    } catch (exception e) {
        cout << " ERROR: " << e.what() << endl;
    }
}
// ======================================= END ACCOUNT ======================================



// ===================================== ACCOUNT_HOLDER =====================================
// ------- CONSTRUCTOR -------
AccountHolder::AccountHolder(const long int &accountHolderId){
    json accountHolder = Util::readData("account_holder", to_string(accountHolderId));

    if(!accountHolder.empty()){
        // initializing data from json file
        this->id = accountHolderId;
        this->name = accountHolder["name"];
        this->mobile = accountHolder["mobile"];
        this->address = accountHolder["address"];
    } else {
        // display error messsage and delete current object if account number not found.
        this->id = 0;
        cout << "\n ERROR: No Account-Holder found with id " << accountHolderId << " !" << endl;
    }
}

// ------- STATIC METHODS -------
void AccountHolder::createAccountHolder(const std::string &name, const std::string &mobile, const std::string &address){}
long int AccountHolder::getLastAccountHolderNumber(){ return Util::readData("last_account_holder_number"); }

// ------- GETTER METHODS -------
long int AccountHolder::getId(){ return this->id; }
// =================================== END ACCOUNT_HOLDER ======================================
