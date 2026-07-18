#ifndef CUSTOMER_MENU_H
#define CUSTOMER_MENU_H

#include "FurnitureManager.h"
#include "OrderManager.h"
#include <string>

void runCustomerMenu(FurnitureManager& fManager, OrderManager& oManager, const std::string& username);

#endif