#include "CustomerMenu.h"
#include "SearchDisplay.h"
#include "Validation.h"
#include <iostream>
#include <limits>

using namespace std;

// Các hàm nhập liệu (có thể tách riêng, nhưng ở đây dùng nội bộ)
static int readNumber(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value < 0) {
                cout << "Error: Value must be >= 0.\n";
                continue;
            }
            return value;
        }
        cout << "Invalid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static int readDay(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value <= 0) {
                cout << "Error: Days must be > 0.\n";
                continue;
            }
            return value;
        }
        cout << "Invalid.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}


static void showCustomerMenu() {
    cout << "\n=============== CUSTOMER MENU ===============\n"
            "1. Display all furniture items\n"
            "2. Find furniture by name\n"
            "3. Create a new order\n"
            "4. View my orders\n"
            "5. Cancel my order (only pending)\n"
            "0. Logout\n"
            "===============================================\n";
}

void runCustomerMenu(FurnitureManager& fManager, OrderManager& oManager, const std::string& username) {
    while (true) {
        showCustomerMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                fManager.displayAll();
                break;
            }
            case 2: {
                string name = readLine("Enter name to search: ");
                auto results = SearchDisplay::searchFurnitureByName(fManager, name);
                if (results.empty()) {
                    cout << "No furniture found with that name.\n";
                } else {
                    cout << "\nFound " << results.size() << " item(s):\n";
                    for (const auto& item : results) {
                        cout << "ID: " << item->getFurnitureID()
                             << " | Name: " << item->getName()
                             << " | Price: " << item->getPrice()
                             << " | Quantity: " << item->getQuantity()
                             << " | Color: " << item->getBaseColor()
                             << " | Size: " << item->getWidth() << "x"
                             << item->getHeight() << "x" << item->getDepth() << "\n";
                    }
                }
                break;
            }
            case 3: {
                string oid = readLine("Order ID: ");
                string fid = readLine("Furniture ID: ");
                string carpenter = readLine("Carpenter name: ");
                string date = readLine("Start date (DD/MM/YYYY): ");
                int days = readDay("Estimated days: ");
                oManager.createOrder(oid, fid, carpenter, date, days, fManager, username);
                break;
            }
            case 4: {
                const auto& allOrders = oManager.getOrders();
                bool found = false;
                cout << "\n--- Your orders ---\n";
                for (const auto& o : allOrders) {
                    if (o.getCustomerUsername() == username) {
                        cout << "Order ID: " << o.getOrderID()
                             << " | Furniture: " << o.getFurnitureID()
                             << " | Status: ";
                        switch (o.getStatus()) {
                            case OrderStatus::PENDING: cout << "PENDING"; break;
                            case OrderStatus::IN_PROGRESS: cout << "IN_PROGRESS"; break;
                            case OrderStatus::COMPLETED: cout << "COMPLETED"; break;
                        }
                        cout << " | Cost: " << o.getLaborCost() << "\n";
                        found = true;
                    }
                }
                if (!found) cout << "You have no orders yet.\n";
                break;
            }
            case 5: {
                string oid = readLine("Enter Order ID to cancel: ");
                if (oManager.cancelOwnOrder(oid, username)) {
                    cout << "Order cancelled successfully.\n";
                } else {
                    cout << "Cancellation failed. (Check ID, status, or ownership)\n";
                }
                break;
            }
            default:
                cout << "Invalid option!\n";
        }
    }
}