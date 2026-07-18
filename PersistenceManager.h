#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <string>
#include "FurnitureManager.h"
#include "AccountManager.h"
#include "OrderManager.h"

class PersistenceManager {
public:
    // Lưu trữ: tách riêng admin.txt và customer.txt, thêm orderFile
    static bool saveAllData(const std::string& furnitureFile,
                           const std::string& adminFile,
                           const std::string& customerFile,
                           const std::string& orderFile,
                           const FurnitureManager& fManager,
                           const AccountManager& aManager,
                           const OrderManager& oManager);

    // Tải dữ liệu
    static bool loadAllData(const std::string& furnitureFile,
                           const std::string& adminFile,
                           const std::string& customerFile,
                           const std::string& orderFile,
                           FurnitureManager& fManager,
                           AccountManager& aManager,
                           OrderManager& oManager);
};

#endif