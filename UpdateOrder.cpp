#include "UpdateOrder.h"
#include "Validation.h"
#include <iostream>

namespace UpdateOrder {
    void updateOrderFromInput(OrderManager& oManager) {
        std::string oid = readLine("Enter Order ID to update: ");
        bool found = false;
        for (auto& o : oManager.getOrders()) {
            if (o.getOrderID() == oid) {
                found = true;
                std::cout << "\nCurrent Order Information:\n";
                std::cout << "Order ID: " << o.getOrderID() << "\n";
                std::cout << "Carpenter: " << o.getCarpenterName() << "\n";
                std::cout << "Start Date: " << o.getStartDate().toString() << "\n";
                std::cout << "Estimated Days: " << o.getEstimatedDays() << "\n";
                std::cout << "Status: ";
                switch (o.getStatus()) {
                    case OrderStatus::PENDING: std::cout << "PENDING\n"; break;
                    case OrderStatus::IN_PROGRESS: std::cout << "IN_PROGRESS\n"; break;
                    case OrderStatus::COMPLETED: std::cout << "COMPLETED\n"; break;
                }
                std::cout << "Labor Cost: " << o.getLaborCost() << "\n";

                std::cout << "\nWhich field do you want to update?\n";
                std::cout << "1. Carpenter\n2. Start Date\n3. Estimated Days\n4. Status\n0. Cancel\n";
                int choice = readNumber("Your choice: ");

                std::string carpenter = o.getCarpenterName();
                Date date = o.getStartDate();
                int days = o.getEstimatedDays();
                OrderStatus status = o.getStatus();

                switch (choice) {
                    case 1: carpenter = readLineAlpha("New carpenter: "); break;
                    case 2: date = readDate("New date"); break;
                    case 3: days = readDay("New estimated days: "); break;
                    case 4: {
                        int s = readInt("New status (0:PENDING,1:IN_PROGRESS,2:COMPLETED): ");
                        if (s >= 0 && s <= 2) status = static_cast<OrderStatus>(s);
                        else std::cout << "Invalid status, keeping old.\n";
                        break;
                    }
                    case 0: std::cout << "Update cancelled.\n"; return;
                    default: std::cout << "Invalid choice.\n"; return;
                }

                if (oManager.updateOrder(oid, carpenter, date, days, status)) {
                    std::cout << "Order updated successfully.\n";
                } else {
                    std::cout << "Update failed.\n";
                }
                break;
            }
        }
        if (!found) std::cout << "Order not found.\n";
    }
}