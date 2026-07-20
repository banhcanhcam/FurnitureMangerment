#include "ProductValidation.h"
#include <algorithm>

// Xử lý edge case: Sắp xếp an toàn danh sách con trỏ, theo GIÁ tăng dần
void updateAndSort(FurnitureList& store) {
    if (store.empty()) return;

    // Sắp xếp tăng dần theo Price sử dụng Lambda Expression
    std::sort(store.begin(), store.end(), 
        [](const std::shared_ptr<FurnitureBase>& a, const std::shared_ptr<FurnitureBase>& b) {
            return a->getPrice() < b->getPrice();
        });
        
    std::cout << "The system automatically sorted the list by price (ascending).\n";
}