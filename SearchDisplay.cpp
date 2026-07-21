#include "SearchDisplay.h"
#include "ProductValidation.h"
#include <algorithm>

namespace SearchDisplay {

    
    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return ""; 
        size_t end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

    std::string toLower(const std::string& s) {
        std::string r = s;
        std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c) { return std::tolower(c); });
        return r;
    }

   
    void printFurnitureHeader() {
        std::cout << std::left
                  << std::setw(12) << "ID"
                  << std::setw(15) << "Name"
                  << std::setw(10) << "Price"
                  << std::setw(10) << "Quantity"
                  << std::setw(10) << "Material"
                  << std::setw(15) << "Size(WxHxD)"
                  << std::setw(15) << "Color"
                  << "\n" << std::string(52, '-') << "\n";
    }

    void printFurniture(const std::shared_ptr<FurnitureBase>& item) {
    if (!item) return;

    // Chuyển MaterialType thành chuỗi
    std::string matStr;
    switch (item->getMaterialType()) {
        case MaterialType::WOOD: matStr = "Wood"; break;
        case MaterialType::METAL: matStr = "Metal"; break;
        case MaterialType::ALUMINUM: matStr = "Aluminum"; break;
        default: matStr = "Unknown";
    }

    
    std::string sizeStr = std::to_string((int)item->getWidth()) + "x" + 
                          std::to_string((int)item->getHeight()) + "x" + 
                          std::to_string((int)item->getDepth());

    
    std::cout << std::left
              << std::setw(12) << item->getFurnitureID()
              << std::setw(15) << item->getName()        
              << std::setw(10) << item->getPrice()       
              << std::setw(10) << item->getQuantity()   
              << std::setw(10) << matStr
              << std::setw(15) << sizeStr
              << std::setw(15) << item->getBaseColor()
              << "\n";
    }
    void displayAllFurniture(const FurnitureManager& fm) {
        // Lấy toàn bộ dữ liệu từ Manager
        const auto& inventory = fm.getInventory();
        if (isStoreEmpty(inventory)) {
            return; // isStoreEmpty() already prints the error message
        }
        printFurnitureHeader();
        for (const auto& pair : inventory) {
            for (const auto& item : pair.second) {
                printFurniture(item);
            }
        }
    }

    void displayAllFurnitureSortedByPrice(const FurnitureManager& fm) {
        const auto& inventory = fm.getInventory();
        if (isStoreEmpty(inventory)) {
            return; // isStoreEmpty() already prints the error message
        }

        // Gộp toàn bộ các nhóm vật liệu thành 1 danh sách để sort xuyên suốt kho
        FurnitureList allItems;
        for (const auto& pair : inventory) {
            for (const auto& item : pair.second) {
                allItems.push_back(item);
            }
        }

        // Dùng lại hàm updateAndSort() đã có sẵn (đã sửa để sort theo giá tăng dần)
        updateAndSort(allItems);

        printFurnitureHeader();
        for (const auto& item : allItems) {
            printFurniture(item);
        }
    }

    // ---- Chuyển OrderStatus thành chuỗi hiển thị ----
    std::string orderStatusToString(OrderStatus status) {
        switch (status) {
            case OrderStatus::PENDING: return "PENDING";
            case OrderStatus::IN_PROGRESS: return "IN_PROGRESS";
            case OrderStatus::COMPLETED: return "COMPLETED";
        }
        return "UNKNOWN";
    }

    void printOrderHeader() {
        std::cout << std::left
                  << std::setw(10) << "OrderID"
                  << std::setw(10) << "Furniture"
                  << std::setw(15) << "Carpenter"
                  << std::setw(12) << "Start Date"
                  << std::setw(13) << "Phone"
                  << std::setw(13) << "Status"
                  << std::setw(15) << "Labor Cost" << "\n"
                  << std::string(88, '-') << "\n";
    }

    void printOrder(const Order& o) {
        std::cout << std::left
                  << std::setw(10) << o.getOrderID()
                  << std::setw(10) << o.getFurnitureID()
                  << std::setw(15) << o.getCarpenterName()
                  << std::setw(12) << o.getStartDate().toString()
                  << std::setw(13) << o.getPhoneNumber()
                  << std::setw(13) << orderStatusToString(o.getStatus())
                  << std::setw(15) << std::fixed << std::setprecision(2) << o.getLaborCost() << "\n";
    }

    void displayOrdersByStatus(const OrderManager& om, OrderStatus status) {
        const auto& orders = om.getOrders();
        bool found = false;

        printOrderHeader();
        for (const auto& o : orders) {
            if (o.getStatus() == status) {
                printOrder(o);
                found = true;
            }
        }
        if (!found) std::cout << "No orders found for this status.\n";
    }

    void displayAllOrders(const OrderManager& om) {
        const auto& orders = om.getOrders();
        if (orders.empty()) {
            std::cout << "No orders in the system yet.\n";
            return;
        }
        printOrderHeader();
        for (const auto& o : orders) {
            printOrder(o);
        }
    }

    void displayOrdersByCustomer(const OrderManager& om, const std::string& username) {
        const auto& orders = om.getOrders();
        bool found = false;

        printOrderHeader();
        for (const auto& o : orders) {
            if (o.getCustomerUsername() == username) {
                printOrder(o);
                found = true;
            }
        }
        if (!found) std::cout << "You have no orders yet.\n";
    }

    // ---- TÌM KIẾM ----
    std::shared_ptr<FurnitureBase> searchFurnitureById(const FurnitureManager& fm, const std::string& id) {
        std::string cleanId = trim(id);
        const auto& inventory = fm.getInventory();
        for (const auto& pair : inventory) {
            for (const auto& item : pair.second) {
                if (item->getFurnitureID() == cleanId) {
                    return item;
                }
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<FurnitureBase>> searchFurnitureByColor(const FurnitureManager& fm, const std::string& keyword) {
        std::vector<std::shared_ptr<FurnitureBase>> results;
        std::string target = toLower(trim(keyword));
        if (target.empty()) return results;

        const auto& inventory = fm.getInventory();
        for (const auto& pair : inventory) {
            for (const auto& item : pair.second) {
                if (toLower(item->getBaseColor()).find(target) != std::string::npos) {
                    results.push_back(item);
                }
            }
        }
        return results;
    }
    std::vector<std::shared_ptr<FurnitureBase>> searchFurnitureByName(const FurnitureManager& fm, const std::string& name) {
        std::vector<std::shared_ptr<FurnitureBase>> results;
        std::string target = toLower(name); 
        
        const auto& inventory = fm.getInventory(); 
        for (const auto& pair : inventory) {
            for (const auto& item : pair.second) {
                std::string itemName = toLower(item->getName());
                if (itemName.find(target) != std::string::npos) {
                    results.push_back(item);
                }
            }
        }
        return results;
    }

    // ---- SẮP XẾP ----
    void sortOrdersByCostAscending(std::vector<Order>& orders) {
        // Dùng stable_sort để giữ nguyên thứ tự đơn hàng nếu có cùng giá trị
        std::stable_sort(orders.begin(), orders.end(), 
            [](const Order& a, const Order& b) {
                return a.getLaborCost() < b.getLaborCost();
            });
    }

} // namespace SearchDisplay