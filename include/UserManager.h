#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include <string>
#include <vector>

using namespace std;

class UserManager {
private:
	vector<User> users;
	string dataFile;

public:
	UserManager(const string &filename);

	void loadUsers();
	void saveUsers() const;

	User *authenticate(const string &username, const string &password);
	bool addUser(const User &user);
	User *findByUsername(const string &username);
	User *findByEmail(const std::string &email);
	void displayAllUsers() const;
	bool resetPassword(const string &email, const string &newHashedPassword);
	void updateUserByUsername();
};

#endif
