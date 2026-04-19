#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

//Teller ID, Full Name, Password, Branch Code

struct TellerRecord {
    char id[10];
    char name[50];
    size_t password_hash;
    char branch_code[10];
};

struct BranchRecord {
    char code[10];
    char name[50];
    char address[100];
    double total_balance;
    int customer_count;
};

struct CustomerRecord {
    char account_number[30];
    char full_name[100];
    char sa_id[14];
    char contact[11];
    char email[100];
    char address[200];
    char dob[11];
    int account_type;  // 1=Savings,2=Cheque,3=Fixed,4=Student
    double balance;
    char branch_code[10];
    char encrypted_pin[10];
    int failed_attempts;
    bool locked;
};

struct TransactionRecord {
    char account_number[30];
    char type[20];
    double amount;
    double new_balance;
    char date[30];
};

// Base Class: Account

class Account{
    public:
        string account_number;
        string full_name;
        string sa_id;
        string contact;
        string email;
        string address;
        string dob;
        int account_type;
        double balance;
        string branch_code;
        string pin;
        int failed_attempts;
        bool locked;

        virtual double get_min_deposit() = 0;

        virtual double get_interest_rate(){
            return 0.0;
        }
};

class savings_acc : public Account{
    public:
        double get_min_deposit() override{
            return 100.0;
        }
        double get_interest_rate() override{
            return 2.5;
        }

};

class cheque_acc : public Account{
    public:
        double get_min_deposit() override{
            return 500.0;
        }

};

class fixed_deposit_acc : public Account{
    public:
        double get_min_deposit() override{
            return 1000.0;
        }
        double get_interest_rate() override{
            return 5.0;
        }

};

class student_acc : public Account{
    public:
        double get_min_deposit() override{
            return 50.0;
        }

};

// Simple encryption (XOR)
string simple_encrypt(string input) {
    for(char &c : input) {
        c = c ^ 0x55;
    }
    return input;
}

// Validation functions

bool validate_said(string id){
    return id.length() == 13 && all_of(id.begin(), id.end(), ::isdigit);
}

bool validate_phone(string phone){
    return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
}

bool validate_email(string email){
    return email.find("@") != string::npos &&
           email.find(".") != string::npos;
}

// Auto generation

string gen_acc_numb(string branch_code){
    static int counter = 1000;
    counter++;
    return "ACC-" + branch_code + "-" + to_string(counter);
}

string gen_pin(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(10000, 99999);
    return to_string(dist(gen));
}

// Initialize system

void init_system() {
    ifstream check("system_config.txt");
    if(!check.is_open()) {
        ofstream config("system_config.txt");
        config << "MAX_LOGIN_ATTEMPTS=3\n";
        config << "SAVINGS_INTEREST=2.5\n";
        config << "FIXED_INTEREST=5.0\n";
        config.close();
    }
    check.close();
    
    ifstream check_branches("branches.dat", ios::binary);
    if(!check_branches.is_open()) {
        ofstream branch_file("branches.dat", ios::binary);
        BranchRecord branches[3] = {
            {"B001", "Johannesburg Main", "123 Main Street, JHB", 0, 0},
            {"B002", "Cape Town Branch", "45 Long Street, CPT", 0, 0},
            {"B003", "Durban Branch", "78 Beach Road, DBN", 0, 0}
        };
        branch_file.write((char*)branches, sizeof(branches));
        branch_file.close();
    }
    check_branches.close();
}

// Branch functions

vector<BranchRecord> load_branches() {
    vector<BranchRecord> branches;
    ifstream file("branches.dat", ios::binary);
    if(file.is_open()) {
        BranchRecord b;
        while(file.read((char*)&b, sizeof(b))) {
            branches.push_back(b);
        }
        file.close();
    }
    return branches;
}

void save_branches(vector<BranchRecord> &branches) {
    ofstream file("branches.dat", ios::binary);
    for(int i = 0; i < branches.size(); i++) {
        file.write((char*)&branches[i], sizeof(branches[i]));
    }
    file.close();
}

