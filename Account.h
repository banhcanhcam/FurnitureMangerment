#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

// Định nghĩa các quyền (Role)
enum class UserRole {
    ADMIN,
    CUSTOMER
};

// Cấu trúc lưu trữ thông tin một tài khoản
struct UserAccount {
    std::string username;
    std::string password;
    UserRole role;
    std::string phoneNumber; // Chỉ áp dụng cho tài khoản CUSTOMER, để trống với ADMIN
};

#endif // ACCOUNT_H