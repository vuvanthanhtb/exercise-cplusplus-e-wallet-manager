#ifndef PASSWORD_UTIL_H
#define PASSWORD_UTIL_H

#include <string>

class PasswordUtil {
public:
    static std::string hashPassword(const std::string &password);
    static bool verifyPassword(const std::string &password, const std::string &hash);
};

#endif
