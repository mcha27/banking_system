#include "../include/account.hpp"
#include <iostream>
#include <string>
using namespace std;

Account::Account(int account_number, double balance, const string& customer_name, const string& account_type)
    : account_number(account_number), balance(balance), customer_name(customer_name), account_type(account_type) {}