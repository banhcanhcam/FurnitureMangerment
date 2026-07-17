#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <memory>
#include "FurnitureBase.h"

enum class OrderStatus { PENDING, IN_PROGRESS, COMPLETED };

class Order {
private:
    std::string orderID;
    std::string carpenterName;
    std::string startDate;
    int estimatedDays;
    OrderStatus status;
    std::string furnitureID;
    double laborCost;
    
    // Đã xóa dòng trùng lặp ở đây
    std::string customerUsername; 
    std::shared_ptr<FurnitureBase> furnitureRef;

public:
	Order(std::string oId, std::string cName, std::string sDate, int eDays, 
      std::string fId, std::shared_ptr<FurnitureBase> fRef, std::string uName);

    // Getters
    std::string getOrderID() const;
    std::string getCarpenterName() const;
    std::string getStartDate() const;
    int getEstimatedDays() const;
    OrderStatus getStatus() const;
    std::string getFurnitureID() const;
    double getLaborCost() const;
    std::shared_ptr<FurnitureBase> getFurniture() const;
    std::string getCustomerUsername() const; // Getter cho tên khách

    // Setters
    void setStatus(OrderStatus s);
    void setLaborCost(double cost);
    void setCustomerUsername(const std::string& name); // Đã sửa: dùng void và tham số
};

#endif // ORDER_H