void view_all_branches() {
    vector<BranchRecord> branches = load_branches();
    
    cout << "\n=== ALL BRANCHES ===\n";
    for(int i = 0; i < branches.size(); i++) {
        cout << "Code: " << branches[i].code << "\n";
        cout << "Name: " << branches[i].name << "\n";
        cout << "Address: " << branches[i].address << "\n";
        cout << "Total Balance: R" << branches[i].total_balance << "\n";
        cout << "Customers: " << branches[i].customer_count << "\n";
        cout << "------------------------\n";
    }
}

void inter_branch_comparison() {
    vector<BranchRecord> branches = load_branches();
    
    cout << "\n=== INTER-BRANCH COMPARISON ===\n";
    
    int best = 0;
    int worst = 0;
    
    for(int i = 1; i < branches.size(); i++) {
        if(branches[i].total_balance > branches[best].total_balance) {
            best = i;
        }
        if(branches[i].total_balance < branches[worst].total_balance) {
            worst = i;
        }
    }
    
    cout << "Best Branch: " << branches[best].name << " (R" << branches[best].total_balance << ")\n";
    cout << "Worst Branch: " << branches[worst].name << " (R" << branches[worst].total_balance << ")\n";
    cout << "Difference: R" << (branches[best].total_balance - branches[worst].total_balance) << "\n";
}

void update_branch_stats(string branch_code, double amount, bool new_customer) {
    vector<BranchRecord> branches = load_branches();
    
    for(int i = 0; i < branches.size(); i++) {
        if(branch_code == branches[i].code) {
            branches[i].total_balance += amount;
            if(new_customer) {
                branches[i].customer_count++;
            }
            break;
        }
    }
    
    save_branches(branches);
}

// Teller functions

void create_teller() {

    ofstream out_file("tellers.dat", ios::binary | ios::app);

    TellerRecord my_obj;

    cout << "Enter Teller ID: " << "\n";

    cin >> my_obj.id;

    cout << "Enter Teller Full Name: " << "\n";

    cin.ignore();

    cin.getline(my_obj.name, 50);

    string password;
    cout << "Create Teller Password: " << "\n";

    cin >> password;

    string salt = "XyZ123";

    string salted = password + salt;

    hash<string> pass_hash;    

    size_t hashed_password = pass_hash(salted);

    my_obj.password_hash = hashed_password;

    cout << "Enter Branch Code: " << "\n";

    cin >> my_obj.branch_code;

        if (out_file.is_open()){

            out_file.write((char*)&my_obj, sizeof(my_obj));


        }
        out_file.close();

    cout << "Teller created!\n";
}

bool login_teller(string &branch_code) {
    string identify;
    string pass_word;

    cout << "Please Enter ID & Password Below: " << "\n";

    cout << "ID Number:" << "\n";

    cin >> identify;

    cout << "Password:" << "\n";

    cin >> pass_word;

    string salt = "XyZ123";

    string salted = pass_word + salt;

    hash<string> hash_pass;
    size_t hashed_password = hash_pass(salted);

    ifstream file("tellers.dat", ios::binary);

    if(!file.is_open()) {
        cout << "No tellers found\n";
        return false;
    }

    bool found = false;

    TellerRecord teller;

    while (file.read((char*)&teller, sizeof(teller))){

        if (identify == teller.id && hashed_password == teller.password_hash){
            found = true;
            branch_code = teller.branch_code;
            cout << "Login Successful! Branch: " << branch_code << "\n";
            break;
        }
    }
    file.close();

    if (!found){
        cout << "Invalid Id or Password" << "\n";
    }
    
    return found;
}

// Customer registration

