#include "../include/bank.hpp"
#include "../include/account.hpp"

#include <iostream>
#include <variant>
#include <string>
#include <random>
using namespace std;

Bank::Bank(variant<int, double, string, string> user) 
    : user_info(user) {}

Account Bank::create_account(string acc_type){
    int account_number = rand() % 100000;
    double starting_balance = 0.0;

    if (acc_type == "savings") {
        return Account(account_number, starting_balance, "Customer", "Savings");
    }

    return Account(account_number, starting_balance, "Customer", "Checking");
}


