#ifndef FURNITURE_BASE_H
#define FURNITURE_BASE_H

#include <string>

enum class MaterialType { WOOD, METAL, ALUMINUM };

class FurnitureBase {
protected:
	std::string name;
	int quantity;
	double price;
    std::string furnitureID;
    MaterialType materialType;
    double width;
    double height;
    double depth;
    std::string baseColor;

public:
    FurnitureBase(std::string id, std::string name, MaterialType type, double w, double h, double d, double p, int q, std::string color);
    virtual ~FurnitureBase() = default;

    // Hàm thuần ảo (Pure virtual function) yêu cầu các class con phải ghi đè
    virtual double getMaterialRate() const = 0;
    virtual void display() const;

    // Getters
    std::string getFurnitureID() const;
    MaterialType getMaterialType() const;
    double getWidth() const;
    double getHeight() const;
    double getDepth() const;
    std::string getBaseColor() const;
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void setDimensions(double w, double h, double d);
    void setBaseColor(const std::string& color);
};

#endif // FURNITURE_BASE_H