#include "OrderManager.h"
#include "ProductValidation.h"
#include <iostream>
#include <fstream>
#include <sstream>

void OrderManager::createOrder(const std::string& orderId, const std::string& fId, 
                               const std::string& carpenter, const Date& date, 
                               int days, const FurnitureManager& fManager, const std::string& username,
                               const std::string& phoneNumber) {

    if (isStoreEmpty(fManager.getInventory())) {
        return; // isStoreEmpty() already prints the error message
    }

    std::shared_ptr<FurnitureBase> linkedFurniture = fManager.searchById(fId);

    if (linkedFurniture == nullptr) {
        std::cout << "Error: Furniture ID " << fId << " not found.\n";
        return;
    }

    Order newOrder(orderId, carpenter, date, days, fId, linkedFurniture, username, phoneNumber);
    
    newOrder.setLaborCost(calculatePrice(newOrder));
    orders.push_back(newOrder);
    std::cout << "Order " << orderId << " created successfully.\n";
}

double OrderManager::calculatePrice(const Order& o) const {
    auto f = o.getFurniture();
    if (!f) return 0.0;

    // Cong thuc tinh dien tich be mat: 2 * (W*H + W*D + H*D)
    double area = 2 * (f->getWidth() * f->getHeight() + 
                       f->getWidth() * f->getDepth() + 
                       f->getHeight() * f->getDepth());
    
    // Tính đa hình (Polymorphism) phát huy tác dụng tại đây
    double matRate = f->getMaterialRate(); 
    double laborRate = 150000.0; // 150,000 VND / ngày (như trong báo cáo)
    
    return (area * matRate) + (laborRate * o.getEstimatedDays());
}

void OrderManager::trackByStatus(OrderStatus status) const {
    for (const auto& o : orders) {
        if (o.getStatus() == status) {
            std::cout << "Order ID: " << o.getOrderID() 
                      << " | Carpenter: " << o.getCarpenterName() 
                      << " | Price: " << o.getLaborCost() << "\n";
        }
    }
}

const std::vector<Order>& OrderManager::getOrders() const { return orders; }

// ==================== FILE I/O ====================
// Định dạng CSV mỗi dòng:
// orderID,carpenterName,day,month,year,estimatedDays,furnitureID,customerUsername,phoneNumber,statusInt,laborCost

bool OrderManager::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot open " << filename << " for writing.\n";
        return false;
    }

    for (const auto& o : orders) {
        Date d = o.getStartDate();
        out << o.getOrderID() << ","
            << o.getCarpenterName() << ","
            << d.day << "," << d.month << "," << d.year << ","
            << o.getEstimatedDays() << ","
            << o.getFurnitureID() << ","
            << o.getCustomerUsername() << ","
            << o.getPhoneNumber() << ","
            << static_cast<int>(o.getStatus()) << ","
            << o.getLaborCost() << "\n";
    }

    out.close();
    return true;
}

bool OrderManager::loadFromFile(const std::string& filename, const FurnitureManager& fManager) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Warning: Cannot open " << filename << " for reading. Starting with no orders.\n";
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string orderId, carpenter, dayStr, monthStr, yearStr, daysStr, fId, username, phone, statusStr, costStr;

        std::getline(ss, orderId, ',');
        std::getline(ss, carpenter, ',');
        std::getline(ss, dayStr, ',');
        std::getline(ss, monthStr, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, daysStr, ',');
        std::getline(ss, fId, ',');
        std::getline(ss, username, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, statusStr, ',');
        std::getline(ss, costStr);

        if (orderId.empty() || fId.empty()) continue;

        try {
            Date d;
            d.day = std::stoi(dayStr);
            d.month = std::stoi(monthStr);
            d.year = std::stoi(yearStr);
            int days = std::stoi(daysStr);
            int statusInt = std::stoi(statusStr);
            double cost = std::stod(costStr);

            auto furniture = fManager.searchById(fId);
            if (!furniture) {
                std::cerr << "Warning: Furniture ID " << fId << " not found for order " << orderId << ". Skipped.\n";
                continue;
            }

            Order restored(orderId, carpenter, d, days, fId, furniture, username, phone);
            restored.setStatus(static_cast<OrderStatus>(statusInt));
            restored.setLaborCost(cost);
            orders.push_back(restored);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing order line: " << line << "\n";
        }
    }

    in.close();
    return true;
}

//khách hàng xóa đơn hàng của chính mình.
bool OrderManager::deleteOrder(const std::string& orderId) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getOrderID() == orderId) {
            orders.erase(it);
            return true;
        }
    }
    return false;
}

bool OrderManager::cancelOwnOrder(const std::string& orderId, const std::string& username) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getOrderID() == orderId && it->getCustomerUsername() == username) {
            if (it->getStatus() == OrderStatus::PENDING) {
                orders.erase(it);
                return true;
            } else {
                std::cout << "Cannot cancel: The order is already IN_PROGRESS or COMPLETED.\n";
                return false;
            }
        }
    }
    std::cout << "Can't find your order.\n";
    return false;
}

bool OrderManager::updateOrder(const std::string& orderId, const std::string& carpenter, const Date& date, int days, OrderStatus status) {
    for (auto& o : orders) {
        if (o.getOrderID() == orderId) {
            o.setCarpenterName(carpenter);
            o.setStartDate(date);
            o.setEstimatedDays(days);
            o.setStatus(status);
            o.setLaborCost(calculatePrice(o));
            return true;
        }
    }
    return false;
}