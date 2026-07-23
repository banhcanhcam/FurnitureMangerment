#include "OrderManager.h"
#include "ProductValidation.h"
#include "order.h"
#include "Validation.h"
#include "Invoice.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

// Helper function to get current date
Date getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    Date d;
    d.day = local->tm_mday;
    d.month = local->tm_mon + 1;
    d.year = local->tm_year + 1900;
    return d;
}
void OrderManager::createOrder(const std::string& orderId, const std::string& fId, 
                 const std::string& carpenter, int days, 
                 const FurnitureManager& fManager, 
                 const std::string& username, const std::string& phoneNumber,
                 int quantity) {

    // ===== KIỂM TRA TRÙNG ORDER ID =====
    for (const auto& o : orders) {
        if (o.getOrderID() == orderId) {
            std::cout << "Error: Order ID " << orderId << " already exists. Please use a different ID.\n";
            return;
        }
    }

    // ===== KIỂM TRA KHO =====
    if (isStoreEmpty(fManager.getInventory())) {
        return;
    }

    // ===== KIỂM TRA FURNITURE =====
    std::shared_ptr<FurnitureBase> linkedFurniture = fManager.searchById(fId);
    if (linkedFurniture == nullptr) {
        std::cout << "Error: Furniture ID " << fId << " not found.\n";
        return;
    }

    // ===== KIỂM TRA SỐ LƯỢNG TỒN KHO =====
    if (quantity > linkedFurniture->getQuantity()) {
        std::cout << "Error: Insufficient stock. Available: " 
                  << linkedFurniture->getQuantity() 
                  << ", Requested: " << quantity << "\n";
        return;
    }
    
    // ===== TẠO ĐƠN HÀNG VỚI NGÀY HIỆN TẠI =====
    Date today = getCurrentDate();
    Order newOrder(orderId, carpenter, today, days, fId, linkedFurniture, username, phoneNumber, quantity);
    
    newOrder.setLaborCost(calculatePrice(newOrder));
    orders.push_back(newOrder);
    std::cout << "Order " << orderId << " created successfully.\n";

    // ===== CẬP NHẬT TỒN KHO =====
    linkedFurniture->setQuantity(linkedFurniture->getQuantity() - quantity);

    // ===== HỎI TẠO INVOICE =====
    std::cout << "Do you want to create an invoice for this order? (y/n): ";
    std::string answer = readLine("");
    if (answer == "y" || answer == "Y") {
    std::string invId;
    bool valid = false;
    while (!valid) {
        invId = readLine("Enter Invoice ID: ");
        // Kiểm tra trùng Invoice ID
        bool duplicate = false;
        for (const auto& inv : invoices) {
            if (inv.getInvoiceID() == invId) {
                duplicate = true;
                std::cout << "Error: Invoice ID " << invId << " already exists. Please use a different ID.\n";
                break;
            }
        }
        if (!duplicate) {
            valid = true;
        }
    }
    Invoice newInvoice(invId, orderId, newOrder.getLaborCost(), today, false);
    addInvoice(newInvoice);
    std::cout << "Invoice " << invId << " created.\n";
}
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
    
    return ((area * matRate) + (laborRate * o.getEstimatedDays())) * o.getQuantity();
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
            << o.getLaborCost() << ","   // Đổi từ dấu ';' thành dấu ','
            << o.getQuantity() << "\n";
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
        std::string orderId, carpenter, dayStr, monthStr, yearStr, daysStr, fId, username, phone, statusStr, costStr, qtyStr;

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
        std::getline(ss, costStr, ',');
        std::getline(ss, qtyStr);

        if (orderId.empty() || fId.empty()) continue;

        try {
            Date d;
            d.day = std::stoi(dayStr);
            d.month = std::stoi(monthStr);
            d.year = std::stoi(yearStr);
            int days = std::stoi(daysStr);
            int statusInt = std::stoi(statusStr);
            double cost = std::stod(costStr);

    // ----- Handle quantity (backward compatible) -----
            int qty = 1; // default
            if (!qtyStr.empty()) {
                try {
                    qty = std::stoi(qtyStr);
                } catch (...) {
                    qty = 1; // if parsing fails, keep default
                }
            }

    auto furniture = fManager.searchById(fId);
    if (!furniture) {
        std::cerr << "Warning: Furniture ID " << fId << " not found for order " << orderId << ". Skipped.\n";
        continue;
    }

    Order restored(orderId, carpenter, d, days, fId, furniture, username, phone, qty);
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
void OrderManager::addInvoice(const Invoice& inv) { invoices.push_back(inv); }
const std::vector<Invoice>& OrderManager::getInvoices() const { return invoices; }

bool OrderManager::saveInvoicesToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot open " << filename << " for writing.\n";
        return false;
    }
    for (const auto& inv : invoices) {
        Date d = inv.getCreatedDate();
        out << inv.getInvoiceID() << ","
            << inv.getOrderID() << ","
            << inv.getTotalAmount() << ","
            << d.day << "," << d.month << "," << d.year << ","
            << inv.getIsPaid() << "\n";
    }
    out.close();
    return true;
}

bool OrderManager::loadInvoicesFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Warning: Cannot open " << filename << " for reading.\n";
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string invId, ordId, amountStr, dayStr, monthStr, yearStr, paidStr;
        std::getline(ss, invId, ',');
        std::getline(ss, ordId, ',');
        std::getline(ss, amountStr, ',');
        std::getline(ss, dayStr, ',');
        std::getline(ss, monthStr, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, paidStr);
        try {
            double amount = std::stod(amountStr);
            int day = std::stoi(dayStr);
            int month = std::stoi(monthStr);
            int year = std::stoi(yearStr);
            bool paid = (paidStr == "1");
            Date date{day, month, year};
            Invoice inv(invId, ordId, amount, date, paid);
            invoices.push_back(inv);
        } catch (...) {
            std::cerr << "Error parsing invoice line: " << line << "\n";
        }
    }
    in.close();
    return true;
}

// ---- NEW METHODS (add them here) ----
Invoice* OrderManager::findInvoiceByOrderId(const std::string& orderId) {
    for (auto& inv : invoices) {
        if (inv.getOrderID() == orderId) {
            return &inv;
        }
    }
    return nullptr;
}

std::vector<Invoice> OrderManager::getInvoicesByCustomer(const std::string& username) const {
    std::vector<Invoice> result;
    for (const auto& inv : invoices) {
        for (const auto& o : orders) {
            if (o.getOrderID() == inv.getOrderID() && o.getCustomerUsername() == username) {
                result.push_back(inv);
                break;
            }
        }
    }
    return result;
}
