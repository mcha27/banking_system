#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <sqlite3.h>

class Account {
private:
    int account_number;
    double balance;
    std::string customer_name;
    std::string account_type;

public:
    Account(int account_number, double balance, const std::string& customer_name, const std::string& account_type);
};

#endif
