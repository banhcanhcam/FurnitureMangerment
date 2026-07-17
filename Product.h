#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    std::string name;
    double price;
    int quantity;

public:
    // Constructor (Hàm khởi tạo)
    Product(std::string n, double p, int q);

    // Getters (Hàm lấy dữ liệu)
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;

    // Setters (Hàm cập nhật dữ liệu)
    void setName(std::string n);
    void setPrice(double p);
    void setQuantity(int q);
};

#endif