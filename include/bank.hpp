#ifndef BANK
#define BANK

#include "../include/account.hpp"

#include <variant>

class Bank {
    private:
        std::variant<int, double, std::string, std::string> user_info;

    public:
        Bank(std::variant<int, double, std::string, std::string> user);

        Account create_account(const std::string type);
};

#endif