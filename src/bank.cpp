#include "../include/bank.hpp"
#include "../include/account.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;

Bank::Bank(sqlite3* d) 
    : db(d) {}

void Bank::create_table(){
    const char* sql =
        "CREATE TABLE IF NOT EXISTS ACC_INFO ("
        "ACCOUNT_NUMBER INTEGER NOT NULL UNIQUE, "
        "BALANCE REAL NOT NULL, "
        "ACCOUNT_NAME TEXT NOT NULL UNIQUE, "
        "ACCOUNT_TYPE TEXT NOT NULL, "
        "CREATED_BY TEXT NOT NULL"
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Bank::create_acc_db(int acc_num, double s_bal, const string& acc_name, const string& acc_type, const string& created_by){
    sqlite3_stmt* stmt;

    const char* insert = "INSERT INTO ACC_INFO (ACCOUNT_NUMBER, BALANCE, ACCOUNT_NAME, ACCOUNT_TYPE, CREATED_BY) VALUES (?, ?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, insert, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
    }

    sqlite3_bind_int(stmt, 1, acc_num);
    sqlite3_bind_double(stmt, 2, s_bal);
    sqlite3_bind_text(stmt, 3, acc_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, acc_type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, created_by.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

void Bank::display_acc_info(const string& acc_name, const string& created_by){
    sqlite3_stmt* stmt = nullptr;
    int rc;

    const char* sql =
        "SELECT ACCOUNT_NUMBER, BALANCE, ACCOUNT_TYPE "
        "FROM ACC_INFO WHERE ACCOUNT_NAME = ? AND CREATED_BY = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, acc_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, created_by.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;

        int acc_num = sqlite3_column_int(stmt, 0);
        double balance = sqlite3_column_double(stmt, 1);

        const unsigned char* text = sqlite3_column_text(stmt, 2);
        string acc_type = text ? reinterpret_cast<const char*>(text) : "";

        cout << "====================================" << endl;
        cout << "ACCOUNT NUMBER:  " << acc_num << endl;
        cout << "BALANCE: " << balance << endl;
        cout << "ACCOUNT TYPE: " << acc_type << endl;
        cout << "CREATED BY: " << created_by << endl;
        cout << "====================================" << endl;
    }

    if (!found) {
        cout << "No account found for customer: " << acc_name << endl;
    } else if (rc != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

Account Bank::create_account(const string& account_type, const string& account_name, const string& created_by){
    srand(static_cast<unsigned int>(time(NULL)));
    long long min_val = 100000000LL;
    long long max_val = 999999999LL;
    long long range = max_val - min_val + 1;
    long long account_number = min_val + (static_cast<long long>(rand()) % range);

    double starting_balance = 0.0;

    if (account_type == "savings") {
        create_acc_db(account_number, starting_balance, account_name, account_type, created_by);
        return Account(account_number, starting_balance, account_name, account_type, created_by);
    }
    else {
        create_acc_db(account_number, starting_balance, account_name, account_type, created_by);
        return Account(account_number, starting_balance, account_name, account_type, created_by);
    }
}

void Bank::deposit(double amount, const string& acc_name) {
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "UPDATE ACC_INFO "
        "SET BALANCE = BALANCE + ? "
        "WHERE ACCOUNT_NAME = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, acc_name.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Update failed: " << sqlite3_errmsg(db) << endl;
    } else if (sqlite3_changes(db) == 0) {
        cerr << "No account found for customer: " << acc_name << endl;
    }
    sqlite3_finalize(stmt);
}

void Bank::withdraw(double amount, const string& acc_name) {
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "UPDATE ACC_INFO "
        "SET BALANCE = BALANCE - ? "
        "WHERE ACCOUNT_NAME = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, acc_name.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Update failed: " << sqlite3_errmsg(db) << endl;
    } else if (sqlite3_changes(db) == 0) {
        cerr << "No account found for customer: " << acc_name << endl;
    }
    sqlite3_finalize(stmt);
}

void Bank::delete_account(const string& acc_name) {
    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "DELETE FROM ACC_INFO WHERE ACCOUNT_NAME = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, acc_name.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Delete failed: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Account deleted successfully" << endl;
    }

    sqlite3_finalize(stmt);
}


void Bank::transfer_funds(const string& from_acc, const string& to_acc, double amount) {
    if (amount <= 0) {
        cerr << "Invalid transfer amount." << endl;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc;

    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to begin transaction." << endl;
    }

    double from_balance = 0.0;
    const char* check_sql = "SELECT BALANCE FROM ACC_INFO WHERE ACCOUNT_NAME = ?;";

    rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, from_acc.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        cerr << "Source account not found." << endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    }

    from_balance = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);

    if (from_balance < amount) {
        cerr << "Insufficient funds." << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    }

    const char* debit_sql =
        "UPDATE ACC_INFO SET BALANCE = BALANCE - ? WHERE ACCOUNT_NAME = ?;";

    rc = sqlite3_prepare_v2(db, debit_sql, -1, &stmt, nullptr);
    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, from_acc.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Debit failed." << endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    }
    sqlite3_finalize(stmt);

    const char* credit_sql =
        "UPDATE ACC_INFO SET BALANCE = BALANCE + ? WHERE ACCOUNT_NAME = ?;";

    rc = sqlite3_prepare_v2(db, credit_sql, -1, &stmt, nullptr);
    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, to_acc.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Credit failed." << endl;
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    }
    sqlite3_finalize(stmt);

    rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Commit failed." << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    }

    cout << "TRANSFER IS SUCCESSFUL." << endl;
}
