#include "Order.h"
#include "Validation.h"

Order::Order(std::string oId, std::string cName, Date sDate, int eDays, 
             std::string fId, std::shared_ptr<FurnitureBase> fRef, 
             std::string uName, std::string phone, int qty)
    : orderID(oId), carpenterName(cName), startDate(sDate), estimatedDays(eDays),
      status(OrderStatus::PENDING), furnitureID(fId), laborCost(0.0), 
      customerUsername(uName), phoneNumber(phone), furnitureRef(fRef), quantity(qty) {
    if (cName.empty()) throw MissingCarpenterException();
    if (qty <= 0) throw std::invalid_argument("Quantity must be positive");
}

std::string Order::getOrderID() const { return orderID; }
std::string Order::getCarpenterName() const { return carpenterName; }
Date Order::getStartDate() const { return startDate; }
int Order::getEstimatedDays() const { return estimatedDays; }
OrderStatus Order::getStatus() const { return status; }
std::string Order::getFurnitureID() const { return furnitureID; }
double Order::getLaborCost() const { return laborCost; }
std::shared_ptr<FurnitureBase> Order::getFurniture() const { return furnitureRef; }
std::string Order::getCustomerUsername() const { return customerUsername; }
std::string Order::getPhoneNumber() const { return phoneNumber; }
int Order::getQuantity() const { return quantity; }

void Order::setCarpenterName(const std::string& name) { carpenterName = name; }
void Order::setStartDate(const Date& date) { startDate = date; }
void Order::setEstimatedDays(int days) { estimatedDays = days; }
void Order::setStatus(OrderStatus s) { status = s; }
void Order::setLaborCost(double cost) { 
    if (cost < 0) throw std::exception(); // Khớp với validateCost() trong báo cáo
    laborCost = cost; 
}
void Order::setCustomerUsername(const std::string& name) { customerUsername = name; }
void Order::setPhoneNumber(const std::string& phone) { phoneNumber = phone; }
void Order::setQuantity(int qty) { quantity = qty; }