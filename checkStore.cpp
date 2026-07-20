#include "ProductValidation.h"

// Xử lý edge case: Kiểm tra toàn bộ kho có rỗng không
bool isStoreEmpty(const FurnitureInventory& store) {
    for (const auto& pair : store) {
        if (!pair.second.empty()) {
            return false; // Chỉ cần 1 danh sách vật liệu có đồ là kho không rỗng
        }
    }
    std::cout << "[Error] The warehouse is currently empty! Cannot perform this action.\n";
    return true;
}

// Xử lý edge case: Trùng ID
bool isDuplicateId(const FurnitureInventory& store, const std::string& id) {
    for (const auto& pair : store) {
        for (const auto& item : pair.second) {
            if (item->getFurnitureID() == id) {
                return true;
            }
        }
    }
    return false;
}

// Xử lý edge case: Trùng Tên sản phẩm (yêu cầu cảnh báo ngay khi tạo trùng tên)
// excludeId: bỏ qua chính item đang được Update để không tự báo trùng với bản thân nó
bool isDuplicateName(const FurnitureInventory& store, const std::string& name, const std::string& excludeId) {
    for (const auto& pair : store) {
        for (const auto& item : pair.second) {
            if (!excludeId.empty() && item->getFurnitureID() == excludeId) {
                continue;
            }
            if (item->getName() == name) {
                return true;
            }
        }
    }
    return false;
}

// Đọc tên Furniture hợp lệ: không rỗng, không số, không khoảng trắng, không trùng tên đã có.
// Báo lỗi và bắt nhập lại NGAY, không đợi tới lúc lưu mới báo.
std::string readUniqueFurnitureName(const std::string& prompt, const FurnitureInventory& store, const std::string& excludeId) {
    std::string name;
    while (true) {
        name = readLine(prompt);

        if (name.empty()) {
            std::cout << "Name cannot be empty! Please try again.\n";
        } else if (hasDigit(name)) {
            std::cout << "Invalid name! Name cannot contain numbers. Try again.\n";
        } else if (hasWhitespace(name)) {
            std::cout << "Invalid name! Name cannot contain spaces. Try again.\n";
        } else if (isDuplicateName(store, name, excludeId)) {
            std::cout << "[Error] A furniture item named '" << name << "' already exists. Please enter a different name.\n";
        } else {
            return name;
        }
    }
}