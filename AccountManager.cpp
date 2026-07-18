#include "AccountManager.h"
#include <iostream>

// Khởi tạo AccountManager (Có thể tạo sẵn 1 tài khoản admin gốc để hệ thống không bị khóa)
AccountManager::AccountManager() {
    // Tài khoản mặc định ban đầu
    accounts["admin"] = {"admin", "1", UserRole::ADMIN, ""};
}

// Chức năng Đăng ký
bool AccountManager::registerAccount(const std::string& username, const std::string& password, UserRole role, const std::string& phone) {
    if (accounts.find(username) != accounts.end()) {
        return false;  // không in gì
    }
    accounts[username] = {username, password, role, phone};
    return true;
}

// Chức năng Đăng nhập
UserAccount* AccountManager::login(const std::string& username, const std::string& password) {
    auto it = accounts.find(username);
    
    // Nếu tìm thấy username và password khớp
    if (it != accounts.end() && it->second.password == password) {
        return &(it->second); // Trả về con trỏ trỏ tới tài khoản đó
    }
    
    // Trả về nullptr nếu sai tài khoản hoặc mật khẩu
    return nullptr; 
}

// Tìm tài khoản theo username
UserAccount* AccountManager::findAccount(const std::string& username) {
    auto it = accounts.find(username);
    if (it != accounts.end()) return &(it->second);
    return nullptr;
}

// Lấy danh sách toàn bộ khách hàng (dùng cho tính năng hiển thị)
std::vector<UserAccount> AccountManager::getAllCustomers() const {
    std::vector<UserAccount> result;
    for (const auto& pair : accounts) {
        if (pair.second.role == UserRole::CUSTOMER) {
            result.push_back(pair.second);
        }
    }
    return result;
}

// Stub cho File I/O (đã có PersistenceManager xử lý thực tế)
bool AccountManager::loadAccountsFromFile(const std::string& filename) { return true; }
bool AccountManager::saveAccountsToFile(const std::string& filename) const { return true; }
// xóa tài khoản khách hàng.
bool AccountManager::deleteCustomerAccount(const std::string& username) {
    // Chỉ xóa nếu đó là Customer, không cho phép xóa Admin khác (để bảo mật)
    if (accounts.count(username) && accounts[username].role == UserRole::CUSTOMER) {
        accounts.erase(username);
        return true;
    }
    return false;
}