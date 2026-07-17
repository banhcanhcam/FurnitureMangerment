#include "Product.h"

Product::Product(std::string n, double p, int q)
    : name(n), price(p), quantity(q) {
    	if( price < 0 ){
    		price = 0;
		}
	
		if(quantity < 0 ){
			quantity = 0;
		}
	}
    
std::string Product::getName() const { return name;}
double Product::getPrice() const { return price;}
int Product::getQuantity() const { return quantity;}

void Product::setName(std::string n) { name = n; }
void Product::setPrice(double p) { price = p; }
void Product::setQuantity(int q) { quantity = q; }