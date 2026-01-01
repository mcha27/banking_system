#include <iostream>
#include <string>
#include "../include/user.hpp"
#include "../include/account.hpp"

User::User(std::string user, std::string pass)
    : username(user), password(pass) {}
