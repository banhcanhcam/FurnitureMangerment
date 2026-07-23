#include "CustomerMenu.h"
#include "SearchDisplay.h"
#include "Validation.h"
#include "UpdateFurniture.h"
#include "UpdateOrder.h"
#include "PersistenceManager.h"

#include <iostream>
#include <iomanip>
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
            "2. Display all, sorted by price (ascending)\n"
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
            "5. View my invoices\n"
            "0. Back to Customer Menu\n"
            "----------------------------------\n";
}

static void showCustomerProfileMenu() {
    cout << "\n----------- MY PROFILE -----------\n"
            "1. View my profile\n"
            "2. Delete my account\n"
            "0. Back to Customer Menu\n"
            "-----------------------------------\n";
}

// ===================== XỬ LÝ CHỨC NĂNG: FURNITURE =====================

// ===================== XỬ LÝ CHỨC NĂNG: ORDER =====================

static void handleCreateOrder(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager, const std::string& username) {
    string oid = readLine("Order ID: ");
    string fid = readLine("Furniture ID: ");
    string carpenter = readLine("Carpenter name: ");
    int days = readDay("Estimated days: ");
    int qty = readNumber("Quantity: ");
    string phone = readPhoneNumber("Contact phone number: ");
    oManager.createOrder(oid, fid, carpenter, days, fManager, username, phone, qty);
    PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
}

static void handleViewMyOrders(OrderManager& oManager, const std::string& username) {
    cout << "\n--- Your orders ---\n";
    SearchDisplay::displayOrdersByCustomer(oManager, username);
}

static void handleCancelMyOrder(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager, const std::string& username) {
    string oid = readLine("Enter Order ID to cancel: ");
    if (oManager.cancelOwnOrder(oid, username)) {
        cout << "Order cancelled successfully.\n";
        PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
    } else {
        cout << "Cancellation failed. (Check ID, status, or ownership)\n";
    }
}

