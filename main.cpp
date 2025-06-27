#include "include/AuthService.h"
#include "include/TransactionManager.h"
#include "include/UserManager.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std;

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// Hàm xử lý chuyển tiền giữa 2 người dùng
void handleTransferMoney(User *currentUser, UserManager &userManager,
                         TransactionManager &txManager) {
  string toUsername;
  double amount;

  cout << "Nhập tên người nhận: ";
  getline(cin, toUsername);

  User *recipient = userManager.findByUsername(toUsername);
  if (!recipient) {
    cout << "Không tìm thấy người nhận.\n";
    return;
  }

  cout << "Nhập số tiền cần chuyển: ";
  cin >> amount;
  cin.ignore();

  double totalBalance =
      currentUser->getWalletBalance() + recipient->getWalletBalance();

  bool success =
      txManager.transferPoints(*currentUser, *recipient, amount, totalBalance);

  if (success) {
    userManager.saveUsers();
    cout << "Chuyển tiền thành công!\n";
  } else {
    cout << "Chuyển tiền thất bại.\n";
  }
}

// Menu chức năng dành cho người dùng có quyền user
void showUserMenu(User *user, UserManager &userManager,
                  TransactionManager &txManager) {
  int choice;
  do {
    cout << "\n=== MENU NGƯỜI DÙNG ===" << endl;
    cout << "1. Xem thông tin tài khoản\n";
    cout << "2. Xem số dư ví\n";
    cout << "3. Cập nhật thông tin cá nhân\n";
    cout << "4. Đổi mật khẩu\n";
    cout << "5. Chuyển tiền\n";
    cout << "6. Lịch sử giao dịch\n";
    cout << "7. Đăng xuất\n";
    cout << "Chọn chức năng: ";
    cin >> choice;
    cin.ignore();
    this_thread::sleep_for(chrono::milliseconds(800));
    clearScreen();

    switch (choice) {
    case 1:
      user->display();
      break;
    case 2:
      user->displayWallet();
      break;
    case 3:
      user->updateInfo();
      userManager.saveUsers();
      break;
    case 4:
      user->changePasswordWithOTP();
      userManager.saveUsers();
      break;
    case 5:
      handleTransferMoney(user, userManager, txManager);
      break;
    case 6:
      txManager.showDetailedHistoryForUser(user->getUsername());
      break;
    case 7:
      cout << "Đăng xuất...\n";
      break;
    default:
      cout << "Lựa chọn không hợp lệ.\n";
    }
  } while (choice != 7);
}

// Menu chức năng dành cho người dùng có quyền admin
void showAdminMenu(User *user, UserManager &userManager,
                   AuthService &authService, TransactionManager &txManager) {
  int choice;
  do {
    cout << "\n=== MENU QUẢN TRỊ VIÊN ===" << endl;
    cout << "1. Xem danh sách người dùng\n";
    cout << "2. Tạo tài khoản người dùng mới\n";
    cout << "3. Cập nhật thông tin cá nhân\n";
    cout << "4. Đổi mật khẩu\n";
    cout << "5. Chuyển tiền\n";
    cout << "6. Cập nhật thông tin người dùng (chỉ quyền 'user')\n";
    cout << "7. Lịch sử giao dịch\n";
    cout << "8. Đăng xuất\n";
    cout << "Chọn chức năng: ";
    cin >> choice;
    cin.ignore();
    this_thread::sleep_for(chrono::milliseconds(800));
    clearScreen();

    switch (choice) {
    case 1:
      userManager.displayAllUsers();
      break;
    case 2:
      authService.registerUser();
      break;
    case 3:
      user->updateInfo();
      userManager.saveUsers();
      break;
    case 4:
      user->changePasswordWithOTP();
      userManager.saveUsers();
      break;
    case 5:
      handleTransferMoney(user, userManager, txManager);
      break;
    case 6:
      userManager.updateUserByUsername();
      break;
    case 7:
      txManager.showDetailedHistoryForUser(user->getUsername());
      break;
    case 8:
      cout << "Đăng xuất...\n";
      break;
    default:
      cout << "Lựa chọn không hợp lệ.\n";
    }
  } while (choice != 8);
}

// Hàm main – điểm khởi đầu của chương trình
int main() {
  UserManager userManager("data/users.csv");
  AuthService authService(userManager);
  TransactionManager txManager("data/transactions.csv");

  while (true) {
    int choice;
    cout << "\n=== HỆ THỐNG VÍ ĐIỆN TỬ ===" << endl;
    cout << "1. Đăng nhập\n";
    cout << "2. Đăng ký\n";
    cout << "3. Quên mật khẩu\n";
    cout << "4. Thoát\n";
    cout << "Chọn chức năng: ";
    cin >> choice;
    cin.ignore();
    this_thread::sleep_for(chrono::milliseconds(800));
    clearScreen();

    User *currentUser = nullptr;

    switch (choice) {
    case 1:
      currentUser = authService.login();
      if (currentUser) {
        if (currentUser->getRole() == "admin") {
          showAdminMenu(currentUser, userManager, authService, txManager);
        } else {
          showUserMenu(currentUser, userManager, txManager);
        }
      }
      break;
    case 2:
      authService.registerUser();
      break;
    case 3:
      authService.resetPassword();
      break;
    case 4:
      cout << "Tạm biệt!\n";
      return 0;
    default:
      cout << "Lựa chọn không hợp lệ.\n";
    }
  }

  return 0;
}
