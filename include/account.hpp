#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>

class Account {
private:
    int account_number;
    double balance;
    std::string customer_name;
    std::string account_type;

public:
    Account(int account_number, double balance, const std::string& customer_name, const std::string& account_type);

    void check_balance() const;
    void deposit(double amount);
    void withdraw(double amount);
    void get_account_info() const;
};

#endif
