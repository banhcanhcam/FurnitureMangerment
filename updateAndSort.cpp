#include "ProductValidation.h"
#include <algorithm>

// Xử lý edge case: Sắp xếp an toàn danh sách con trỏ
void updateAndSort(FurnitureList& store) {
    if (store.empty()) return;

    // Sắp xếp tăng dần theo ID sử dụng Lambda Expression
    std::sort(store.begin(), store.end(), 
        [](const std::shared_ptr<FurnitureBase>& a, const std::shared_ptr<FurnitureBase>& b) {
            return a->getFurnitureID() < b->getFurnitureID();
        });
        
    std::cout << "The system automatically rearranged the list.\n";
}