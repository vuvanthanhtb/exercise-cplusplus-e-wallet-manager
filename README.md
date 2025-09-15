# Tóm tắt bài tập E-Wallet
Dự án mô phông hệ thống E-Wallet (ví điện tử) hoạt động dựa trên giao diện console, cho phép người dùng quản lý tài khoản, chuyển tiền, và theo dõi giao dịch.

## Cài đặt thư viện gửi mail

#### macOS
```bsh
brew install curl
```

#### Ubuntu / Debian
```bsh
sudo apt update
sudo apt install libcurl4-openssl-dev
```

#### Thông tin mail
- Sử dụng App Password, được tạo [tại đây](https://myaccount.google.com/apppasswords)
- File: MailUtil.cpp nhập thông tin FROM_EMAIL & APP_PASSWORD để thực hiện chức năng gửi mail

#### Windows
```bsh
vcpkg install curl
```

## Chức năng chính

### 1. Đăng nhập (Login) (Vũ Văn Thanh)
- Xác thực dựa trên username và password.
- Mã hóa password bằng `PasswordUtil`.

### 2. Đăng ký tài khoản (Register) (Vũ Văn Thanh)
- Tạo user mới với họ tên, email, username, password.
- Phân quyền: user/admin.
- Kiểm tra username trùng lặp.

### 3. Quên mật khẩu (Reset Password) (Vũ Văn Thanh)
- Tìm user theo email.
- Cho nhập password mới và lưu vào database.

### 4. Giao diện User (Đỗ Hữu Tự)
- Xem thông tin tài khoản.
- Xem số dư ví.
- Cập nhật họ tên/email (OTP).
- Đổi mật khẩu (OTP + timeout).
- Chuyển tiền cho user khác.
- Xem lịch sử giao dịch (đi/nhận).

### 5. Giao diện Admin (Đỗ Hữu Tự)
- Xem danh sách tất cả user.
- Đăng ký user mới.
- Cập nhật thông tin và đổi password (admin).
- Chuyển tiền.
- Cập nhật thông tin user theo username (gửi OTP).

### 6. Giao dịch chuyển tiền (OTP) (Đặng Anh Đức)
- Ghi lại giao dịch vào file CSV.
- Kiểm tra số dư người gửi, và ràng buộc tổng số dư sau khi chuyển không thay đổi.
- Ghi nhật ký chi tiết.

### 7. OTP và Timeout (Đặng Anh Đức)
- OTP sinh ngẫu nhiên cho xác minh.
- OTP timeout sau 60 giây khi đổi mật khẩu.

### 8. Lịch sử giao dịch
- Hiển thị danh sách giao dịch chuyển/đã nhận với thời gian.

### 9. Xóa màn hình sau mỗi tính năng
- Sử dụng `system("clear")` trên Linux/macOS hoặc `system("cls")` trên Windows.

## Cấu trúc file
- `main.cpp` - Điểm khởi đầu và giao diện menu.
- `UserManager.*` - Quản lý danh sách user.
- `User.*` - Lớp đại diện user.
- `AuthService.*` - Xử lý login, đăng ký, reset.
- `TransactionManager.*` - Quản lý giao dịch chuyển tiền.
- `PasswordUtil.*` - Hash, verify password.
- `OTPUtil.*` - Sinh mã OTP.
- `UUIDUtil.*` - Sinh ID duy nhất.

## File dữ liệu
- `data/users.csv` - Lưu user.
- `data/transactions.csv` - Lưu giao dịch chuyển tiền.

---
