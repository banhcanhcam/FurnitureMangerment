#ifndef FURNITURE_MANAGER_H
#define FURNITURE_MANAGER_H

#include <map>
#include <vector>
#include <memory>
#include <string>
#include "FurnitureBase.h"

class FurnitureManager {
private:
    std::map<MaterialType, std::vector<std::shared_ptr<FurnitureBase>>> inventory;

public:
    FurnitureManager() = default;
    
const std::map<MaterialType, std::vector<std::shared_ptr<FurnitureBase>>>& getInventory() const {
        return inventory;
    }

    // CRUD và Tìm kiếm
    void addFurniture(std::shared_ptr<FurnitureBase> furniture);
    std::shared_ptr<FurnitureBase> searchById(const std::string& id) const;
    void displayAll() const;
    bool updateFurniture(const std::string& id, const std::string& name, double price, int qty, double w, double h, double d, const std::string& color);
    
    // --- THÊM DÒNG NÀY VÀO TRONG CLASS ---
    bool deleteProduct(const std::string& id); 
    // --------------------------------------

    // File I/O
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
};
#endif // FURNITURE_MANAGER_H