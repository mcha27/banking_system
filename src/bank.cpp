#include "../include/bank.hpp"
#include "../include/account.hpp"

#include <iostream>
#include <variant>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
using namespace std;

Bank::Bank(sqlite3* d) 
    : db(d) {}

void Bank::create_table(){
    const char* sql =
        "CREATE TABLE IF NOT EXISTS ACC_INFO ("
        "ACCOUNT_NUMBER INTEGER NOT NULL UNIQUE, "
        "BALANCE REAL NOT NULL, "
        "CUSTOMER_NAME TEXT NOT NULL, "
        "ACCOUNT_TYPE TEXT NOT NULL"
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Bank::create_acc_db(int acc_num, double s_bal, string cust_name, string acc_type){
    sqlite3_stmt* stmt;

    const char* insert = "INSERT INTO ACC_INFO (ACCOUNT_NUMBER, BALANCE, CUSTOMER_NAME, ACCOUNT_TYPE) VALUES (?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, insert, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        //return false;
    }

    sqlite3_bind_int(stmt, 1, acc_num);
    sqlite3_bind_double(stmt, 2, s_bal);
    sqlite3_bind_text(stmt, 3, cust_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, acc_type.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    /*
    if (rc == SQLITE_DONE) {
        cout << "REGISTRATION SUCCESSFUL\n";
        return true;
    }

    if (rc == SQLITE_CONSTRAINT) {
        cout << "USERNAME ALREADY EXISTS\n";
    } else {
        cerr << "SQLite error: " << sqlite3_errmsg(db) << "\n";
    }
    */
   //return false;
}

Account Bank::create_account(const string acc_type, const string cust_name){
    srand(static_cast<unsigned int>(time(NULL)));
    long long min_val = 100000000LL;
    long long max_val = 999999999LL;
    long long range = max_val - min_val + 1;
    long long account_number = min_val + (static_cast<long long>(rand()) % range);

    double starting_balance = 0.0;

    if (acc_type == "savings") {
        create_acc_db(account_number, starting_balance, cust_name, acc_type);
        return Account(account_number, starting_balance, cust_name, acc_type);
    }
    else {
        create_acc_db(account_number, starting_balance, cust_name, acc_type);
        return Account(account_number, starting_balance, cust_name, acc_type);
    }
}


