#include "../include/account.hpp"
#include <iostream>
#include <string>
using namespace std;

Account::Account(int account_number, double balance, const string& customer_name, const string& account_type)
    : account_number(account_number), balance(balance), customer_name(customer_name), account_type(account_type) {}

void Account::check_balance() const {
    cout << "Your account balance is: " << balance << endl; //get balance from db and class variable
}

void Account::deposit(double amount) {
    balance += amount; // have to update bank.db balance and record transactions on transactions.db

}

void Account::withdraw(double amount) {
    balance -= amount; // have to update bank.db balance and record transactions on transactions.db
}

void Account::get_account_info() const {
    cout << "Customer name: " << customer_name << endl;
    cout << "Account type: " << account_type << endl;
    cout << "Account number: " << account_number << endl;
}
