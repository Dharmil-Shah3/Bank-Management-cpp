#include "admin.h"

using namespace std;
using namespace utils;
using namespace bankerror;

using json = nlohmann::json;

// ------- CONSTRUCTOR -------
Admin::Admin(const string &adminId, const string &password) : Staff(adminId, password)
{
    try {
        if (!isAdmin(adminId)){ //  if its not an admin
            throw ERROR_STAFF::NOT_AN_ADMIN;
        }
    } catch (const ERROR_STAFF &error) {
        throw error;
    } catch (const exception &error){
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
    catch (const exception & error) {
        cout << "\n ERROR: " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")";
        return NULL;
    }
}

bool Admin::isAdmin(const string &id)
{
    json admins = readData("admin");
    for(auto& admin: admins.items()){
        if(id == admin.value()) return true;
    }
    return false;
}

void Admin::displayStaffDetails(const std::string &staffId)
{
    json staff = readData("staff", staffId);
    if(!staff.empty()){
        Staff(staffId, staff["password"]).displayStaffDetails();
    } else {
        cout << "\n ERROR: NO STAFF MEMBER FOUND WITH ID \""<< staffId << "\"" << endl;
    }
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
        scanNumber(choice, " Enter Designation Choice: ");

        switch (choice) {
            case 1: return "Branch Manager";
            case 2: return "Cashier";
            case 3: return "Loan Officer";
            case 4: return "Security";
            case 5: return "PO";
            default: cout << "\n => Invalid choice enterd..." << endl;
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
    scanNumber(choice, " Enter choice: ");

    switch (choice) {
        case 1:
            cout << "\n Enter Staff id: ";
            cin.ignore();
            getline(cin, input);
            this->displayStaffDetails(input);
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

void Admin::displayWithdrawDepositLogs(const json &dateLogs, const string &month, const string &date)
{
    string transactionType, staffId;

    try {
        for(auto &transaction: dateLogs.items()){
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
                 << "| " << setw(2) << setfill('0') << month << "/" << setw(2) << date << " "
                 << "| " << setw(2) << to_string(value["time"]["hour"])
                 << ":" << setw(2) << to_string(value["time"]["minute"])
                 << ":" << setw(2) << to_string(value["time"]["second"]) << " |" << setfill(' ')<< endl;
        }
    } catch (const json::exception &error) {
        cout << "\n ERROR (JSON): " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    } catch (const exception &error) {
        cout << "\n ERROR: " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
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
        scanNumber(choice, " => Enter Choice: ");

        switch (choice) {
            case 0: system("clear"); return;
            case 1:
                Staff::displayStaffDetails();
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
            case 8: this->displayLogs(); break;
            default: cout << "\n => Invalid choice entered..." << endl;
        }
        getc(stdin);
        system("clear");
    }
}

void Admin::displayLogs()
{
    system("clear");
    unsigned short choice;
    cout << "\n ---------- SELECT LOG TYPE ----------\n" << endl
         << " 1) Withdraw-Deposit logs of Date" << endl
         << " 2) Withdraw-Deposit logs of Monnth" << endl
         << " 3) Withdraw-Deposit logs of Year" << endl;
    scanNumber(choice, " Enter choice: ");

    switch (choice) {
        case 1: this->displayWithdrawDepositLogsByDate();
            break;
        case 2: this->displayWithdrawDepositLogsByMonth();
            break;
        case 3: this->displayWithdrawDepositLogsByYear();
            break;
        default:
            cout << "\n => Invalid choice entered" << endl;
    }
}

void Admin::displayWithdrawDepositLogsByDate()
{
    json logs, tempLog = NULL;
    unsigned short year, month, date;

    system("clear");
    cout << "===== Date wise Withdraw & Deposite logs =====" << endl
         << "\n [Enter 0 for current date]" << endl;

    scanNumber(year ," Enter year: ");

    try {
    // if year == 0, means consider current data
    if(year == 0){
        //get the current date using time_t
        time_t now = time(0);
        tm *currentTime = localtime(&now);
        year = currentTime->tm_year + 1900;
        month = currentTime->tm_mon + 1;
        date = currentTime->tm_mday;
    } else { // get the specific month from user
        scanNumber(month ," Enter month: ");
        scanNumber(date ," Enter date: ");
    }

    // getchar() is called here to clear the input stream for kbhit() used in next looping statement.
    // kbhit() checks for the keypress to stop infinate loop

    getchar();

    system("clear");
    while(!kbhit()){ // showing logs until any key is pressed from keyboard

        logs = readLogs(year, month, date);

        if(logs != tempLog){ // display logs only if there is any change in logs
            system("clear");

            if(logs.empty()){
                cout << "\n => NO LOGS FOUND FOR "<<year<<"/"<<month<<"/"<<date<<" !" << endl;
                return;
            }
            cout << "\n--------------------------------- Logs of "
                 << year << "/" << setw(2) << setfill('0') << month << "/" << date << setfill(' ')
                 << " --------------------------------";
            cout << "\n| Trans_Id |   Desc   | Bank Account Id | Amount(Rs) | Staff id |  M/D  |   Time   |" << endl
                 << "-------------------------------------------------------------------------------------" << endl;

            this->displayWithdrawDepositLogs(logs, to_string(month), to_string(date)); // displaying logs

            cout << "-------------------------------------------------------------------------------------" << endl
                 << "\n=> Press any key to exit..." << endl;
        }

        tempLog = logs; // copiying value of logs to compare in the next iteration

        sleep(1); // sleep for 1 second
    } // end while
    } catch (const json::exception &error) {
        cout << "\n ERROR (JOSN): " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    } catch (const exception &error) {
        cout << "\n ERROR: " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    }
}

void Admin::displayWithdrawDepositLogsByMonth()
{
    json logs, tempLog = NULL;
    unsigned short year, month;

    // getting the year and month from user to display logs of that month
    system("clear");
    cout << "===== Monthly Withdraw & Deposite logs =====" << endl
         << "\n [ Enter 0 for current month ]" << endl;

    scanNumber(year ," Enter year: ");

    try {
    // if year == 0, means current year and month should be considered
    if(year == 0){
        //get the current year and month using time_t
        time_t now = time(0);
        tm *currentTime = localtime(&now);
        year = currentTime->tm_year + 1900;
        month = currentTime->tm_mon + 1;
    } else { // get the specific month from user
        scanNumber(month ," Enter month: ");
    }

    // getchar() is called here to clear the input stream for kbhit() used in next looping statement.
    // kbhit() checks for the keypress to stop infinate loop

    getchar();

    system("clear");

    while(!kbhit()){ // showing logs until any key is pressed from keyboard

        logs = readLogs(year, month);

        if(logs != tempLog){ // display logs only if there is any change in logs
            system("clear");

            if(logs.empty()){
                cout << "\n => NO LOGS FOUND FOR "<<year<<"/"<<month << endl;
                return;
            }
            cout << "\n--------------------------------- Logs of "
                 << year << "/" << setw(2) << setfill('0') << month << setfill(' ')
                 << " ----------------------------------";
            cout << "\n| Trans_Id |   Desc   | Bank Account Id | Amount(Rs) | Staff id |  M/D  |   Time   |" << endl
                 << "-------------------------------------------------------------------------------------" << endl;

            for(auto &date: logs.items()){ // displaying logs
                string day = date.key();
                this->displayWithdrawDepositLogs(date.value(), to_string(month), day); // displaying logs
            }

            cout << "-------------------------------------------------------------------------------------" << endl
                 << "\n=> Press any key to exit..." << endl;
        }

        tempLog = logs; // copiying value of logs to compare in the next iteration

        sleep(1); // sleep for 1 second
    } // end while
    } catch (const json::exception &error) {
        cout << "\n ERROR (JOSN): " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    } catch (const exception &error) {
        cout << "\n ERROR: " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    }
}

void Admin::displayWithdrawDepositLogsByYear()
{
    json logs, tempLog = NULL;
    unsigned short year;

    system("clear");
    cout << "===== Yearly Withdraw & Deposite logs =====" << endl
         << "\n [Enter 0 for current year]" << endl;

    scanNumber(year ," Enter year: ");

    try {
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    // if year == 0, means consider current year
    if(year == 0){
        //get the current year using time_t
        year = currentTime->tm_year + 1900;
    } else if (year < 2023 || year > (currentTime->tm_year+1900)){ // check for valid year value
        throw string("INVALID YEAR ENTERED. ENTER IN RANGE 2023-"+to_string((currentTime->tm_year+1900)));
    }

    // getchar() is called here to clear the input stream for kbhit() used in next looping statement.
    // kbhit() checks for the keypress to stop infinate loop

    getchar();

    system("clear");

    while(! kbhit()){ // showing logs until any key is pressed from keyboard

        logs = readLogs(year);
        if(logs != tempLog){ // display logs only if there is any change in logs
            system("clear");

            if(logs.empty()){
                cout << "\n => NO LOGS FOUND FOR " << year << " !" << endl;
                return;
            }
            cout << "\n--------------------------------- Logs of Year "<< year << " --------------------------------";
            cout << "\n| Trans_Id |   Desc   | Bank Account Id | Amount(Rs) | Staff id |  M/D  |   Time   |" << endl
                 << "------------------------------------------------------------------------------------" << endl;

            for(auto &monthLogs: logs.items()){
                string month = monthLogs.key();
                for(auto dateLogs: monthLogs.value().items()){
                    string date = dateLogs.key();
                    this->displayWithdrawDepositLogs(dateLogs.value(), month, date); // displaying logs
                }
            }

            cout << "------------------------------------------------------------------------------------" << endl
                 << "\n=> Press any key to exit...";
        }

        tempLog = logs; // copiying value of logs to compare in the next iteration

        sleep(1); // sleep for 1 second
    } // end while
    } catch(const string &error) {
        cout << "ERROR: " << error << endl;
    } catch (const json::exception &error) {
        cout << "\n ERROR (JOSN): " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    } catch (const exception &error) {
        cout << "\n ERROR: " << error.what() << " in " << __PRETTY_FUNCTION__ << " ("<<__FILE__<<")" << endl;
    }
}

void Admin::searchStaffDetailsByName(string &inputName)
{
    json allStaff = readData("staff");
    trim(inputName);
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
        trim(name, "\"");
        string surname = name.substr(name.find(" ", 3)+1); // to compare with surname too

        if(!name.rfind(inputName, 0) || !surname.rfind(inputName, 0)){
            json value = staff.value();

            designation = value["designation"];
            mobile = value["mobile"];
            email = value["email"];

            cout << " | "  << setw(8)  << std::left << staff.key()
                 << " | " << setw(18) << name
                 << " | " << setw(15) << trim(designation, "\"")
                 << " | " << setw(9)  << to_string(value["branch_id"])
                 << " | " << setw(10) << trim(mobile, "\"")
                 << " | " << setw(30) << trim(email, "\"") << " |" << endl;
            recordsFound++;
        }
    }
    cout << " -------------------------------------------------------------------------------------------------------------" << endl
         << "\n => TOTAL RECORDS FOUND: " << recordsFound << endl;
}

void Admin::updateStaffDetails(const string &staffId)
{
    json data = readData();

    if(data["staff"].contains(staffId)){
        short choice;
        while(1){
            displayStaffDetails(staffId);
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

            scanNumber(choice, " Enter Choice: ");

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
                scanNumber(newValue, " Enter New Value: ");
                updateData(data, data["staff"][staffId][fieldToUpdate], newValue);
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
                if( ( fieldToUpdate=="name"     && ! isNameValid(newValue) )     ||
                    ( fieldToUpdate=="password" && ! isPasswordValid(newValue) ) ||
                    ( fieldToUpdate=="mobile"   && ! isMobileValid(newValue) )   ||
                    ( fieldToUpdate=="email"    && ! isEmailValid(newValue) ))
                { return; }
                updateData(data, data["staff"][staffId][fieldToUpdate], newValue);
            }
            else { // invalid menu choice
                cout << "\n => Invalid choice entered..." << endl;
                getc(stdin);
                continue;
            }
        } // end of while(1)
    }
    else {
        cout << "\n ERROR: NO STAFF FOUND WITH ID \"" << staffId << "\" !" << endl;
        getc(stdin);
    }
}

void Admin::addStaff()
{
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

    scanNumber(branch_id, " Enter Branch id: ");
    scanNumber(salary, " Enter Salary: ");
    cin.ignore();

    cout << "\n Enter password for staff's account: ";
    getline(cin, password);

    // validating inputs
    if( isNameValid(name)         &&
        isPasswordValid(password) &&
        isEmailValid(email)       &&
        isMobileValid(mobile)     &&
        isAddressValid(address))
    {
        // push the data into JSON file
        json data = readData();
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
        updateData(data);

        cout << "\n => NEW STAFF ADDED SUCCESSFULLY..." << endl;
        displayStaffDetails(staffId);
    }
}

void Admin::removeStaff()
{
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

    json data = readData();

    if(data["staff"].contains(staffIdToRemove)){
        string password = "";
        short count = 0;
        displayStaffDetails(staffIdToRemove); // Displaying details of staff that is going to be removed

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
            updateData(data);
            cout << "\n => STAFF IS DELETED SUCCESSFULLY..." << endl;
            break;
        }
    }
    else {
        cout << "\n ERROR: NO STAFF FOUND WITH ID \"" << staffIdToRemove << "\"" << endl;
    }
    getc(stdin);
}

// ======================================= END ADMIN ======================================
