#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User {
protected:
	string id;
	string fullname;
	string username;
	string email;
	string hashedPassword;
	string role; // "user" or "admin"
	double walletBalance;

public:
	User();
	User(string, string, string, string, string, double = 0.0);
	User(string, string, string, string, string, string, double = 0.0);

	string getId() const;
	string getUsername() const;
	string getFullname() const;
	string getEmail() const;
	string getRole() const;
	string getHashedPassword() const;
	void display() const;
	void updateInfo(string email);
	void setPassword(const string &hashed);
	void setId(const string &);
	bool verifyEmail(const string &) const;
	void changePasswordWithOTP(string email);

	string toCSV() const;
	static User fromCSV(const string &line);

	double getWalletBalance() const;
	void setWalletBalance(double);
	void displayWallet() const;
	void setRole(const string &newRole);
};

#endif