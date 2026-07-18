#include "Date.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

// ---- Hàm kiểm tra ngày hợp lệ (giữ nguyên) ----
bool isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && isLeap) maxDay = 29;
    return day <= maxDay;
}

// ---- toString (giữ nguyên) ----
std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << "/"
        << std::setw(2) << std::setfill('0') << month << "/"
        << year;
    return oss.str();
}

// ---- Chuyển đổi Date -> số ngày (mốc 1900-01-01) ----
int Date::toDays() const {
    int days = 0;
    // Cộng dồn số ngày của các năm từ 1900 đến year-1
    for (int y = 1900; y < year; ++y) {
        bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        days += leap ? 366 : 365;
    }
    // Cộng dồn số ngày của các tháng trong năm hiện tại
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int m = 1; m < month; ++m) {
        days += daysInMonth[m - 1];
        if (m == 2) {
            bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (leap) days += 1;
        }
    }
    // Cộng ngày trong tháng hiện tại (trừ 1 vì ngày bắt đầu từ 1)
    days += day - 1;
    return days;
}

// ---- Tạo Date từ số ngày (ngược lại) ----
Date Date::fromDays(int days) {
    Date d;
    d.year = 1900;
    while (true) {
        bool leap = (d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0);
        int daysInYear = leap ? 366 : 365;
        if (days < daysInYear) break;
        days -= daysInYear;
        d.year++;
    }
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (d.month = 1; d.month <= 12; ++d.month) {
        int maxDay = daysInMonth[d.month - 1];
        if (d.month == 2) {
            bool leap = (d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0);
            if (leap) maxDay = 29;
        }
        if (days < maxDay) break;
        days -= maxDay;
    }
    d.day = days + 1;
    return d;
}

// ---- Cộng thêm số ngày ----
Date Date::addDays(int days) const {
    int total = toDays() + days;
    return fromDays(total);
}

// ---- Chênh lệch ngày (other - this) ----
int Date::daysBetween(const Date& other) const {
    return other.toDays() - toDays();
}

// ---- Các toán tử so sánh ----
bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}
bool Date::operator>(const Date& other) const { return other < *this; }
bool Date::operator<=(const Date& other) const { return !(*this > other); }
bool Date::operator>=(const Date& other) const { return !(*this < other); }
bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}
bool Date::operator!=(const Date& other) const { return !(*this == other); }