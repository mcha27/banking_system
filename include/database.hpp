#ifndef DATABASE
#define DATABASE

#include <string>
#include <sqlite3.h>
#include "../include/user.hpp"

class Database {
    private:
        sqlite3* db;

    public:
        Database(sqlite3* d);
        void create_table();
        bool register_user(User* user);
        bool login_user(std::string username, std::string password);
        std::string hash_password(std::string& password);
};


#endif