#include "Order.h"
#include "Validation.h"
void Order::setCustomerUsername(const std::string& name) {
    customerUsername = name;
}
Order::Order(std::string oId, std::string cName, std::string sDate, int eDays, 
             std::string fId, std::shared_ptr<FurnitureBase> fRef, std::string uName)
    : orderID(oId), carpenterName(cName), startDate(sDate), estimatedDays(eDays),
      status(OrderStatus::PENDING), furnitureID(fId), laborCost(0.0), 
      furnitureRef(fRef), customerUsername(uName) {
    
    if (cName.empty()) {
        throw MissingCarpenterException();
    }
}

std::string Order::getOrderID() const { return orderID; }
std::string Order::getCarpenterName() const { return carpenterName; }
std::string Order::getStartDate() const { return startDate; }
int Order::getEstimatedDays() const { return estimatedDays; }
OrderStatus Order::getStatus() const { return status; }
std::string Order::getFurnitureID() const { return furnitureID; }
double Order::getLaborCost() const { return laborCost; }
std::shared_ptr<FurnitureBase> Order::getFurniture() const { return furnitureRef; }

void Order::setStatus(OrderStatus s) { status = s; }
void Order::setLaborCost(double cost) { 
    if (cost < 0) throw std::exception(); // Khớp với validateCost() trong báo cáo
    laborCost = cost; 
}
// Đảm bảo đoạn này nằm ở cuối file Order.cpp
std::string Order::getCustomerUsername() const {
    return customerUsername;
}