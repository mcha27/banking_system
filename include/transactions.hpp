#ifndef TRANSACTIONS
#define TRANSACTIONS

#include <string>
#include <sqlite3.h>

class Transactions {
    private:
        sqlite3* db2;
        std::string date;
        std::string transaction_type;
        std::string user;
        std::string account_name;
        std::string to_account;
        double amount;
    
        public:
            Transactions(sqlite3* db2);
            void create_table();
            void record_transaction(std::string date, std::string transaction_type, std::string user, std::string account_name, std::string to_account, double amount);
};

#endif
