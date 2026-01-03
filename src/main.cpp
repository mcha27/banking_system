#include "../include/account.hpp"
#include "../include/user.hpp"
#include "../include/database.hpp"
#include "../include/bank.hpp"

#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

bool auth(){
    sqlite3* db;
    cout << "===============================" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "===============================" << endl;
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
                return true;
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

    return false;
}

int main(){
    bool authenticated = auth();
    bool RUN = true;
    if (authenticated == true){
        while (RUN){
            sqlite3* db;
            int rc = sqlite3_open("data/bank.db", &db);
            if (rc != SQLITE_OK) {
                cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
            }

            int option;
            cout << "===============================" << endl;
            cout << "1. View Account Information" << endl;
            cout << "2. Create an Account" << endl;
            cout << "3. Deposit funds into an Account" << endl;
            cout << "4. Withdraw funds from an Account" << endl;
            cout << "===============================" << endl;
            cin >> option;

            Bank* bank = new Bank(db);
            bank->create_table();
            if (option == 1){
                string cust_name;
                cout << "Enter account name: ";
                cin >> cust_name;
                bank->display_acc_info(cust_name);
            }
            else if (option == 2){
                string cust_name;
                string acc_type;

                cout << "Enter name for the account: ";
                cin >> cust_name;

                cout << "Savings or Checking? ";
                cin >> acc_type;

                Account new_acc = bank->create_account(acc_type, cust_name);
                cout << "USER HAS BEEN SUCCESSFULLY CREATED." << endl;
            }
            else if (option == 3){ 
                string cust_name;
                double amount;
                cout << "Enter name for the account: " << endl;
                cin >> cust_name;
                cout << "How much would you like to deposit?" << endl;
                cin >> amount;
                bank->deposit(amount, cust_name);
                cout << "Deposit is successful." << endl;
            }
            else if (option == 4){
                string cust_name;
                double amount;
                cout << "Enter name for the account: " << endl;
                cin >> cust_name;
                cout << "How much would you like to deposit?" << endl;
                cin >> amount;
                bank->withdraw(amount, cust_name);
                cout << "Withdraw is successful." << endl;
            }
        }

    }
    
    return 0;
}