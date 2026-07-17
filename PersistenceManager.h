#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <string>
#include "FurnitureManager.h"
#include "AccountManager.h"

class PersistenceManager {
public:
    // Luu trữ
    static bool saveAllData(const std::string& furnitureFile, const std::string& accountFile, 
                           const FurnitureManager& fManager, const AccountManager& aManager);
    
    // Tải dữ liệu
    static bool loadAllData(const std::string& furnitureFile, const std::string& accountFile, 
                           FurnitureManager& fManager, AccountManager& aManager);                   
};

#endif