#ifndef BANK
#define BANK

#include "../include/account.hpp"

#include <vector>

class Bank {
    private:
        sqlite3* db;

    public:
        Bank(sqlite3* db);
        
        void create_table();
        void create_acc_db(int acc_num, double s_bal, const std::string& acc_name, const std::string& acc_type, const std::string& created_by);
        Account create_account(const std::string& acc_type, const std::string& acc_name, const std::string& created_by);
        void display_acc_info(const std::string& acc_name, const std::string& created_by);
        void deposit(double amount, const std::string& acc_name);
        void withdraw(double amount, const std::string& acc_name);
};

#endif