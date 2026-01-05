#include "../include/transactions.hpp"

#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

Transactions::Transactions(sqlite3* d) 
    : db2(d) {}

void Transactions::create_table(){
    const char* sql =
        "CREATE TABLE IF NOT EXISTS TRANSACTION_HISTORY ("
        "DATE TEXT NOT NULL, "
        "TRANSACTION_TYPE TEXT NOT NULL, "
        "USER TEXT NOT NULL, "
        "ACCOUNT_NAME TEXT NOT NULL, "
        "TO_ACCOUNT TEXT NOT NULL, "
        "AMOUNT REAL NOT NULL "
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db2, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Transactions::record_transaction(const string date, const string transaction_type, const string user, const string account_name, const string to_account, double amount){
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "INSERT INTO TRANSACTION_HISTORY "
        "(DATE, TRANSACTION_TYPE, USER, ACCOUNT_NAME, TO_ACCOUNT, AMOUNT) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db2, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db2) << std::endl;
    }

    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, transaction_type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, account_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, to_account.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, amount);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Insert failed: " << sqlite3_errmsg(db2) << std::endl;
    }

    sqlite3_finalize(stmt);
}