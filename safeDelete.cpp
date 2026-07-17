#include "ProductValidation.h"

// Xử lý edge case: Delete làm lệch chỉ số
bool safeDelete(FurnitureList& store, int index) {
    if (index < 0 || index >= store.size()) {
        std::cout << "[Lỗi] Chỉ số xóa không tồn tại hoặc đã bị lệch!\n";
        return false;
    }
    
    store.erase(store.begin() + index);
    std::cout << "Xóa sản phẩm thành công!\n";
    return true;
}