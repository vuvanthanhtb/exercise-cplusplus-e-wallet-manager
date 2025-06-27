#include "../include/AuthService.h"
#include "../include/utils/OTPUtil.h"
#include "../include/utils/PasswordUtil.h"
#include "../include/utils/RandomUtil.h"
#include <iostream>

using namespace std;

AuthService::AuthService(UserManager &um) : userManager(um) {}

// Đăng nhập hệ thống
User *AuthService::login() {
  string username, password;
  cout << "\n=== ĐĂNG NHẬP ===\nTên đăng nhập: ";
  getline(cin, username);
  cout << "Mật khẩu: ";
  getline(cin, password);

  User *user = userManager.findByUsername(username);
  if (user &&
      PasswordUtil::verifyPassword(password, user->getHashedPassword())) {
    cout << "Đăng nhập thành công!\n";
    AuthService::setCurrentUser(user);
    return user;
  }

  cout << "Tên đăng nhập hoặc mật khẩu không đúng.\n";
  return nullptr;
}

// Đăng ký tài khoản mới (admin có thể tạo cả tài khoản user/admin)
void AuthService::registerUser() {
  User *currentUser = AuthService::getCurrentUserr();
  string fullname, username, email, password, role;

  cout << "\n=== ĐĂNG KÝ TÀI KHOẢN ===\nHọ và tên: ";
  getline(cin, fullname);
  cout << "Tên đăng nhập: ";
  getline(cin, username);
  cout << "Email: ";
  getline(cin, email);
  cout << "Mật khẩu: ";
  getline(cin, password);

  // Nếu không có người dùng hiện tại (tức là người dùng tự đăng ký)
  if (currentUser->getUsername().empty()) {
    role = "user";
  } else {
    cout << "Phân quyền (user/admin): ";
    getline(cin, role);
  }

  // Kiểm tra tên đăng nhập đã tồn tại
  if (userManager.findByUsername(username)) {
    cout << "Tên đăng nhập đã tồn tại.\n";
    return;
  }

  // Mã hóa mật khẩu và tạo tài khoản
  string hashed = PasswordUtil::hashPassword(password);
  User newUser(fullname, username, email, hashed, role, 0.0);
  userManager.addUser(newUser);

  cout << "Tạo tài khoản thành công.\n";
}

// Đặt lại mật khẩu qua email
void AuthService::resetPassword() {
  string email;
  cout << "\n=== ĐẶT LẠI MẬT KHẨU ===\nNhập email của bạn: ";
  getline(cin, email);

  User *user = userManager.findByEmail(email);
  if (!user) {
    cout << "Không tìm thấy email trong hệ thống.\n";
    return;
  }

  // Tạo mật khẩu mới và OTP
  string newPassword = RandomUtil::generateRandomString(8);
  string otp = OTPUtil::generate();

  // Gửi thông báo (giả lập gửi email)
  cout << "OTP đã được gửi qua email: " << otp << endl;
  cout << "Mật khẩu mới (được gửi kèm qua email): " << newPassword << endl;

  // Nhập OTP để xác nhận
  string inputOtp;
  cout << "Nhập mã OTP: ";
  getline(cin, inputOtp);

  if (inputOtp == otp) {
    string hashed = PasswordUtil::hashPassword(newPassword);
    user->setPassword(hashed);
    userManager.saveUsers();
    cout << "Mật khẩu đã được đặt lại thành công.\n";
  } else {
    cout << "Mã OTP không đúng. Mật khẩu không được thay đổi.\n";
  }
}
