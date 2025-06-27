#include "../include/TransactionManager.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

// Chuyển giao dịch thành dòng CSV
string Transaction::toCSV() const {
  return fromUser + "," + toUser + "," + to_string(amount) + "," + timestamp;
}

// Tạo đối tượng Transaction từ dòng CSV
Transaction Transaction::fromCSV(const string &line) {
  Transaction tx;
  stringstream ss(line);
  string amt;

  getline(ss, tx.fromUser, ',');
  getline(ss, tx.toUser, ',');
  getline(ss, amt, ',');
  getline(ss, tx.timestamp);

  tx.amount = stod(amt); // chuyển chuỗi sang double
  return tx;
}

// Constructor: khởi tạo với tên file giao dịch
TransactionManager::TransactionManager(const string &filename)
    : transactionFile(filename) {
  loadTransactions(); // nạp dữ liệu từ file
}

// Nạp danh sách giao dịch từ file
void TransactionManager::loadTransactions() {
  transactions.clear();
  ifstream file(transactionFile);
  string line;
  while (getline(file, line)) {
    transactions.push_back(Transaction::fromCSV(line));
  }
}

// Lưu tất cả giao dịch hiện tại vào file
void TransactionManager::saveTransactions() const {
  ofstream file(transactionFile);
  for (const auto &tx : transactions) {
    file << tx.toCSV() << "\n";
  }
}

// Ghi lại giao dịch mới vào danh sách và lưu ra file
void TransactionManager::logTransaction(const string &from, const string &to,
                                        double amount) {
  time_t now = time(0);
  string timestamp = ctime(&now);
  timestamp.pop_back(); // loại bỏ ký tự xuống dòng '\n'

  Transaction tx = {from, to, amount, timestamp};
  transactions.push_back(tx);
  saveTransactions();
}

// Hiển thị lịch sử giao dịch của người dùng
void TransactionManager::showHistoryForUser(const string &username) const {
  cout << "\n[Lịch sử giao dịch của " << username << "]\n";
  for (const auto &tx : transactions) {
    if (tx.fromUser == username || tx.toUser == username) {
      cout << tx.timestamp << " | " << tx.fromUser << " -> " << tx.toUser
           << " : " << tx.amount << " VND\n";
    }
  }
}

// Chuyển tiền giữa hai người dùng, có kiểm tra và rollback nếu lỗi
bool TransactionManager::transferPoints(User &from, User &to, double amount,
                                        double totalWalletBalance) {
  if (amount <= 0) {
    cout << "Chuyển thất bại: Số tiền không hợp lệ.\n";
    return false;
  }

  if (from.getWalletBalance() < amount) {
    cout << "Chuyển thất bại: Số dư ví không đủ.\n";
    return false;
  }

  // Ghi nhớ số dư trước giao dịch để rollback nếu cần
  double fromOld = from.getWalletBalance();
  double toOld = to.getWalletBalance();

  try {
    // Trừ tiền người gửi, cộng tiền người nhận
    from.setWalletBalance(fromOld - amount);
    to.setWalletBalance(toOld + amount);

    // Kiểm tra ràng buộc tổng số dư không vượt quá giới hạn
    if (from.getWalletBalance() + to.getWalletBalance() > totalWalletBalance) {
      throw runtime_error("Tổng số dư ví vượt quá giới hạn.");
    }

    // Lưu lại giao dịch nếu thành công
    logTransaction(from.getUsername(), to.getUsername(), amount);
    cout << "Chuyển tiền thành công!\n";
    return true;
  } catch (const exception &ex) {
    // Nếu lỗi, khôi phục lại số dư cũ
    from.setWalletBalance(fromOld);
    to.setWalletBalance(toOld);
    cout << "Chuyển thất bại: " << ex.what() << "\n";
    return false;
  }
}

void TransactionManager::showDetailedHistoryForUser(
    const string &username) const {
  cout << "\n[ LỊCH SỬ GIAO DỊCH - " << username << " ]\n";
  cout << left << setw(30) << "Thời gian" << setw(30) << "Từ" << setw(30)
       << "Đến" << setw(20) << "Số tiền" << setw(15) << "Loại" << endl;

  cout << string(150, '-') << "\n";

  for (const auto &tx : transactions) {
    if (tx.fromUser == username || tx.toUser == username) {
      string type = tx.fromUser == username ? "Chuyển" : "Nhận";
      cout << left << setw(30) << tx.timestamp << setw(30) << tx.fromUser
           << setw(30) << tx.toUser << setw(20) << fixed << setprecision(2)
           << tx.amount << setw(15) << type << endl;
    }
  }
}