void register_cust(string branch_code){

    Account * acc = nullptr;

    int type;
    cout << "Select Account Type: " << "\n";
    cout << "1. Savings" << "\n";
    cout << "2. Cheque" << "\n";
    cout << "3. Fixed Deposit" << "\n";
    cout << "4. Student" << "\n";

    cin >> type;

    if(type == 1) acc = new savings_acc();
    else if(type == 2) acc = new cheque_acc();
    else if(type == 3) acc = new fixed_deposit_acc();
    else if(type == 4) acc = new student_acc();
    else {
        cout << "Invalid type\n";
        return;
    }

    cin.ignore();

    cout << "Full Name: " << "\n";
    getline(cin, acc -> full_name);

    do{
        cout << "SA ID (13 digits long): " << "\n";
        cin >> acc -> sa_id;
    }while(!validate_said(acc -> sa_id));

    do{
        cout << "Contact Number (10 digits): " << "\n";
        cin >> acc -> contact;
    }while(!validate_phone(acc -> contact));

    do{
        cout << "Email Address: " << "\n";
        cin >> acc -> email;
    }while(!validate_email(acc -> email));

    cin.ignore();
    cout << "Physical Address: " << "\n";
    getline(cin, acc -> address);

    cout << "Date of Birth (DD/MM/YYYY): " << "\n";
    cin >> acc -> dob;

    acc -> branch_code = branch_code;
    acc -> account_number = gen_acc_numb(branch_code);
    acc -> account_type = type;

    double min_deposit = acc -> get_min_deposit();

    do{
        cout << "Initial Deposit (Min: " << min_deposit << "): ";
        cin >> acc -> balance;
    }while(acc -> balance < min_deposit);

    acc -> pin = gen_pin();
    acc -> failed_attempts = 0;
    acc -> locked = false;

    cout << "\n=== CUSTOMER CREATED ===\n";
    cout << "Account Number: " << acc -> account_number << "\n";
    cout << "Generated PIN: " << acc -> pin << "\n";
    cout << "PLEASE SAVE THIS PIN!\n";

    // Save to binary file
    ofstream file("customers.dat", ios::binary | ios::app);
    
    CustomerRecord record;
    strcpy(record.account_number, acc->account_number.c_str());
    strcpy(record.full_name, acc->full_name.c_str());
    strcpy(record.sa_id, acc->sa_id.c_str());
    strcpy(record.contact, acc->contact.c_str());
    strcpy(record.email, acc->email.c_str());
    strcpy(record.address, acc->address.c_str());
    strcpy(record.dob, acc->dob.c_str());
    record.account_type = acc->account_type;
    record.balance = acc->balance;
    strcpy(record.branch_code, acc->branch_code.c_str());
    string encrypted = simple_encrypt(acc->pin);
    strcpy(record.encrypted_pin, encrypted.c_str());
    record.failed_attempts = 0;
    record.locked = false;
    
    file.write((char*)&record, sizeof(record));
    file.close();
    
    update_branch_stats(branch_code, acc->balance, true);

    delete acc;
}

// Customer find and update

