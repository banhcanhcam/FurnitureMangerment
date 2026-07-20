#ifndef UPDATE_ORDER_H
#define UPDATE_ORDER_H

#include "OrderManager.h"
#include "FurnitureManager.h"
#include "AccountManager.h"

namespace UpdateOrder {
    void updateOrderFromInput(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager);
}

#endif