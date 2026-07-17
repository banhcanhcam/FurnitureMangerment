#include "ProductValidation.h"
#include <limits>

std::string getValidString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::cin >> std::ws; 
    std::getline(std::cin, input);
    return input;
}

// Hàm dùng chung cho Width, Height, Depth
double getValidDimension(const std::string& prompt) {
    double value;
    std::cout << prompt;
    while (!(std::cin >> value) || value <= 0) {
        std::cout << "[Lỗi] Kích thước không hợp lệ. Vui lòng nhập lại (số > 0): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}