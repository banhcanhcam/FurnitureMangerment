#ifndef VALIDATION_H
#define VALIDATION_H

#include <exception>
#include <limits>
#include <string>
#include <cctype>
#include <iostream>
#include <sstream>
#include "Date.h"
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
// Đọc một số nguyên không âm (>=0)
inline int readNumber(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value < 0) {
                std::cout << "Error: Value must be >= 0.\n";
                continue;
            }
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a valid integer.\n";
    }
}

// Đọc số nguyên dương (>0)
inline int readDay(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value <= 0) {
                std::cout << "Error: Value must be > 0.\n";
                continue;
            }
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a valid integer.\n";
    }
}

// Đọc số nguyên với giới hạn (có thể tùy chỉnh, ở đây đọc bất kỳ int)
inline int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a valid integer.\n";
    }
}

// Đọc số thực dương (>0)
inline double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value <= 0) {
                std::cout << "Error: Value must be > 0.\n";
                continue;
            }
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a valid decimal number.\n";
    }
}

// Đọc chuỗi chỉ chứa chữ cái và khoảng trắng (không số)
inline std::string readLineAlpha(const std::string& prompt) {
    std::string line;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, line);
        bool allLetters = true;
        for (char c : line) {
            if (!std::isalpha(c) && !std::isspace(c)) {
                allLetters = false;
                break;
            }
        }
        if (allLetters && !line.empty()) {
            return line;
        }
        std::cout << "Invalid input, please enter letters only (no numbers).\n";
    }
}

// Đọc số điện thoại hợp lệ (chỉ chữ số, độ dài 9-11 ký tự, ví dụ số VN)
inline std::string readPhoneNumber(const std::string& prompt) {
    std::string phone;
    while (true) {
        phone = readLine(prompt);
        bool allDigits = !phone.empty();
        for (char c : phone) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                allDigits = false;
                break;
            }
        }
        if (allDigits && phone.size() >= 9 && phone.size() <= 11) {
            return phone;
        }
        std::cout << "Invalid phone number! Must contain only digits (9-11 numbers). Try again.\n";
    }
}

// Parse "DD/MM/YYYY" từ một dòng nhập. Trả về true nếu đúng định dạng VÀ là ngày hợp lệ.
inline bool tryParseDate(const std::string& line, Date& outDate) {
    std::istringstream iss(line);
    int dd, mm, yyyy;
    char sep1, sep2;
    if (!(iss >> dd >> sep1 >> mm >> sep2 >> yyyy)) return false;
    if (sep1 != '/' || sep2 != '/') return false;
    if (!isValidDate(dd, mm, yyyy)) return false;
    outDate.day = dd;
    outDate.month = mm;
    outDate.year = yyyy;
    return true;
}

// Đọc một ngày hợp lệ, bắt buộc phải nhập (không cho để trống)
inline Date readDate(const std::string& prompt) {
    Date d;
    while (true) {
        std::string line = readLine(prompt + " (DD/MM/YYYY): ");
        if (tryParseDate(line, d)) return d;
        std::cout << "Invalid date! Please enter a real date in DD/MM/YYYY format.\n";
    }
}

// Đọc một ngày, cho phép nhấn Enter để giữ nguyên giá trị hiện tại
inline Date readDateOrKeep(const std::string& prompt, const Date& current) {
    Date d;
    while (true) {
        std::string line = readLine(prompt + " [" + current.toString() + "] (DD/MM/YYYY, Enter to keep): ");
        if (line.empty()) return current;
        if (tryParseDate(line, d)) return d;
        std::cout << "Invalid date! Please enter a real date in DD/MM/YYYY format, or press Enter to keep.\n";
    }
}



#endif // VALIDATION_H