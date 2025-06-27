#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "User.h"
#include <string>
#include <vector>

using namespace std;

struct Transaction {
  string fromUser;
  string toUser;
  double amount;
  string timestamp;

  string toCSV() const;
  static Transaction fromCSV(const string &);
};

class TransactionManager {
private:
  vector<Transaction> transactions;
  string transactionFile;

public:
  TransactionManager(const string &filename);
  void loadTransactions();
  void saveTransactions() const;
  void logTransaction(const string &from, const string &to, double amount);
  void showHistoryForUser(const string &username) const;
  bool transferPoints(User &from, User &to, double amount,
                      double totalWalletBalance);
  void showDetailedHistoryForUser(const string &username) const;
};

#endif // TRANSACTION_MANAGER_H
