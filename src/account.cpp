#include "../include/account.hpp"
#include <iostream>
#include <string>
using namespace std;

Account::Account(int account_number, double balance, const string& account_name, const string& account_type, const string& created_by)
    : account_number(account_number), balance(balance), account_name(account_name), account_type(account_type), created_by(created_by) {}