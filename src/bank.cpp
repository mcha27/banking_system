#include "../include/bank.hpp"
#include "../include/account.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

Bank::Bank(sqlite3* d) 
    : db(d), list_of_accs{} {}

void Bank::create_table(){
    const char* sql =
        "CREATE TABLE IF NOT EXISTS ACC_INFO ("
        "ACCOUNT_NUMBER INTEGER NOT NULL UNIQUE, "
        "BALANCE REAL NOT NULL, "
        "CUSTOMER_NAME TEXT NOT NULL UNIQUE, "
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
    }

    sqlite3_bind_int(stmt, 1, acc_num);
    sqlite3_bind_double(stmt, 2, s_bal);
    sqlite3_bind_text(stmt, 3, cust_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, acc_type.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}


void Bank::display_acc_info(string cust_name){
    sqlite3_stmt* stmt = nullptr;
    int rc;

    const char* sql =
        "SELECT ACCOUNT_NUMBER, BALANCE, ACCOUNT_TYPE "
        "FROM ACC_INFO WHERE CUSTOMER_NAME = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
    }

    rc = sqlite3_bind_text(stmt, 1, cust_name.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Bind failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int acc_num = sqlite3_column_int(stmt, 0);
        double balance = sqlite3_column_double(stmt, 1);

        const unsigned char* text = sqlite3_column_text(stmt, 2);
        std::string acc_type = text ? reinterpret_cast<const char*>(text) : "";

        std::cout << "ACCOUNT NUMBER = " << acc_num << std::endl;
        std::cout << "BALANCE = " << balance << std::endl;
        std::cout << "ACCOUNT TYPE = " << acc_type << std::endl;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
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
        list_of_accs.push_back(cust_name);
        return Account(account_number, starting_balance, cust_name, acc_type);
    }
    else {
        create_acc_db(account_number, starting_balance, cust_name, acc_type);
        list_of_accs.push_back(cust_name);
        return Account(account_number, starting_balance, cust_name, acc_type);
    }
}


