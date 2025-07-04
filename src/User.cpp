#include "../include/User.h"
#include "../include/utils/MailUtil.h"
#include "../include/utils/OTPUtil.h"
#include "../include/utils/PasswordUtil.h"
#include "../include/utils/UUIDUtil.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

User::User() {}

User::User(string fn, string un, string em, string hp, string rl, double wb)
    : fullname(fn), username(un), email(em), hashedPassword(hp), role(rl),
      walletBalance(wb) {
  id = UUIDUtil::generateUUID(); // Tự động tạo UUID
}

// Các phương thức getter
string User::getId() const { return id; }
string User::getUsername() const { return username; }
string User::getEmail() const { return email; }
string User::getFullname() const { return fullname; }
string User::getRole() const { return role; }
double User::getWalletBalance() const { return walletBalance; }
string User::getHashedPassword() const { return hashedPassword; }

// Các phương thức setter
void User::setId(const string &newId) { id = newId; }
void User::setWalletBalance(double amount) { walletBalance = amount; }
void User::setPassword(const string &hashed) { hashedPassword = hashed; }
void User::setRole(const string &newRole) { role = newRole; }

// Hiển thị thông tin tài khoản
void User::display() const {
  cout << "\n[ Thông tin tài khoản ]" << endl;
  cout << left << setw(25) << "ID:" << id << endl;
  cout << left << setw(28) << "Họ tên:" << fullname << endl;
  cout << left << setw(30) << "Tên đăng nhập:" << username << endl;
  cout << left << setw(25) << "Email:" << email << endl;
  cout << left << setw(26) << "Vai trò:" << role << endl;
}

// Cập nhật thông tin cá nhân có xác thực OTP
void User::updateInfo(string email) {
  cout << "Nhập họ tên mới: ";
  getline(cin, fullname);
  cout << "Nhập email mới: ";
  getline(cin, email);

  string otp = OTPUtil::generate();
  bool isSendMail =
      MailUtil::sendMail(email, "Xác nhận OTP", "Mã OTP của bạn là: " + otp);
  if (!isSendMail) {
    return;
  }

  string input;
  const int maxAttempts = 3;

  for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
    cout << "Nhập mã OTP (" << attempt << "/" << maxAttempts << "): ";
    getline(cin, input);

    if (input == otp) {
      cout << "Thông tin đã được cập nhật." << endl;
      return;
    } else if (attempt < maxAttempts) {
      cout << "Mã OTP không đúng. Vui lòng thử lại." << endl;
    }
  }

  cout << "Bạn đã nhập sai quá số lần cho phép. Cập nhật bị hủy." << endl;
}

// Hiển thị số dư trong ví
void User::displayWallet() const {
  cout << "\n[Số dư ví]: " << walletBalance << " $" << endl;
}

// Kiểm tra email trùng khớp
bool User::verifyEmail(const string &mail) const { return email == mail; }

// Chuyển đối tượng User thành chuỗi CSV
string User::toCSV() const {
  return id + "," + fullname + "," + username + "," + email + "," +
         hashedPassword + "," + role + "," + to_string(walletBalance);
}

// Tạo đối tượng User từ dòng CSV
User User::fromCSV(const string &line) {
  stringstream ss(line);
  string id, fn, un, em, hp, rl, wb;
  getline(ss, id, ',');
  getline(ss, fn, ',');
  getline(ss, un, ',');
  getline(ss, em, ',');
  getline(ss, hp, ',');
  getline(ss, rl, ',');
  getline(ss, wb);
  User u(fn, un, em, hp, rl, stod(wb));
  u.id = id;
  return u;
}

// Thay đổi mật khẩu với xác thực OTP và kiểm tra mật khẩu cũ
void User::changePasswordWithOTP(string email) {
  string currentPass;
  cout << "Nhập mật khẩu hiện tại: ";
  getline(cin, currentPass);

  if (!PasswordUtil::verifyPassword(currentPass, hashedPassword)) {
    cout << "Mật khẩu hiện tại không đúng." << endl;
    return;
  }

  string newPass;
  cout << "Nhập mật khẩu mới: ";
  getline(cin, newPass);

  string otp = OTPUtil::generate();
  cout << "Mã OTP đã gửi: " << otp << endl;
  bool isSendMail =
      MailUtil::sendMail(email, "Xác nhận OTP", "Mã OTP của bạn là: " + otp);
  if (!isSendMail) {
    return;
  }

  string input;
  bool timeout = false;

  // Hẹn giờ 60 giây: hết thời gian thì timeout
  thread timeoutThread([&timeout]() {
    this_thread::sleep_for(chrono::minutes(1));
    timeout = true;
  });

  cout << "Nhập mã OTP (trong vòng 60 giây): ";
  getline(cin, input);

  timeoutThread.detach(); // Không chặn luồng chính

  if (timeout) {
    cout << "\nMã OTP đã hết hạn. Phiên làm việc kết thúc." << endl;
    exit(0); // Có thể thay bằng gọi hàm logout nếu cần
  }

  if (input == otp) {
    hashedPassword = PasswordUtil::hashPassword(newPass);
    cout << "Mật khẩu đã được thay đổi thành công." << endl;
  } else {
    cout << "Mã OTP không hợp lệ. Mật khẩu không được thay đổi." << endl;
  }
}
