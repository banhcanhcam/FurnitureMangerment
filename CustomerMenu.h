#ifndef CUSTOMER_MENU_H
#define CUSTOMER_MENU_H

#include "FurnitureManager.h"
#include "OrderManager.h"
#include "AccountManager.h"
#include <string>

void runCustomerMenu(FurnitureManager& fManager, OrderManager& oManager, AccountManager& aManager, std::string username);

#endif