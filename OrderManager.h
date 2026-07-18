#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include <string>
#include "Order.h"
#include "Date.h"
#include "FurnitureManager.h"

class OrderManager {
private:
    std::vector<Order> orders;

public:
	bool cancelOwnOrder(const std::string& orderId, const std::string& username);
    OrderManager() = default;

    void createOrder(const std::string& orderId, const std::string& fId, 
                 const std::string& carpenter, const Date& date, 
                 int days, const FurnitureManager& fManager, const std::string& username,
                 const std::string& phoneNumber);
    double calculatePrice(const Order& o) const;
    void trackByStatus(OrderStatus status) const;
    
    bool deleteOrder(const std::string& orderId); 
    bool updateOrder(const std::string& orderId, const std::string& carpenter, const Date& date, int days, OrderStatus status);

    const std::vector<Order>& getOrders() const;

    // Logic File I/O
    bool loadFromFile(const std::string& filename, const FurnitureManager& fManager);
    bool saveToFile(const std::string& filename) const;
};

#endif // ORDER_MANAGER_H