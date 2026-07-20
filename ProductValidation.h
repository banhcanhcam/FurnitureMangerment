#ifndef PRODUCT_VALIDATION_H
#define PRODUCT_VALIDATION_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "FurnitureBase.h"
#include "Validation.h"

// Định nghĩa Alias để tái sử dụng code dễ dàng hơn, khớp với kiến trúc FurnitureManager
using FurnitureList = std::vector<std::shared_ptr<FurnitureBase>>;
using FurnitureInventory = std::map<MaterialType, FurnitureList>;

// Nhóm kiểm tra trạng thái (Truyền toàn bộ map inventory vào)
bool isStoreEmpty(const FurnitureInventory& store);
bool isDuplicateId(const FurnitureInventory& store, const std::string& id);
// excludeId: bỏ qua item có ID này khi kiểm tra (dùng khi Update, để không tự báo trùng với chính nó)
bool isDuplicateName(const FurnitureInventory& store, const std::string& name, const std::string& excludeId = "");

// Đọc tên hợp lệ cho Furniture: không rỗng, không số, không khoảng trắng, không trùng tên đã có.
// Kiểm tra và báo lỗi NGAY sau khi nhập, trước khi hỏi các field khác.
std::string readUniqueFurnitureName(const std::string& prompt, const FurnitureInventory& store, const std::string& excludeId = "");

// Nhóm xử lý nhập liệu an toàn
std::string getValidString(const std::string& prompt);
double getValidDimension(const std::string& prompt); // Thay thế Price/Quantity bằng Dimension

// Nhóm thao tác dữ liệu an toàn
bool safeDelete(FurnitureList& store, int index);
void updateAndSort(FurnitureList& store);
bool safeWriteToFile(const std::string& filename, const std::string& data);

#endif