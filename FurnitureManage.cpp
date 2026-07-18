#include "FurnitureManager.h"
#include "Validation.h"
#include "ProductValidation.h"

#include <iostream>

void FurnitureManager::addFurniture(std::shared_ptr<FurnitureBase> furniture) {
    // Tìm xem ID đã tồn tại chưa
    auto existingItem = searchById(furniture->getFurnitureID());
    
    if (existingItem != nullptr) {
        // ID đã tồn tại: Cập nhật dữ liệu
        std::cout << "The ID is duplicated, the data will be updated\n";
        
        // Cập nhật thông tin mới vào object cũ
        existingItem->setDimensions(furniture->getWidth(), furniture->getHeight(), furniture->getDepth());
        existingItem->setBaseColor(furniture->getBaseColor());
        // Nếu lớp FurnitureBase của bạn có các hàm set khác như setName, setPrice thì gọi ở đây
        
        return; // Kết thúc hàm sau khi đã cập nhật
    }
    
    // Nếu chưa tồn tại, thêm mới như bình thường
    inventory[furniture->getMaterialType()].push_back(furniture);
}

std::shared_ptr<FurnitureBase> FurnitureManager::searchById(const std::string& id) const {
    for (const auto& pair : inventory) {
        for (const auto& item : pair.second) {
            if (item->getFurnitureID() == id) {
                return item;
            }
        }
    }
    return nullptr; // Khong tim thay
}

void FurnitureManager::displayAll() const {
    for (const auto& pair : inventory) {
        for (const auto& item : pair.second) {
            std::string matStr;
            switch (item->getMaterialType()) {
                case MaterialType::WOOD: matStr = "Wood"; break;
                case MaterialType::METAL: matStr = "Metal"; break;
                case MaterialType::ALUMINUM: matStr = "Aluminum"; break;
            }
            std::cout << "ID: " << item->getFurnitureID() 
                      << " | Name: " << item->getName()     
					  << " | Material: " << matStr         
                      << " | Price: " << item->getPrice()            
                      << " | Quantity: " << item->getQuantity()     
                      << " | Color: " << item->getBaseColor() 
                      << " | WxHxD: " << item->getWidth() << "x" 
                      << item->getHeight() << "x" << item->getDepth() << "\n";
        }
    }
}

// Logic File I/O se duoc viet chi tiet trong Phan 3 (Phan tich Du lieu & I/O)
bool FurnitureManager::loadFromFile(const std::string& filename) { return true; }
bool FurnitureManager::saveToFile(const std::string& filename) const { return true; }
//xóa sản phẩm theo ID.
bool FurnitureManager::deleteProduct(const std::string& id) {
    for (auto& pair : inventory) {
        auto& vec = pair.second;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (vec[i]->getFurnitureID() == id) {
                return safeDelete(vec, i); // dùng hàm này
            }
        }
    }
    return false;
}