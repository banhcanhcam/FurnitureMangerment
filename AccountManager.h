#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "Account.h"

class AccountManager {
private:
    std::map<std::string, UserAccount> accounts;

public:
    AccountManager();
    const std::map<std::string, UserAccount>& getAccounts() const { return accounts; }
    // Các hàm chức năng chính
    bool registerAccount(const std::string& username, const std::string& password, UserRole role, const std::string& phone = "");
    UserAccount* login(const std::string& username, const std::string& password);

    // Tìm tài khoản theo username (trả về nullptr nếu không có)
    UserAccount* findAccount(const std::string& username);

    // Lấy danh sách toàn bộ tài khoản khách hàng (dùng để hiển thị)
    std::vector<UserAccount> getAllCustomers() const;

    // Hàm xóa (Phần bạn đang cần)
    bool deleteCustomerAccount(const std::string& username);

    // File I/O
    bool loadAccountsFromFile(const std::string& filename);
    bool saveAccountsToFile(const std::string& filename) const;
};

#endif // ACCOUNT_MANAGER_H