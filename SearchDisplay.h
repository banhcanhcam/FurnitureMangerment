#ifndef SEARCH_DISPLAY_H
#define SEARCH_DISPLAY_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cctype>

#include "FurnitureBase.h"
#include "Order.h"
#include "FurnitureManager.h"
#include "OrderManager.h"

namespace SearchDisplay {

    // ---- TIỆN ÍCH (Utilities) ----
    std::string trim(const std::string& s);
    std::string toLower(const std::string& s);

    // ---- HIỂN THỊ (Display) ----
    void printFurnitureHeader();
    void printFurniture(const std::shared_ptr<FurnitureBase>& item);
    
    // Yêu cầu FurnitureManager phải có hàm getInventory()
    void displayAllFurniture(const FurnitureManager& fm); 
    void displayOrdersByStatus(const OrderManager& om, OrderStatus status);

    // ---- TÌM KIẾM (Search) ----
    std::shared_ptr<FurnitureBase> searchFurnitureById(const FurnitureManager& fm, const std::string& id);
    
    // Tìm gần đúng theo màu sắc (Substring keyword)
    std::vector<std::shared_ptr<FurnitureBase>> searchFurnitureByColor(const FurnitureManager& fm, const std::string& keyword);

    // ---- SẮP XẾP (Sorting) ----
    // Sắp xếp đơn hàng theo tổng chi phí tăng dần (stable_sort)
    void sortOrdersByCostAscending(std::vector<Order>& orders);
    std::vector<std::shared_ptr<FurnitureBase>> searchFurnitureByName(const FurnitureManager& fm, const std::string& name);

} // namespace SearchDisplay

#endif // SEARCH_DISPLAY_H