static void handleUpdateMyOrder(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager, const std::string& username) {
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
                PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
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

// Trả về true nếu tài khoản đã bị xóa (để báo hiệu thoát toàn bộ menu)
static bool handleDeleteMyAccount(AccountManager& aManager, FurnitureManager& fManager, OrderManager& oManager, const std::string& username) {
    cout << "Are you sure you want to permanently delete your account '" << username << "'? (y/n): ";
    string confirm = readLine("");
    if (confirm != "y" && confirm != "Y") {
        cout << "Cancelled.\n";
        return false;
    }
    if (aManager.deleteCustomerAccount(username)) {
        cout << "Your account has been deleted. You will now be logged out.\n";
        PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
        return true;
    }
    cout << "Failed to delete account.\n";
    return false;
}

// ===================== VÒNG LẶP SUBMENU (CUSTOMER) =====================

static void runCustomerFurnitureMenu(FurnitureManager& fManager, AccountManager& aManager, OrderManager& oManager) {
    while (true) {
        showCustomerFurnitureMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: SearchDisplay::displayAllFurniture(fManager); break;
            case 2: SearchDisplay::displayAllFurnitureSortedByPrice(fManager); break;
            case 3: UpdateFurniture::updateFurnitureFromInput(fManager, aManager, oManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}
// CustomerMenu.cpp
// Add this before runCustomerOrderMenu() or anywhere before it's called

static void handleViewMyInvoices(OrderManager& oManager, const std::string& username) {
    auto invoices = oManager.getInvoicesByCustomer(username);
    if (invoices.empty()) {
        cout << "You have no invoices yet.\n";
        return;
    }

    cout << "\n--- Your Invoices ---\n";
    cout << std::left
         << std::setw(15) << "Invoice ID"
         << std::setw(12) << "Order ID"
         << std::setw(18) << "Total Amount"
         << std::setw(12) << "Date"
         << std::setw(10) << "Paid"
         << "\n" << std::string(67, '-') << "\n";

    for (const auto& inv : invoices) {
        cout << std::left
             << std::setw(15) << inv.getInvoiceID()
             << std::setw(12) << inv.getOrderID()
             << std::setw(18) << std::fixed << std::setprecision(2) << inv.getTotalAmount()
             << std::setw(12) << inv.getCreatedDate().toString()
             << std::setw(10) << (inv.getIsPaid() ? "Yes" : "No")
             << "\n";
    }

    // Option to view invoice details
    cout << "\nEnter Order ID to view invoice details (or 0 to return): ";
    string oid = readLine("");
    if (oid != "0") {
        Invoice* inv = oManager.findInvoiceByOrderId(oid);
        if (inv) {
            cout << "\n+-------------------------------------------+\n";
            cout << "|          INVOICE DETAILS                  |\n";
            cout << "+-------------------------------------------+\n";
            cout << "| Invoice ID : " << std::left << std::setw(28) << inv->getInvoiceID() << "|\n";
            cout << "| Order ID   : " << std::left << std::setw(28) << inv->getOrderID() << "|\n";
            cout << "| Total      : " << std::left << std::setw(28) << std::fixed << std::setprecision(2) << inv->getTotalAmount() << "|\n";
            cout << "| Date       : " << std::left << std::setw(28) << inv->getCreatedDate().toString() << "|\n";
            cout << "| Paid       : " << std::left << std::setw(28) << (inv->getIsPaid() ? "Yes" : "No") << "|\n";
            
            // Hiển thị thông tin đơn hàng liên quan
            for (const auto& o : oManager.getOrders()) {
                if (o.getOrderID() == oid) {
                    cout << "+-------------------------------------------+\n";
                    cout << "| ORDER DETAILS                            |\n";
                    cout << "+-------------------------------------------+\n";
                    cout << "| Carpenter : " << std::left << std::setw(28) << o.getCarpenterName() << "|\n";
                    cout << "| Furniture : " << std::left << std::setw(28) << o.getFurnitureID() << "|\n";
                    cout << "| Quantity  : " << std::left << std::setw(28) << o.getQuantity() << "|\n";
                    cout << "| Start Date: " << std::left << std::setw(28) << o.getStartDate().toString() << "|\n";
                    break;
                }
            }
            cout << "+-------------------------------------------+\n";
        } else {
            cout << "No invoice found for Order ID: " << oid << "\n";
        }
    }
}

static void runCustomerOrderMenu(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager, const std::string& username) {
    while (true) {
        showCustomerOrderMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleCreateOrder(oManager, fManager, aManager, username); break;
            case 2: handleViewMyOrders(oManager, username); break;
            case 3: handleCancelMyOrder(oManager, fManager, aManager, username); break;
            case 4: handleUpdateMyOrder(oManager, fManager, aManager, username); break;
            case 5: handleViewMyInvoices(oManager, username); break;
            default: cout << "Invalid option!\n";
        }
    }
}

// Trả về true nếu tài khoản đã bị xóa trong submenu này (để báo hiệu logout ngay)
static bool runCustomerProfileMenu(AccountManager& aManager, FurnitureManager& fManager, OrderManager& oManager, const std::string& username) {
    while (true) {
        showCustomerProfileMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return false;

        switch (choice) {
            case 1: handleViewMyProfile(aManager, username); break;
            case 2:
                if (handleDeleteMyAccount(aManager, fManager, oManager, username)) {
                    return true; // báo hiệu logout ngay lập tức
                }
                break;
            default: cout << "Invalid option!\n";
        }
    }
}

// ===================== ENTRY POINT =====================

void runCustomerMenu(FurnitureManager& fManager, OrderManager& oManager, AccountManager& aManager, std::string username) {
    while (true) {
        showCustomerMainMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) break;

        switch (choice) {
            case 1: runCustomerFurnitureMenu(fManager, aManager, oManager); break;
            case 2: runCustomerOrderMenu(oManager, fManager, aManager, username); break;
            case 3:
                if (runCustomerProfileMenu(aManager, fManager, oManager, username)) {
                    return; // Tài khoản đã bị xóa, thoát menu ngay, không dùng username nữa
                }
                break;
            default: cout << "Invalid option!\n";
        }
    }

}