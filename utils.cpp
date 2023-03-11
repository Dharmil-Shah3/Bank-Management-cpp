#include "utils.h"

using namespace std;
using json = nlohmann::json;

const char* utils::trim(string &str, const string &characterstoRemove)
{
    str.erase(str.find_last_not_of(characterstoRemove) + 1);
    str.erase(0, str.find_first_not_of(characterstoRemove));
    return str.c_str();
}

string utils::padLeft(std::string &str, const unsigned int &totalLength, const char &paddingCharacter)
{
    if(totalLength > str.size())
        str.insert(0, totalLength - str.size(), paddingCharacter);
    return str;
}

void utils::displayCustomErrorMessage(const string &functionName,
                                      const string &fileName,
                                      const string &message)
{
    cerr << "\n ERROR: " << message << endl
         << "\t- in function -> " << functionName << endl
         << "\t- in file     -> " << fileName << endl;
}

bool utils::isNameValid(string& name)
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
bool utils::isPasswordValid(string& password)
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
bool utils::isEmailValid(string& email)
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
bool utils::isMobileValid(string& mobile)
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
bool utils::isAddressValid(string& address)
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

int utils::kbhit(void)
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

json utils::readData(const string &key1, const string &key2)
{
    try {
        ifstream fin(_dataFilename_);
        if(!fin || fin.fail()){ // if file is not opened
            string error = "COULDN'T OPEN FILE " + _dataFilename_ + " TO READ,"
                         + " ON LINE NUMBER " + to_string(__LINE__-3);
            throw error;
        }

        json data = json::parse(fin);
        fin.close();

        if(key1 == "") return data;
        else if(key2 == "") return data[key1];
        else return data[key1][key2];

    } catch (const string &error) {
        cerr << "\n ERROR: " << error << endl;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    return NULL;
}

json utils::readLogs(const unsigned short &key1, const unsigned short &key2,
                     const unsigned short &key3, const unsigned long int &key4)
{
    try {
        ifstream fin(_withdrawDepositLogsFilename_);
        if(!fin){ // if file is not opened
            string error = "COULDN'T OPEN FILE " + _withdrawDepositLogsFilename_ + " TO READ,"
                         + " ON LINE NUMBER " + to_string(__LINE__-3);
            throw error;
        }
        json data = json::parse(fin);
        fin.close();

        if(key1 == 0) return data;
        else if(key2 == 0) return data[to_string(key1)];
        else if(key3 == 0) return data[to_string(key1)][to_string(key2)];
        else if(key4 == 0) return data[to_string(key1)][to_string(key2)][to_string(key3)];
        else return data[to_string(key1)][to_string(key2)][to_string(key3)][to_string(key4)];
    }
    catch (const string &error) {
        cerr << "\n ERROR: " << error << endl;
    }
    catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    }
    catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    return NULL;
}

void utils::writeWithdrawDepositeLog(const BANK_LOG_TYPES &type,
                                     const long &bankAccountId,
                                     const long &amount,
                                     const std::string &staffId)
{
    try {
        if(type != WITHDRAW && type != DEPOSIT){
            throw string("INVALID LOG TYPE PASSED AS PARAMETER");
        }

        // variable declarations
        json transaction, data;
        ofstream fout;
        ifstream fin;
        unsigned int transactionId;
        unsigned short hour, minute, second;
        string year, month, day, transactionIdWithLeadindZeors;
        time_t now;
        tm *currentTime;

        now         = time(0);
        currentTime = localtime(&now);

        year   = currentTime->tm_year + 1900;
        month  = currentTime->tm_mon + 1;
        day    = currentTime->tm_mday;
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

        transactionId = 0;

        fin.open(_withdrawDepositLogsFilename_);
        data = json::parse(fin);
        fin.close();

        if(! data.contains("total_transactions")){ // if "total_transactions" not found in the file
            data["total_transactions"] = 0;
        }

        transactionId = data["total_transactions"];

        // update total transactions and get transaction id for current transaction
        data["total_transactions"] = ++transactionId;

        // update transactionId into json file quickly so other can get proper value of it
        fout.open(_withdrawDepositLogsFilename_);
        fout << setw(4) << data << endl;
        fout.close();

        // making treansaction id with leading 0s.
        transactionIdWithLeadindZeors = to_string(transactionId);
        padLeft(transactionIdWithLeadindZeors, 8, '0');

        // adding new transaction in data
        data[year][month][day].push_back(json::object_t::value_type(transactionIdWithLeadindZeors, transaction));

        // write the transatction object
        fout.open(_withdrawDepositLogsFilename_);
        fout << setw(4) << data << endl;
        fout.close();
    }
    catch (const string &error){
        cerr << "\n ERROR: " << error << endl;
    } catch (const exception &error) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
}

void utils::updateData(const nlohmann::json &data)
{
    ofstream fout;
    try {
        fout.open(_dataFilename_);
        fout << setw(4) << data << flush;
        if(fout.fail()){
            string error = "FILE OPERATION FAILED ! DATA IS NOT UPDATED SUCCESSFULLY..."
                           "\n ON LINE NUMBER " + to_string(__LINE__-1);
            throw error;
        } else {
            cout << "\n => Data is updated successfully..." << endl;
        }
    } catch (const string &error){ // if any error in file operation
        cerr << "\n ERROR: " << error << endl;
    } catch (const exception &error){
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__, error.what());
    } catch (...) {
        displayCustomErrorMessage(__PRETTY_FUNCTION__, __FILE__);
    }
    fout.close();
    getc(stdin);
}
