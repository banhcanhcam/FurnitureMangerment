#ifndef DATE_H
#define DATE_H

#include <string>

struct Date {
    int day = 1;
    int month = 1;
    int year = 2000;

    // ---- Các hàm hiện có ----
    std::string toString() const; // Định dạng DD/MM/YYYY

    // ---- HÀM MỚI: Chuyển đổi ----
    int toDays() const;           // Đếm số ngày kể từ 1900-01-01
    static Date fromDays(int days); // Tạo Date từ số ngày (ngược lại)

    // ---- HÀM MỚI: Cộng / trừ ----
    Date addDays(int days) const;   // Trả về Date mới sau khi cộng thêm days
    int daysBetween(const Date& other) const; // Chênh lệch ngày giữa hai Date

    // ---- TOÁN TỬ SO SÁNH ----
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
};

bool isValidDate(int day, int month, int year); // Đã có

#endif