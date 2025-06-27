#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include "UserManager.h"
#include <string>

class AuthService {
private:
  UserManager &userManager;
  User *currentUser;

public:
  AuthService(UserManager &um);

  void setCurrentUser(User *user) { currentUser = user; }
  User *getCurrentUser() { return currentUser; }

  User *login();
  void registerUser();
  void resetPassword();
};

#endif
