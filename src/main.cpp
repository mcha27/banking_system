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

    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
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
    
    if (authenticated == true){
        sqlite3* db;
        int rc = sqlite3_open("data/bank.db", &db);
        if (rc != SQLITE_OK) {
            cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
        }

        int option;
        cout << "1. View Account Information" << endl;
        cout << "2. Create an Account" << endl;
        cin >> option;

        Bank* bank = new Bank(db);
        bank->create_table();
        if (option == 2){
            string cust_name;
            string acc_type;

            cout << "Enter name for the account: ";
            cin >> cust_name;

            cout << "Savings or Checking? ";
            cin >> acc_type;

            Account new_acc = bank->create_account(acc_type, cust_name);
            new_acc.get_account_info();
            new_acc.check_balance();

            new_acc.deposit(199999);
            new_acc.check_balance();
        }
    }
    
    return 0;
}