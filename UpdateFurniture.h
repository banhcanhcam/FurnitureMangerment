#ifndef UPDATE_FURNITURE_H
#define UPDATE_FURNITURE_H

#include "FurnitureManager.h"
#include "AccountManager.h"
#include "OrderManager.h"

namespace UpdateFurniture {
    void updateFurnitureFromInput(FurnitureManager& fManager, 
                                  AccountManager& aManager, 
                                  OrderManager& oManager);
}

#endif