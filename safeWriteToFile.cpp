#include "ProductValidation.h"
#include <fstream>

// Xử lý edge case: Lỗi ghi file
bool safeWriteToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[Error] Cannot open file '" << filename << "' to write. Check the access rights.\n";
        return false;
    }
    file << data << "\n";
    file.close();
    return true;
}