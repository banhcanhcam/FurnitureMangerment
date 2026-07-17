#include "PersistenceManager.h"
#include "DerivedFurniture.h"
#include <sstream>
#include <fstream>
#include <iostream>

// Trong PersistenceManager.cpp
bool PersistenceManager::saveAllData(const std::string& furnitureFile, const std::string& accountFile, 
                                     const FurnitureManager& fManager, const AccountManager& aManager) {
    // 1. Lưu dữ liệu Furniture
    std::ofstream fOut(furnitureFile);
    if (fOut.is_open()) {
        const auto& inventory = fManager.getInventory(); // Lấy dữ liệu qua hàm get
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            const auto& list = it->second;
            for (size_t i = 0; i < list.size(); ++i) {
                auto f = list[i];
                fOut << f->getFurnitureID() << "," << f->getName() << "," << f->getPrice() << "," 
                     << f->getQuantity() << "," << (int)it->first << "," << f->getWidth() << "," 
                     << f->getHeight() << "," << f->getDepth() << "," << f->getBaseColor() << "\n";
            }
        }
        fOut.close();
    }
    std::ofstream aOut(accountFile);
    if (!aOut.is_open()) return false;
    
    // Giả sử getAccounts() trả về std::map<std::string, UserAccount>
    const auto& accounts = aManager.getAccounts();
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        const std::string& username = it->first;
        const UserAccount& acc = it->second;
        // Lưu theo định dạng: username,password,role
        aOut << username << "," << acc.password << "," << (int)acc.role << "\n";
    }
    aOut.close();
    
    return true;
}
bool PersistenceManager::loadAllData(const std::string& furnitureFile, const std::string& accountFile, 
                           FurnitureManager& fManager, AccountManager& aManager){
    std::ifstream fIn(furnitureFile);
    if (!fIn.is_open()) return false;

    std::string line;
    while (std::getline(fIn, line)) {
        std::stringstream ss(line);
        std::string id, name, color, temp;
        double w, h, d, price;
        int qty, typeInt;

        // Đọc dữ liệu theo đúng thứ tự bạn đã lưu
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        ss >> price; ss.ignore();
        ss >> qty; ss.ignore();
        ss >> typeInt; ss.ignore();
        ss >> w; ss.ignore();
        ss >> h; ss.ignore();
        ss >> d; ss.ignore();
        std::getline(ss, color);

        // Tạo lại đối tượng và thêm vào quản lý[cite: 8, 9]
        MaterialType type = static_cast<MaterialType>(typeInt);
        std::shared_ptr<FurnitureBase> f;
        if (type == MaterialType::WOOD) 
        f = std::make_shared<WoodFurniture>(id, w, h, d, color, name, price, qty);
        // ... (tương tự cho các loại nội thất khác)
        
        fManager.addFurniture(f); //[cite: 8]
    }
    fIn.close();
    return true;
}