#include "../include/account.hpp"
#include "../include/user.hpp"
#include "../include/database.hpp"
#include "../include/bank.hpp"

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

pair<bool, string> auth(){
    sqlite3* db;
    cout << "===============" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "===============" << endl;
    int option;
    cin >> option;

    int rc = sqlite3_open("data/auth.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
    }

    Database* datab = new Database(db);
    datab->create_table();

    bool RUN = true;

    while(RUN){
        if (option == 1){
            string username;
            string password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            
            string hashed = datab->hash_password(password);
            bool auth = datab->login_user(username, hashed);
            if (auth == true){
                RUN = false;
                return {auth, username};
            }
            else {
                RUN = true;
            }
        }

        if (option == 2){
            string username;
            string password;
            cout << "Create a username: ";
            cin >> username;
            cout << "Create a password: ";
            cin >> password;
            User* user = new User(username, password);
            RUN = !datab->register_user(user);          
        }
    }
    sqlite3_close(db);
    return {false, ""};
}

int main(){
    auto [authenticated, username] = auth();
    bool RUN = true;
    this_thread::sleep_for(chrono::seconds(2));
    if (authenticated == true){
        while (RUN){
            sqlite3* db;
            int rc = sqlite3_open("data/bank.db", &db);
            if (rc != SQLITE_OK) {
                cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
            }

            int option;
            cout << "===================================" << endl;
            cout << "1. View Account Information" << endl;
            cout << "2. Create an Account" << endl;
            cout << "3. Deposit funds into an Account" << endl;
            cout << "4. Withdraw funds from an Account" << endl;
            cout << "5. Transfer funds between two Accounts" << endl;
            cout << "6. Delete an Account" << endl;
            cout << "===================================" << endl;
            cin >> option;

            Bank* bank = new Bank(db);
            bank->create_table();
            if (option == 1){
                string account_name;
                cout << "Enter account name: ";
                cin >> account_name;
                this_thread::sleep_for(chrono::seconds(2));
                bank->display_acc_info(account_name, username);
                this_thread::sleep_for(chrono::seconds(2));
            }
            else if (option == 2){
                string account_name;
                string account_type;
                cout << "Enter name for the account: ";
                cin >> account_name;
                cout << "Savings or Checking? ";
                cin >> account_type;
                this_thread::sleep_for(chrono::seconds(2));
                Account new_acc = bank->create_account(account_type, account_name, username);
                cout << "ACCOUNT HAS BEEN SUCCESSFULLY CREATED." << endl;
                this_thread::sleep_for(chrono::seconds(2));
            }
            else if (option == 3){ 
                string account_name;
                double amount;
                cout << "Enter name for the account: " << endl;
                cin >> account_name;
                cout << "How much would you like to deposit?" << endl;
                cin >> amount;
                this_thread::sleep_for(chrono::seconds(2));
                bank->deposit(amount, account_name);
                cout << "Deposit is successful." << endl;
                this_thread::sleep_for(chrono::seconds(2));
            }
            else if (option == 4){
                string account_name;
                double amount;
                cout << "Enter name for the account: " << endl;
                cin >> account_name;
                cout << "How much would you like to deposit?" << endl;
                cin >> amount;
                this_thread::sleep_for(chrono::seconds(2));
                bank->withdraw(amount, account_name);
                cout << "Withdraw is successful." << endl;
                this_thread::sleep_for(chrono::seconds(2));
            }
            else if (option == 5){
                string from_acc;
                string to_acc;
                double amount;
                cout << "Enter account name to transfer from: " << endl;
                cin >> from_acc;
                cout << "Enter account name to transfer to: " << endl;
                cin >> to_acc;
                cout << "How much would you like to transfer?" << endl;
                cin >> amount;
                this_thread::sleep_for(chrono::seconds(2));
                bank->transfer_funds(from_acc, to_acc, amount);
                this_thread::sleep_for(chrono::seconds(2));
            }
            else if (option == 6){
                string account_name;
                cout << "Enter name for the account: " << endl;
                cin >> account_name;
                this_thread::sleep_for(chrono::seconds(2));
                bank->delete_account(account_name);
                this_thread::sleep_for(chrono::seconds(2));
            }
        }
    }

    return 0;
}