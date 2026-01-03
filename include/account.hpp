#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <sqlite3.h>

class Account {
private:
    int account_number;
    double balance;
    std::string account_name;
    std::string account_type;
    std::string created_by;

public:
    Account(int account_number, double balance, const std::string& account_name, const std::string& account_type, const std::string& created_by);
};

#endif
