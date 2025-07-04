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

  cout << "Nhập tên tài khoản người nhận: ";
  getline(cin, toUsername);
  if (currentUser->getUsername() == toUsername) {
    cout << "Không thể thực hiện hành động này trên cùng tài khoản." << endl;
    return;
  }

  User *recipient = userManager.findByUsername(toUsername);
  if (!recipient) {
    cout << "Không tìm thấy người nhận." << endl;
    return;
  }

  cout << "Nhập số tiền cần chuyển: ";
  cin >> amount;
  cin.ignore();

  double totalBalance =
      currentUser->getWalletBalance() + recipient->getWalletBalance();

  txManager.transferPoints(*currentUser, *recipient, amount, totalBalance);
}

// Menu chức năng dành cho người dùng có quyền user
void showUserMenu(User *user, UserManager &userManager,
                  TransactionManager &txManager) {
  int choice;
  do {
    cout << "\n=== MENU NGƯỜI DÙNG ===" << endl;
    cout << "1. Xem thông tin tài khoản" << endl;
    cout << "2. Xem số dư ví" << endl;
    cout << "3. Cập nhật thông tin cá nhân" << endl;
    cout << "4. Đổi mật khẩu" << endl;
    cout << "5. Chuyển tiền" << endl;
    cout << "6. Lịch sử giao dịch" << endl;
    cout << "7. Đăng xuất" << endl;
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
      user->updateInfo(user->getEmail());
      userManager.saveUsers();
      break;
    case 4:
      user->changePasswordWithOTP(user->getEmail());
      userManager.saveUsers();
      break;
    case 5:
      handleTransferMoney(user, userManager, txManager);
      break;
    case 6:
      txManager.showDetailedHistoryForUser(user->getUsername());
      break;
    case 7:
      cout << "Đăng xuất..." << endl;
      break;
    default:
      cout << "Lựa chọn không hợp lệ." << endl;
    }
  } while (choice != 7);
}

// Menu chức năng dành cho người dùng có quyền admin
void showAdminMenu(User *user, UserManager &userManager,
                   AuthService &authService, TransactionManager &txManager) {
  int choice;
  do {
    cout << "\n=== MENU QUẢN TRỊ VIÊN ===" << endl;
    cout << "1. Xem danh sách người dùng" << endl;
    cout << "2. Tạo tài khoản người dùng mới" << endl;
    cout << "3. Cập nhật thông tin cá nhân" << endl;
    cout << "4. Đổi mật khẩu" << endl;
    cout << "5. Chuyển tiền" << endl;
    cout << "6. Cập nhật thông tin người dùng (chỉ quyền 'user')" << endl;
    cout << "7. Lịch sử giao dịch" << endl;
    cout << "8. Đăng xuất" << endl;
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
      user->updateInfo(user->getEmail());
      userManager.saveUsers();
      break;
    case 4:
      user->changePasswordWithOTP(user->getEmail());
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
      cout << "Đăng xuất..." << endl;
      break;
    default:
      cout << "Lựa chọn không hợp lệ." << endl;
    }
  } while (choice != 8);
}

// Hàm main – điểm khởi đầu của chương trình
int main() {
  UserManager userManager("data/users.csv");
  AuthService authService(userManager);
  TransactionManager txManager("data/transactions.csv");

  while (true) {
    string input;
    int choice = -1;

    cout << "\n=== HỆ THỐNG VÍ ĐIỆN TỬ ===" << endl;
    cout << "1. Đăng nhập" << endl;
    cout << "2. Đăng ký" << endl;
    cout << "3. Quên mật khẩu" << endl;
    cout << "4. Thoát" << endl;
    cout << "Chọn chức năng: ";
    getline(cin, input);

    stringstream ss(input);
    if (!(ss >> choice)) {
      cout << "Vui lòng nhập một số hợp lệ." << endl;
      continue;
    }

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
      cout << "Tạm biệt!" << endl;
      return 0;
    default:
      cout << "Lựa chọn không hợp lệ. Vui lòng chọn từ 1 đến 4." << endl;
    }
  }

  return 0;
}
