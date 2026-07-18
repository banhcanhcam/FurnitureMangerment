#include "ProductValidation.h"

// Xử lý edge case: Delete làm lệch chỉ số
bool safeDelete(FurnitureList& store, int index) {
    if (index < 0 || index >= store.size()) {
        std::cout << "[Error] The delete index does not exist or has been misaligned!\n";
        return false;
    }
    
    store.erase(store.begin() + index);
    std::cout << "Product deleted successfully!\n";
    return true;
}