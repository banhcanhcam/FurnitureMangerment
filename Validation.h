#ifndef VALIDATION_H
#define VALIDATION_H

#include <exception>
#include <string>
#include <iostream>
// Lỗi khi kích thước <= 0
class InvalidDimensionException : public std::exception {
    std::string msg;
public:
    InvalidDimensionException(const std::string& field) {
        msg = "Invalid dimension: " + field + " must be > 0";
    }
    const char* what() const noexcept override { 
        return msg.c_str(); 
    }
};

// Lỗi khi ID sản phẩm bị trùng
class DuplicateIdException : public std::exception {
    std::string msg;
public:
    DuplicateIdException(const std::string& id) {
        msg = "Duplicate ID: Furniture " + id + " already exists.";
    }
    const char* what() const noexcept override { 
        return msg.c_str(); 
    }
};

// Lỗi khi không nhập tên thợ mộc
class MissingCarpenterException : public std::exception {
public:
    const char* what() const noexcept override { 
        return "Carpenter name cannot be empty."; 
    }
};
inline std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);
    return line;
}
inline bool hasDigit(const std::string& str) {
    for (char c : str) {
        if (std::isdigit(c)) return true;
    }
    return false;
}
// Hàm này sẽ đảm nhận toàn bộ việc "hỏi - kiểm tra - ép nhập lại"
inline std::string readValidName(const std::string& prompt) {
    std::string name;
    do {
        name = readLine(prompt);
        
        if (name.empty()) {
            std::cout << "Name cannot be empty! Please try again.\n";
        } else if (hasDigit(name)) {
            std::cout << "Invalid name! Name cannot contain numbers. Try again.\n";
        }
        
    } while (name.empty() || hasDigit(name));
    
    return name;
}

#endif // VALIDATION_H