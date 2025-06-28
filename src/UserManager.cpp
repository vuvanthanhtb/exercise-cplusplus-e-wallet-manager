#include "../include/UserManager.h"
#include "../include/utils/PasswordUtil.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

UserManager::UserManager(const string &filename) : dataFile(filename) {
	loadUsers(); // Tải dữ liệu người dùng từ file khi khởi tạo
}

// Tải danh sách người dùng từ file CSV
void UserManager::loadUsers() {
	users.clear();
	ifstream file(dataFile);
	string line;
	while (getline(file, line)) {
		User user = User::fromCSV(line);
		users.push_back(user);
	}
}

// Lưu danh sách người dùng về file CSV
void UserManager::saveUsers() const {
	ofstream file(dataFile);
	for (const auto &user : users) {
		file << user.toCSV() << "\n";
	}
}

// Xác thực đăng nhập bằng username và mật khẩu
User *UserManager::authenticate(const string &username,
																const string &password) {
	for (auto &user : users) {
		if (user.getUsername() == username &&
				PasswordUtil::verifyPassword(
						password,
						user.toCSV().substr(
								user.toCSV().find(username) + username.length() + 1, 64))) {
			return &user; // Trả về con trỏ nếu đúng tài khoản
		}
	}
	return nullptr;
}

// Thêm người dùng mới (nếu username chưa tồn tại)
bool UserManager::addUser(const User &user) {
	for (const auto &u : users) {
		if (u.getUsername() == user.getUsername()) {
			return false; // Username đã tồn tại
		}
	}

	users.push_back(user);
	saveUsers(); // Lưu lại danh sách mới
	return true;
}

// Tìm người dùng theo username
User *UserManager::findByUsername(const string &username) {
	for (auto &user : users) {
		if (user.getUsername() == username) {
			return &user;
		}
	}
	return nullptr;
}

// Tìm người dùng theo email
User *UserManager::findByEmail(const string &email) {
	for (auto &user : users) {
		if (user.verifyEmail(email)) {
			return &user;
		}
	}
	return nullptr;
}

// Đặt lại mật khẩu theo email (mật khẩu đã được mã hóa)
bool UserManager::resetPassword(const string &email,
																const string &newHashedPassword) {
	for (auto &user : users) {
		if (user.verifyEmail(email)) {
			user.setPassword(newHashedPassword);
			saveUsers();
			return true;
		}
	}
	return false;
}

// Hiển thị tất cả người dùng dưới dạng bảng
void UserManager::displayAllUsers() const {
	cout << "\nDanh sách tất cả người dùng:\n";
	cout << left << setw(40) << "ID" << setw(20) << "Họ tên" << setw(25)
			 << "Tên đăng nhập" << setw(25) << "Email" << setw(10) << "Quyền" << right
			 << setw(10) << "Số dư" << endl;
	cout << string(125, '-') << endl;

	for (const auto &user : users) {
		cout << left << setw(40) << user.getId() << setw(20) << user.getFullname()
				 << setw(20) << user.getUsername() << setw(25) << user.getEmail()
				 << setw(10) << user.getRole() << right << setw(10) << fixed
				 << setprecision(2) << user.getWalletBalance() << endl;
	}
}

// Cập nhật thông tin người dùng theo tên đăng nhập (chỉ dành cho admin)
void UserManager::updateUserByUsername() {
	string username;
	cout << "Nhập tên tài khoản cần cập nhật: ";
	getline(cin, username);

	for (auto &user : users) {
		if (user.getUsername() == username) {
			if (user.getRole() != "user") {
				cout << "Chỉ có thể cập nhật người dùng có quyền 'user'." << endl;
				return;
			}
			
			string sendMail = user.getEmail();

			cout << "Đang cập nhật thông tin cho: " << username << endl;
			user.updateInfo(sendMail);
			saveUsers();
			return;
		}
	}

	cout << "Không tìm thấy người dùng." << endl;
}
