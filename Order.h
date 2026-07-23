#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <memory>
#include "FurnitureBase.h"
#include "Date.h"

enum class OrderStatus { PENDING, IN_PROGRESS, COMPLETED };

class Order {
private:
    std::string orderID;
    std::string carpenterName;
    Date startDate;
    int estimatedDays;
    OrderStatus status;
    std::string furnitureID;
    double laborCost;
    std::string customerUsername;
    std::string phoneNumber; // Số điện thoại liên hệ cho đơn hàng này
    std::shared_ptr<FurnitureBase> furnitureRef;
    int quantity;

public:
    Order(std::string oId, std::string cName, Date sDate, int eDays, 
          std::string fId, std::shared_ptr<FurnitureBase> fRef, 
          std::string uName, std::string phone, int qty);

    // Getters
    std::string getOrderID() const;
    std::string getCarpenterName() const;
    Date getStartDate() const;
    int getEstimatedDays() const;
    OrderStatus getStatus() const;
    std::string getFurnitureID() const;
    double getLaborCost() const;
    std::shared_ptr<FurnitureBase> getFurniture() const;
    std::string getCustomerUsername() const;
    std::string getPhoneNumber() const;
    int getQuantity() const;

    // Setters
    void setCarpenterName(const std::string& name);
    void setStartDate(const Date& date);
    void setEstimatedDays(int days);
    void setStatus(OrderStatus s);
    void setLaborCost(double cost);
    void setCustomerUsername(const std::string& name);
    void setPhoneNumber(const std::string& phone);
    void setQuantity(int qty);
};

#endif // ORDER_H