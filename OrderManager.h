#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include <string>
#include "Order.h"
#include "Invoice.h"
#include "Date.h"
#include "FurnitureManager.h"

class OrderManager {
private:
    std::vector<Order> orders;
    std::vector<Invoice> invoices;

public:
	bool cancelOwnOrder(const std::string& orderId, const std::string& username);
    OrderManager() = default;

    void createOrder(const std::string& orderId, const std::string& fId, 
                 const std::string& carpenter, int days, 
                 const FurnitureManager& fManager, 
                 const std::string& username, const std::string& phoneNumber,
                 int quantity);
    double calculatePrice(const Order& o) const;
    void trackByStatus(OrderStatus status) const;
    
    bool deleteOrder(const std::string& orderId); 
    bool updateOrder(const std::string& orderId, const std::string& carpenter, const Date& date, int days, OrderStatus status);

    const std::vector<Order>& getOrders() const;

    // Logic File I/O
    bool loadFromFile(const std::string& filename, const FurnitureManager& fManager);
    bool saveToFile(const std::string& filename) const;
    //Load invoice
    void addInvoice(const Invoice& inv);
    const std::vector<Invoice>& getInvoices() const;
    bool saveInvoicesToFile(const std::string& filename) const;
    bool loadInvoicesFromFile(const std::string& filename);
    Invoice* findInvoiceByOrderId(const std::string& orderId);
    std::vector<Invoice> getInvoicesByCustomer(const std::string& username) const;
    const std::vector<Invoice>& getAllInvoices() const { return invoices; }
};

#endif // ORDER_MANAGER_H