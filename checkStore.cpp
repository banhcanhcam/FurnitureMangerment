#include "ProductValidation.h"

// Xử lý edge case: Kiểm tra toàn bộ kho có rỗng không
bool isStoreEmpty(const FurnitureInventory& store) {
    for (const auto& pair : store) {
        if (!pair.second.empty()) {
            return false; // Chỉ cần 1 danh sách vật liệu có đồ là kho không rỗng
        }
    }
    std::cout << "[Lỗi] Kho hàng hiện đang trống! Không thể thực hiện thao tác này.\n";
    return true;
}

// Xử lý edge case: Trùng ID (Thay vì trùng tên)
bool isDuplicateId(const FurnitureInventory& store, const std::string& id) {
    for (const auto& pair : store) {
        for (const auto& item : pair.second) {
            // Sử dụng con trỏ thông minh item->getFurnitureID()
            if (item->getFurnitureID() == id) { 
                return true;
            }
        }
    }
    return false;
}