#include "CustomerMenu.h"
#include "SearchDisplay.h"
#include "Validation.h"
#include "UpdateFurniture.h"
#include "UpdateOrder.h"
#include <iostream>
#include <limits>

using namespace std;

// ===================== MENU HIỂN THỊ =====================

static void showCustomerMainMenu() {
    cout << "\n=============== CUSTOMER MENU ===============\n"
            "1. Furniture\n"
            "2. My Orders\n"
            "3. My Profile\n"
            "0. Logout\n"
            "===============================================\n";
}

static void showCustomerFurnitureMenu() {
    cout << "\n---------- FURNITURE ----------\n"
            "1. Display all furniture items\n"
            "2. Find furniture by ID\n"
            "3. Update furniture (search by ID)\n"
            "0. Back to Customer Menu\n"
            "--------------------------------\n";
}

static void showCustomerOrderMenu() {
    cout << "\n----------- MY ORDERS -----------\n"
            "1. Create a new order\n"
            "2. View my orders\n"
            "3. Cancel my order (only pending)\n"
            "4. Update my order (only pending)\n"
            "0. Back to Customer Menu\n"
            "----------------------------------\n";
}

// ===================== XỬ LÝ CHỨC NĂNG: FURNITURE =====================

static void handleFindFurnitureById(FurnitureManager& fManager) {
    std::string searchID = readLine("ID: ");
    auto item = SearchDisplay::searchFurnitureById(fManager, searchID);
    if (item) {
        std::cout << "Found item:\n";
        SearchDisplay::printFurniture(item);
    } else {
        std::cout << "Item not found.\n";
    }
}

// ===================== XỬ LÝ CHỨC NĂNG: ORDER =====================

static void handleCreateOrder(OrderManager& oManager, FurnitureManager& fManager, const std::string& username) {
    string oid = readLine("Order ID: ");
    string fid = readLine("Furniture ID: ");
    string carpenter = readLine("Carpenter name: ");
    Date date = readDate("Start date");
    int days = readDay("Estimated days: ");
    string phone = readPhoneNumber("Contact phone number: ");
    oManager.createOrder(oid, fid, carpenter, date, days, fManager, username, phone);
}

static void handleViewMyOrders(OrderManager& oManager, const std::string& username) {
    const auto& allOrders = oManager.getOrders();
    bool found = false;
    cout << "\n--- Your orders ---\n";
    for (const auto& o : allOrders) {
        if (o.getCustomerUsername() == username) {
            cout << "Order ID: " << o.getOrderID()
                 << " | Furniture: " << o.getFurnitureID()
                 << " | Start Date: " << o.getStartDate().toString()
                 << " | Phone: " << o.getPhoneNumber()
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
}

static void handleCancelMyOrder(OrderManager& oManager, const std::string& username) {
    string oid = readLine("Enter Order ID to cancel: ");
    if (oManager.cancelOwnOrder(oid, username)) {
        cout << "Order cancelled successfully.\n";
    } else {
        cout << "Cancellation failed. (Check ID, status, or ownership)\n";
    }
}

static void handleUpdateMyOrder(OrderManager& oManager, const std::string& username) {
    string oid = readLine("Enter your Order ID to update: ");
    bool own = false;
    for (auto& o : oManager.getOrders()) {
        if (o.getOrderID() == oid && o.getCustomerUsername() == username) {
            if (o.getStatus() != OrderStatus::PENDING) {
                cout << "Cannot update order that is not PENDING.\n";
                own = true;
                break;
            }
            cout << "Current info:\n";
            cout << "Carpenter: " << o.getCarpenterName() << "\n";
            cout << "Start Date: " << o.getStartDate().toString() << "\n";
            cout << "Days: " << o.getEstimatedDays() << "\n";
            string carpenter = readLine("New carpenter (or Enter to keep): ");
            if (carpenter.empty()) carpenter = o.getCarpenterName();
            Date date = readDateOrKeep("New start date", o.getStartDate());
            int days = readNumber("New days (-1 to keep): ");
            if (days == -1) days = o.getEstimatedDays();
            if (oManager.updateOrder(oid, carpenter, date, days, o.getStatus())) {
                cout << "Order updated.\n";
            } else {
                cout << "Update failed.\n";
            }
            own = true;
            break;
        }
    }
    if (!own) cout << "Order not found or not yours.\n";
}

// ===================== XỬ LÝ CHỨC NĂNG: PROFILE =====================

static void handleViewMyProfile(AccountManager& aManager, const std::string& username) {
    UserAccount* acc = aManager.findAccount(username);
    if (!acc) {
        cout << "Profile not found.\n";
        return;
    }
    cout << "\n--- My Profile ---\n";
    cout << "Username: " << acc->username << "\n";
    cout << "Phone Number: " << (acc->phoneNumber.empty() ? "(not set)" : acc->phoneNumber) << "\n";
}

// ===================== VÒNG LẶP SUBMENU (CUSTOMER) =====================

static void runCustomerFurnitureMenu(FurnitureManager& fManager) {
    while (true) {
        showCustomerFurnitureMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: SearchDisplay::displayAllFurniture(fManager); break;
            case 2: handleFindFurnitureById(fManager); break;
            case 3: UpdateFurniture::updateFurnitureFromInput(fManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

static void runCustomerOrderMenu(OrderManager& oManager, FurnitureManager& fManager, const std::string& username) {
    while (true) {
        showCustomerOrderMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleCreateOrder(oManager, fManager, username); break;
            case 2: handleViewMyOrders(oManager, username); break;
            case 3: handleCancelMyOrder(oManager, username); break;
            case 4: handleUpdateMyOrder(oManager, username); break;
            default: cout << "Invalid option!\n";
        }
    }
}

// ===================== ENTRY POINT =====================

void runCustomerMenu(FurnitureManager& fManager, OrderManager& oManager, AccountManager& aManager, const std::string& username) {
    while (true) {
        showCustomerMainMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) break;

        switch (choice) {
            case 1: runCustomerFurnitureMenu(fManager); break;
            case 2: runCustomerOrderMenu(oManager, fManager, username); break;
            case 3: handleViewMyProfile(aManager, username); break;
            default: cout << "Invalid option!\n";
        }
    }
}