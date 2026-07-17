#include "ProductValidation.h"
#include <fstream>

// Xử lý edge case: Lỗi ghi file
bool safeWriteToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[Lỗi] Không thể mở file '" << filename << "' để ghi. Kiểm tra lại quyền truy cập.\n";
        return false;
    }
    file << data << "\n";
    file.close();
    return true;
}