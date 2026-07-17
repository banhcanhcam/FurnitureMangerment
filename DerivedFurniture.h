#ifndef DERIVED_FURNITURE_H
#define DERIVED_FURNITURE_H

#include "FurnitureBase.h"

class WoodFurniture : public FurnitureBase {
private:
 double width, height, depth;
 std::string baseColor;
public:
  WoodFurniture(std::string id, double w, double h, double d, std::string color, std::string name, double price, int qty)
    : FurnitureBase(id, name, MaterialType::WOOD, w, h, d, price, qty, color) {}
    
    double getMaterialRate() const override { return 150.0; } 
};

class MetalFurniture : public FurnitureBase {
public:
    MetalFurniture(std::string id, std::string name, double w, double h, double d, double p, int q, std::string color)
        : FurnitureBase(id, name, MaterialType::METAL, w, h, d, p, q, color) {}
    
    
    double getMaterialRate() const override { return 250.0; } 
};

class AluminumFurniture : public FurnitureBase {
public:
    AluminumFurniture(std::string id, std::string name, double w, double h, double d, double p, int q, std::string color)
        : FurnitureBase(id, name, MaterialType::ALUMINUM, w, h, d, p, q, color) {} 
    
    double getMaterialRate() const override { return 50.0; }
};

#endif // DERIVED_FURNITURE_H