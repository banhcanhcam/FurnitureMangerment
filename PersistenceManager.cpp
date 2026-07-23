#include "PersistenceManager.h"
#include "DerivedFurniture.h"
#include <sstream>
#include <fstream>
#include <iostream>

bool PersistenceManager::saveAllData(const std::string& furnitureFile,
                                     const std::string& adminFile,
                                     const std::string& customerFile,
                                     const std::string& orderFile,
                                     const std::string& invoiceFile,
                                     const FurnitureManager& fManager,
                                     const AccountManager& aManager,
                                     const OrderManager& oManager,
                                     bool silent) {
    bool success = true;

    // Lưu furniture
    std::ofstream fOut(furnitureFile);
    if (!fOut.is_open()) {
        std::cerr << "Error: Cannot open " << furnitureFile << " for writing.\n";
        success = false;
    } else {
        const auto& inventory = fManager.getInventory();
        for (const auto& pair : inventory) {
            for (const auto& f : pair.second) {
                fOut << f->getFurnitureID() << ","
                     << f->getName() << ","
                     << f->getPrice() << ","
                     << f->getQuantity() << ","
                     << static_cast<int>(pair.first) << ","
                     << f->getWidth() << ","
                     << f->getHeight() << ","
                     << f->getDepth() << ","
                     << f->getBaseColor() << "\n";
            }
        }
        fOut.close();
        if (!silent) std::cout << "[OK] Saved " << furnitureFile << "\n";
    }

    // Lưu tài khoản: tách riêng Admin và Customer ra 2 file khác nhau
    std::ofstream adminOut(adminFile);
    if (!adminOut.is_open()) {
        std::cerr << "Error: Cannot open " << adminFile << " for writing.\n";
        success = false;
    }

    std::ofstream customerOut(customerFile);
    if (!customerOut.is_open()) {
        std::cerr << "Error: Cannot open " << customerFile << " for writing.\n";
        success = false;
    }

    if (adminOut.is_open() && customerOut.is_open()) {
        const auto& accounts = aManager.getAccounts();
        for (const auto& pair : accounts) {
            const UserAccount& acc = pair.second;
            if (acc.role == UserRole::ADMIN) {
                adminOut << acc.username << "," << acc.password << "\n";
            } else {
                customerOut << acc.username << "," << acc.password << "," << acc.phoneNumber << "\n";
            }
        }
        if (!silent) {
            std::cout << "[OK] Saved " << adminFile << "\n";
            std::cout << "[OK] Saved " << customerFile << "\n";
        }
    }
    adminOut.close();
    customerOut.close();

        // Lưu đơn hàng
    if (!oManager.saveToFile(orderFile)) {
        success = false;
    } else if (!silent) {
        std::cout << "[OK] Saved " << orderFile << "\n";
    }

    // ---- LƯU INVOICE (đặt trong hàm) ----
    if (!oManager.saveInvoicesToFile(invoiceFile)) {
        success = false;
        std::cerr << "Error: Cannot save invoices.\n";
    } else if (!silent) {
        std::cout << "[OK] Saved " << invoiceFile << "\n";
    }

    return success;   // ← Chỉ một lần duy nhất
}
bool PersistenceManager::loadAllData(const std::string& furnitureFile,
                                     const std::string& adminFile,
                                     const std::string& customerFile,
                                     const std::string& orderFile,
                                     const std::string& invoiceFile,
                                     FurnitureManager& fManager,
                                     AccountManager& aManager,
                                     OrderManager& oManager) {
    bool success = true;

    // Load furniture (phải load trước Order vì Order cần tra cứu Furniture theo ID)
    std::ifstream fIn(furnitureFile);
    if (!fIn.is_open()) {
        std::cerr << "Warning: Cannot open " << furnitureFile << " for reading. Starting with empty inventory.\n";
        success = false;
    } else {
        std::string line;
        while (std::getline(fIn, line)) {
            std::stringstream ss(line);
            std::string id, name, color, priceStr, qtyStr, typeStr, wStr, hStr, dStr;

            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, qtyStr, ',');
            std::getline(ss, typeStr, ',');
            std::getline(ss, wStr, ',');
            std::getline(ss, hStr, ',');
            std::getline(ss, dStr, ',');
            std::getline(ss, color);

            if (id.empty() || name.empty()) continue;

            try {
                double price = std::stod(priceStr);
                int qty = std::stoi(qtyStr);
                int typeInt = std::stoi(typeStr);
                double w = std::stod(wStr);
                double h = std::stod(hStr);
                double d = std::stod(dStr);

                MaterialType type = static_cast<MaterialType>(typeInt);
                std::shared_ptr<FurnitureBase> f;
                if (type == MaterialType::WOOD)
                    f = std::make_shared<WoodFurniture>(id, w, h, d, color, name, price, qty);
                else if (type == MaterialType::METAL)
                    f = std::make_shared<MetalFurniture>(id, name, w, h, d, price, qty, color);
                else if (type == MaterialType::ALUMINUM)
                    f = std::make_shared<AluminumFurniture>(id, name, w, h, d, price, qty, color);
                else {
                    std::cerr << "Unknown material type " << typeInt << " for ID " << id << "\n";
                    continue;
                }
                fManager.addFurniture(f);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing line: " << line << "\n";
            }
        }
        fIn.close();
    }

    // Load tài khoản Admin
    std::ifstream adminIn(adminFile);
    if (!adminIn.is_open()) {
        std::cerr << "Warning: Cannot open " << adminFile << " for reading. Starting with default admin account.\n";
        success = false;
    } else {
        std::string line;
        while (std::getline(adminIn, line)) {
            std::stringstream ss(line);
            std::string username, password;
            std::getline(ss, username, ',');
            std::getline(ss, password);

            if (username.empty()) continue;
            aManager.registerAccount(username, password, UserRole::ADMIN);
        }
        adminIn.close();
    }

    // Load tài khoản Customer (kèm số điện thoại)
    std::ifstream customerIn(customerFile);
    if (!customerIn.is_open()) {
        std::cerr << "Warning: Cannot open " << customerFile << " for reading. Starting with no customer accounts.\n";
        success = false;
    } else {
        std::string line;
        while (std::getline(customerIn, line)) {
            std::stringstream ss(line);
            std::string username, password, phone;
            std::getline(ss, username, ',');
            std::getline(ss, password, ',');
            std::getline(ss, phone);

            if (username.empty()) continue;
            aManager.registerAccount(username, password, UserRole::CUSTOMER, phone);
        }
        customerIn.close();
    }

    // Load đơn hàng (phải load sau Furniture)
    if (!oManager.loadFromFile(orderFile, fManager)) {
        success = false;
    }
    // ---- LOAD INVOICE ----
    if (!oManager.loadInvoicesFromFile(invoiceFile)) {
        success = false;
        std::cerr << "Warning: Cannot load invoices.\n";
    }

    return success;
}