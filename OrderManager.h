#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include <string>
#include "Order.h"
#include "FurnitureManager.h"

class OrderManager {
private:
    std::vector<Order> orders;

public:
	bool cancelOwnOrder(const std::string& orderId, const std::string& username);
    OrderManager() = default;

    // Thêm tham số const std::string& username vào khai báo
void createOrder(const std::string& orderId, const std::string& fId, 
                 const std::string& carpenter, const std::string& date, 
                 int days, const FurnitureManager& fManager, const std::string& username);
    double calculatePrice(const Order& o) const;
    void trackByStatus(OrderStatus status) const;
    
    // --- THÊM DÒNG NÀY VÀO TRONG CLASS ---
    bool deleteOrder(const std::string& orderId); 
    // --------------------------------------

    const std::vector<Order>& getOrders() const;

    // Logic File I/O
    bool loadFromFile(const std::string& filename, const FurnitureManager& fManager);
    bool saveToFile(const std::string& filename) const;
};

#endif // ORDER_MANAGER_H