bool find_customer(string acc_num, CustomerRecord &record) {
    ifstream file("customers.dat", ios::binary);
    if(!file.is_open()) return false;
    
    while(file.read((char*)&record, sizeof(record))) {
        if(acc_num == record.account_number) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void update_customer(CustomerRecord &record) {
    ifstream infile("customers.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);
    
    CustomerRecord r;
    while(infile.read((char*)&r, sizeof(r))) {
        if(string(r.account_number) == string(record.account_number)) {
            temp.write((char*)&record, sizeof(record));
        } else {
            temp.write((char*)&r, sizeof(r));
        }
    }
    infile.close();
    temp.close();
    remove("customers.dat");
    rename("temp.dat", "customers.dat");
}

bool verify_customer_pin(string acc_num, string entered_pin) {
    CustomerRecord record;
    if(!find_customer(acc_num, record)) return false;
    
    if(record.locked) {
        cout << "Account is LOCKED! Contact branch manager.\n";
        return false;
    }
    
    string decrypted = simple_encrypt(record.encrypted_pin);
    
    if(entered_pin == decrypted) {
        record.failed_attempts = 0;
        update_customer(record);
        return true;
    } else {
        record.failed_attempts++;
        if(record.failed_attempts >= 3) {
            record.locked = true;
            cout << "TOO MANY FAILED ATTEMPTS! ACCOUNT LOCKED!\n";
        }
        update_customer(record);
        return false;
    }
}

// Transaction functions

void log_transaction(string acc_num, string type, double amount, double balance) {
    ofstream file("transactions.dat", ios::binary | ios::app);
    TransactionRecord trans;
    
    strcpy(trans.account_number, acc_num.c_str());
    strcpy(trans.type, type.c_str());
    trans.amount = amount;
    trans.new_balance = balance;
    
    time_t now = time(0);
    strcpy(trans.date, ctime(&now));
    trans.date[strlen(trans.date) - 1] = '\0';
    
    file.write((char*)&trans, sizeof(trans));
    file.close();
}

void deposit(CustomerRecord &record) {
    double amount;
    cout << "Enter deposit amount: R";
    cin >> amount;
    
    try {
        if(amount <= 0) {
            throw runtime_error("Invalid amount!");
        }
        
        record.balance += amount;
        update_customer(record);
        log_transaction(record.account_number, "DEPOSIT", amount, record.balance);
        update_branch_stats(record.branch_code, amount, false);
        
        cout << "Deposit successful! New balance: R" << record.balance << "\n";
    }
    catch(const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void withdraw(CustomerRecord &record) {
    double amount;
    cout << "Enter withdrawal amount: R";
    cin >> amount;
    
    try {
        if(amount <= 0) {
            throw runtime_error("Invalid amount!");
        }
        if(amount > record.balance) {
            throw runtime_error("Insufficient funds!");
        }
        
        record.balance -= amount;
        update_customer(record);
        log_transaction(record.account_number, "WITHDRAWAL", amount, record.balance);
        update_branch_stats(record.branch_code, -amount, false);
        
        cout << "Withdrawal successful! New balance: R" << record.balance << "\n";
    }
    catch(const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void transfer(CustomerRecord &from) {
    string to_account;
    double amount;
    
    cout << "Enter destination account number: ";
    cin >> to_account;
    
    CustomerRecord to;
    if(!find_customer(to_account, to)) {
        cout << "Destination account not found!\n";
        return;
    }
    
    cout << "Enter amount to transfer: R";
    cin >> amount;
    
    try {
        if(amount <= 0) {
            throw runtime_error("Invalid amount!");
        }
        if(amount > from.balance) {
            throw runtime_error("Insufficient funds!");
        }
        
        from.balance -= amount;
        to.balance += amount;
        
        update_customer(from);
        update_customer(to);
        
        log_transaction(from.account_number, "TRANSFER OUT", amount, from.balance);
        log_transaction(to.account_number, "TRANSFER IN", amount, to.balance);
        
        update_branch_stats(from.branch_code, -amount, false);
        update_branch_stats(to.branch_code, amount, false);
        
        cout << "Transfer successful!\n";
    }
    catch(const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void view_statement(CustomerRecord &record) {
    cout << "\n=== ACCOUNT STATEMENT ===\n";
    cout << "Account: " << record.account_number << "\n";
    cout << "Name: " << record.full_name << "\n";
    cout << "Current Balance: R" << record.balance << "\n";
    cout << "\n=== RECENT TRANSACTIONS ===\n";
    
    ifstream file("transactions.dat", ios::binary);
    TransactionRecord trans;
    int count = 0;
    
    while(file.read((char*)&trans, sizeof(trans)) && count < 10) {
        if(string(trans.account_number) == string(record.account_number)) {
            cout << trans.date << " | " << trans.type << " | R" << trans.amount << " | Balance: R" << trans.new_balance << "\n";
            count++;
        }
    }
    file.close();
    
    if(count == 0) {
        cout << "No transactions found.\n";
    }
}

void change_pin(CustomerRecord &record) {
    string old_pin, new_pin;
    string decrypted = simple_encrypt(record.encrypted_pin);
    
    cout << "Enter current PIN: ";
    cin >> old_pin;
    
    if(old_pin != decrypted) {
        cout << "Incorrect PIN!\n";
        return;
    }
    
    cout << "Enter new PIN (5 digits): ";
    cin >> new_pin;
    
    if(new_pin.length() != 5 || !all_of(new_pin.begin(), new_pin.end(), ::isdigit)) {
        cout << "PIN must be 5 digits!\n";
        return;
    }
    
    string encrypted_new = simple_encrypt(new_pin);
    strcpy(record.encrypted_pin, encrypted_new.c_str());
    update_customer(record);
    cout << "PIN changed successfully!\n";
}

// Customer menu

void customer_menu(CustomerRecord &record) {
    int choice;
    do {
        cout << "\n=== CUSTOMER MENU ===\n";
        cout << "1. View Balance\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Transfer\n";
        cout << "5. View Statement\n";
        cout << "6. Change PIN\n";
        cout << "7. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        
        if(choice == 1) {
            cout << "Balance: R" << record.balance << "\n";
        }
        else if(choice == 2) {
            deposit(record);
        }
        else if(choice == 3) {
            withdraw(record);
        }
        else if(choice == 4) {
            transfer(record);
        }
        else if(choice == 5) {
            view_statement(record);
        }
        else if(choice == 6) {
            change_pin(record);
        }
    } while(choice != 7);
}

// Reports

void daily_transaction_report() {
    cout << "\n=== DAILY TRANSACTION REPORT ===\n";
    ifstream file("transactions.dat", ios::binary);
    TransactionRecord trans;
    int count = 0;
    
    time_t now = time(0);
    string today = ctime(&now);
    today = today.substr(0, 10);
    
    while(file.read((char*)&trans, sizeof(trans))) {
        string trans_date(trans.date);
        if(trans_date.find(today) != string::npos) {
            cout << trans.date << " | " << trans.account_number << " | " << trans.type << " | R" << trans.amount << "\n";
            count++;
        }
    }
    file.close();
    
    cout << "Total transactions today: " << count << "\n";
}

void customer_summary_report() {
    cout << "\n=== CUSTOMER ACCOUNT SUMMARY ===\n";
    ifstream file("customers.dat", ios::binary);
    CustomerRecord record;
    int count = 0;
    double total = 0;
    
    while(file.read((char*)&record, sizeof(record))) {
        cout << record.account_number << " | " << record.full_name << " | R" << record.balance << " | " << record.branch_code << "\n";
        count++;
        total += record.balance;
    }
    file.close();
    
    cout << "\nTotal Customers: " << count << "\n";
    cout << "Total Bank Balance: R" << total << "\n";
}

void branch_performance_report() {
    vector<BranchRecord> branches = load_branches();
    
    cout << "\n=== BRANCH PERFORMANCE REPORT ===\n";
    
    for(int i = 0; i < branches.size(); i++) {
        cout << "Branch: " << branches[i].name << " (" << branches[i].code << ")\n";
        cout << "Total Balance: R" << branches[i].total_balance << "\n";
        cout << "Customer Count: " << branches[i].customer_count << "\n";
        cout << "Average per Customer: R" << (branches[i].customer_count > 0 ? branches[i].total_balance / branches[i].customer_count : 0) << "\n";
        cout << "------------------------\n";
    }
}

// Advanced features

void apply_interest() {
    cout << "\n=== APPLYING INTEREST ===\n";
    ifstream infile("customers.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);
    CustomerRecord record;
    int count = 0;
    
    while(infile.read((char*)&record, sizeof(record))) {
        double interest = 0;
        
        if(record.account_type == 1) { // Savings
            interest = record.balance * 0.025;
            record.balance += interest;
            count++;
            cout << "Added R" << interest << " interest to " << record.account_number << " (Savings)\n";
        }
        else if(record.account_type == 3) { // Fixed Deposit
            interest = record.balance * 0.05;
            record.balance += interest;
            count++;
            cout << "Added R" << interest << " interest to " << record.account_number << " (Fixed Deposit)\n";
        }
        
        temp.write((char*)&record, sizeof(record));
    }
    infile.close();
    temp.close();
    remove("customers.dat");
    rename("temp.dat", "customers.dat");
    
    cout << "Interest applied to " << count << " accounts.\n";
}

void search_customer() {
    string search_term;
    cout << "Enter account number or name to search: ";
    cin.ignore();
    getline(cin, search_term);
    
    cout << "\n=== SEARCH RESULTS ===\n";
    ifstream file("customers.dat", ios::binary);
    CustomerRecord record;
    bool found = false;
    
    while(file.read((char*)&record, sizeof(record))) {
        string acc(record.account_number);
        string name(record.full_name);
        
        if(acc.find(search_term) != string::npos || name.find(search_term) != string::npos) {
            cout << "Account: " << record.account_number << "\n";
            cout << "Name: " << record.full_name << "\n";
            cout << "Balance: R" << record.balance << "\n";
            cout << "Branch: " << record.branch_code << "\n";
            cout << "------------------------\n";
            found = true;
        }
    }
    file.close();
    
    if(!found) {
        cout << "No customers found matching: " << search_term << "\n";
    }
}

// Backup and export

void backup_data() {
    system("copy customers.dat customers_backup.dat");
    system("copy transactions.dat transactions_backup.dat");
    system("copy branches.dat branches_backup.dat");
    system("copy tellers.dat tellers_backup.dat");
    cout << "Backup completed!\n";
}

void recover_data() {
    system("copy customers_backup.dat customers.dat");
    system("copy transactions_backup.dat transactions.dat");
    system("copy branches_backup.dat branches.dat");
    system("copy tellers_backup.dat tellers.dat");
    cout << "Recovery completed!\n";
}

void export_to_csv() {
    ofstream csv("bank_export.csv");
    csv << "Account Number,Full Name,SA ID,Contact,Email,Address,DOB,Account Type,Balance,Branch Code\n";
    
    ifstream file("customers.dat", ios::binary);
    CustomerRecord record;
    
    while(file.read((char*)&record, sizeof(record))) {
        string type;
        if(record.account_type == 1) type = "Savings";
        else if(record.account_type == 2) type = "Cheque";
        else if(record.account_type == 3) type = "Fixed Deposit";
        else type = "Student";
        
        csv << record.account_number << ","
            << record.full_name << ","
            << record.sa_id << ","
            << record.contact << ","
            << record.email << ","
            << record.address << ","
            << record.dob << ","
            << type << ","
            << record.balance << ","
            << record.branch_code << "\n";
    }
    file.close();
    csv.close();
    
    cout << "Data exported to bank_export.csv\n";
}

// Teller transaction helper

void teller_transaction(string branch_code) {
    string acc_num, pin;
    cout << "Enter customer account number: ";
    cin >> acc_num;
    cout << "Verify customer PIN: ";
    cin >> pin;
    
    if(verify_customer_pin(acc_num, pin)) {
        CustomerRecord record;
        if(find_customer(acc_num, record)) {
            cout << "Customer: " << record.full_name << "\n";
            cout << "Current Balance: R" << record.balance << "\n";
            
            int trans_choice;
            cout << "1. Deposit\n2. Withdraw\nChoice: ";
            cin >> trans_choice;
            
            if(trans_choice == 1) {
                deposit(record);
            } else if(trans_choice == 2) {
                withdraw(record);
            }
        }
    } else {
        cout << "PIN verification failed!\n";
    }
}

// Main

int main(){

    init_system();
    bool run = true;

    while (run == true){

        int choice;

        cout << "\n=== BANKING SYSTEM ===\n";
        cout << "1. Create Teller Account" << "\n";
        cout << "2. Login As Teller" << "\n";
        cout << "3. Customer Login" << "\n";
        cout << "4. View All Branches" << "\n";
        cout << "5. Inter-Branch Comparison" << "\n";
        cout << "6. Branch Performance Report" << "\n";
        cout << "7. Daily Transaction Report" << "\n";
        cout << "8. Customer Summary Report" << "\n";
        cout << "9. Apply Interest" << "\n";
        cout << "10. Search Customer" << "\n";
        cout << "11. Backup Data" << "\n";
        cout << "12. Recover Data" << "\n";
        cout << "13. Export to CSV" << "\n";
        cout << "14. Exit" << "\n";
        cout << "Choice: ";

        cin >> choice;

        if (choice == 1){
           
            create_teller();
           

        } else if(choice == 2){

            string branch_code;
            
            if(login_teller(branch_code)) {

                int teller_choice;
                
                do {
                    cout << "\n=== TELLER MENU ===\n";
                    cout << "1. Register New Customer" << "\n";
                    cout << "2. Process Customer Transaction" << "\n";
                    cout << "3. Search Customer" << "\n";
                    cout << "4. View Branch Details" << "\n";
                    cout << "5. Generate Reports" << "\n";
                    cout << "6. Logout" << "\n";
                    cout << "Choice: ";
                    
                    cin >> teller_choice;

                    if (teller_choice == 1){
                        register_cust(branch_code);
                    }
                    else if(teller_choice == 2){
                        teller_transaction(branch_code);
                    }
                    else if(teller_choice == 3){
                        search_customer();
                    }
                    else if(teller_choice == 4){
                        view_all_branches();
                    }
                    else if(teller_choice == 5){
                        int report_choice;
                        cout << "1. Daily Transaction Report\n2. Customer Summary\n3. Branch Performance\nChoice: ";
                        cin >> report_choice;
                        if(report_choice == 1) daily_transaction_report();
                        else if(report_choice == 2) customer_summary_report();
                        else if(report_choice == 3) branch_performance_report();
                    }
                } while(teller_choice != 6);
            }

        } else if(choice == 3){
            
            string acc_num, pin;
            cout << "Enter Account Number: ";
            cin >> acc_num;
            cout << "Enter PIN: ";
            cin >> pin;
            
            if(verify_customer_pin(acc_num, pin)) {
                CustomerRecord record;
                if(find_customer(acc_num, record)) {
                    cout << "Login successful! Welcome " << record.full_name << "\n";
                    customer_menu(record);
                }
            } else {
                cout << "Invalid account number or PIN!\n";
            }
            
        } else if(choice == 4){
            view_all_branches();
        } else if(choice == 5){
            inter_branch_comparison();
        } else if(choice == 6){
            branch_performance_report();
        } else if(choice == 7){
            daily_transaction_report();
        } else if(choice == 8){
            customer_summary_report();
        } else if(choice == 9){
            apply_interest();
        } else if(choice == 10){
            search_customer();
        } else if(choice == 11){
            backup_data();
        } else if(choice == 12){
            recover_data();
        } else if(choice == 13){
            export_to_csv();
        } else if(choice == 14){
            run = false;
            cout << "Thank you for using the banking system!\n";
        }
        
    }
   
    return 0;
}

/*
Sources:
- C++17 documentation: https://en.cppreference.com/w/cpp/17
- C++ file handling: https://www.cplusplus.com/doc/tutorial/files/
- C++ classes and inheritance: https://www.cplusplus.com/doc/tutorial/classes/
- C++ exception handling: https://www.cplusplus.com/doc/tutorial/exceptions/
- C++ STL: https://www.cplusplus.com/reference/stl/
- C++ hashing: https://www.cplusplus.com/reference/functional/hash/
- C++ random number generation: https://www.cplusplus.com/reference/random/
- C++ string manipulation: https://www.cplusplus.com/reference/string/string/
- C++ time functions: https://www.cplusplus.com/reference/ctime/
- C++ overriding virtual functions: https://www.cplusplus.com/doc/tutorial/polymorphism/
- C++ binary file I/O: https://www.cplusplus.com/doc/tutorial/files/#binary
- GeeksforGeeks XOR: https://www.geeksforgeeks.org/dsa/xor-cipher/
*/