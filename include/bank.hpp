#ifndef BANK
#define BANK

#include "../include/account.hpp"

#include <vector>

class Bank {
    private:
        //std::variant<int, double, std::string, std::string> user_info;
        sqlite3* db;
        std::vector<std::string> list_of_accs; //list of accounts by username. search user in array, get account info

    public:
        Bank(sqlite3* db);
        
        void create_table();
        void create_acc_db(int acc_num, double s_bal, std::string cust_name, std::string acc_type);
        Account create_account(const std::string acc_type, const std::string cust_name);
        void display_acc_info(std::string cust_name);
};

#endif