#include "OrderManager.h"
#include <iostream>

void OrderManager::createOrder(const std::string& orderId, const std::string& fId, 
                               const std::string& carpenter, const std::string& date, 
                               int days, const FurnitureManager& fManager, const std::string& username) { // Thêm tham số này

    std::shared_ptr<FurnitureBase> linkedFurniture = fManager.searchById(fId);

    if (linkedFurniture == nullptr) {
        std::cout << "Error: Furniture ID " << fId << " not found.\n";
        return;
    }

    // Truyền thêm username vào tham số cuối cùng (tham số thứ 7)
    Order newOrder(orderId, carpenter, date, days, fId, linkedFurniture, username); 
    
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

// Logic File I/O
bool OrderManager::loadFromFile(const std::string& filename, const FurnitureManager& fManager) { return true; }
bool OrderManager::saveToFile(const std::string& filename) const { return true; }
// xóa đơn hàng bất kỳ.
// Admin xóa bất kỳ đơn hàng nào
bool OrderManager::deleteOrder(const std::string& orderId) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getOrderID() == orderId) {
            orders.erase(it);
            return true;
        }
    }
    return false;
}
//khách hàng xóa đơn hàng của chính mình.
bool OrderManager::cancelOwnOrder(const std::string& orderId, const std::string& username) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        // Kiểm tra ID đơn hàng VÀ kiểm tra xem đơn hàng có thuộc về user này không
        if (it->getOrderID() == orderId && it->getCustomerUsername() == username) {
            if (it->getStatus() == OrderStatus::PENDING) { // Chỉ hủy nếu đơn chưa bắt đầu
                orders.erase(it);
                return true;
            } else {
                std::cout << "Không thể hủy: Đơn hàng đã ở trạng thái IN_PROGRESS hoặc COMPLETED.\n";
                return false;
            }
        }
    }
    std::cout << "Không tìm thấy đơn hàng của bạn.\n";
    return false;
}
