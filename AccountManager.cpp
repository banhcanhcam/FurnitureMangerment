#include "AccountManager.h"
#include <iostream>

// Khởi tạo AccountManager (Có thể tạo sẵn 1 tài khoản admin gốc để hệ thống không bị khóa)
AccountManager::AccountManager() {
    // Tài khoản mặc định ban đầu
    accounts["admin"] = {"admin", "1", UserRole::ADMIN};
}

// Chức năng Đăng ký
bool AccountManager::registerAccount(const std::string& username, const std::string& password, UserRole role) {
    // Kiểm tra xem username đã tồn tại trong map chưa
    if (accounts.find(username) != accounts.end()) {
        std::cout << "invalid username '" << username << "' already exist please choose another name.\n";
        return false;
    }

    // Nếu chưa tồn tại, thêm vào map
    accounts[username] = {username, password, role};
    
    std::string roleStr = (role == UserRole::ADMIN) ? "ADMIN" : "CUSTOMER";
    std::cout << "success create a new account " << roleStr << " new: " << username << "\n";
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

// Stub cho File I/O (Sẽ code sau)
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