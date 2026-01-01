#ifndef USER
#define USER

#include "../include/account.hpp"
#include <string>

class User {
    public:
        std::string username;
        std::string password;
        User(std::string user, std::string pass);
};

#endif
