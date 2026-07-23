#include <cctype>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <memory>
#include "UpdateFurniture.h"
#include "UpdateOrder.h"
#include "AccountManager.h"
#include "Validation.h"
#include "DerivedFurniture.h"
#include "FurnitureManager.h"
#include "OrderManager.h"
#include "Product.h"
#include "SearchDisplay.h"
#include "PersistenceManager.h"
#include "CustomerMenu.h"
#include "ProductValidation.h"


using namespace std;

// ===================== MENU HIỂN THỊ =====================

static void showAdminMainMenu() {
    cout << "\n================= ADMIN MENU =================\n"
            "1. Furniture Management\n"
            "2. Order Management\n"
            "3. Customer Management\n"
            "0. Logout\n"
            "==============================================\n";
}

static void showAdminFurnitureMenu() {
    cout << "\n---------- FURNITURE MANAGEMENT ----------\n"
            "1. Add new furniture item\n"
            "2. Find furniture by name\n"
            "3. Display all furniture items\n"
            "4. Display all, sorted by price (ascending)\n"
            "5. Update furniture\n"
            "6. Delete a furniture item\n"
            "0. Back to Admin Menu\n"
            "-------------------------------------------\n";
}

static void showAdminOrderMenu() {
    cout << "\n------------ ORDER MANAGEMENT -------------\n"
            "1. Create manufacturing order\n"
            "2. Display orders (filter by status)\n"
            "3. Update order\n"
            "4. Delete an order\n"
            "5. Display all invoices\n"
            "0. Back to Admin Menu\n"
            "-------------------------------------------\n";
}

static void showAdminCustomerMenu() {
    cout << "\n----------- CUSTOMER MANAGEMENT -----------\n"
            "1. Display all customers\n"
            "2. Search customer by username\n"
            "3. Delete a customer account\n"
            "0. Back to Admin Menu\n"
            "-------------------------------------------\n";
}

// ===================== XỬ LÝ CHỨC NĂNG: FURNITURE =====================

static void handleAddFurniture(FurnitureManager& fManager, AccountManager& aManager, OrderManager& oManager) {
    string id = readLine("ID: ");
    string name = readUniqueFurnitureName("Enter Product Name: ", fManager.getInventory());
    int m = readIntRange("Material (0:Wood, 1:Metal, 2:Aluminum): ",0 ,2);
    double w = readDouble("Width(cm): ");
    double h = readDouble("Height(cm): ");
    double d = readDouble("Depth(cm): ");
    string c = readLineAlpha("Color: ");
    double price = readDouble("Price($): ");
    int quantity = readNumber("Quantity: ");
    shared_ptr<FurnitureBase> f;
    try {
        if (m == 0) f = make_shared<WoodFurniture>(id, w, h, d, c, name, price, quantity);
        else if (m == 1) f = make_shared<MetalFurniture>(id, name, w, h, d, price, quantity, c);
        else f = make_shared<AluminumFurniture>(id, name, w, h, d, price, quantity, c);
        fManager.addFurniture(f);
        cout << "\n>>> Success: Added new furniture item (Name: " << name << ", ID: " << id << ") successfully! <<<\n";
        PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
    }
    
    catch (const std::exception& e) {
        cout << "[Error] " << e.what() << "\n";
    }
}

static void handleFindFurnitureByName(const FurnitureManager& fManager) {
    string searchName = readLine("Enter Name to search: ");
    auto results = SearchDisplay::searchFurnitureByName(fManager, searchName);

    if (results.empty()) {
        cout << "No furniture found with name: " << searchName << "\n";
    } else {
        cout << "\nFound " << results.size() << " items matching '" << searchName << "':\n";
        for (const auto& item : results) {
            cout << "ID: " << item->getFurnitureID()
                 << " | Name: " << item->getName()
                 << " | Price: " << item->getPrice()
                 << " | Qty: " << item->getQuantity()
                 << " | Color: " << item->getBaseColor()
                 << " | WxHxD: " << item->getWidth() << "x"
                 << item->getHeight() << "x" << item->getDepth() << "\n";
        }
    }
}

static void handleDeleteFurniture(FurnitureManager& fManager, AccountManager& aManager, OrderManager& oManager) {
    string fid = readLine("Delete FID: ");
    if (fid.empty()) {
        cout << "Don't have ID to delete!\n";
    } else if (fManager.deleteProduct(fid)) {
        cout << "Furniture item deleted successfully.\n";
        PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
    } else {
        cout << "Don't have Furniture Item with ID: " << fid << "\n";
    }
}

// ===================== XỬ LÝ CHỨC NĂNG: ORDER =====================

static void handleCreateOrder(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager, const string& username) {
    string oid = readLine("OID: ");
    string fid = readLine("FID: ");
    string carpenter = readLineAlpha("Carpenter: ");
    int days = readDay("Days: ");
    int qty = readNumber("Quantity: ");
    string phone = readPhoneNumber("Customer contact phone number: ");
    oManager.createOrder(oid, fid, carpenter, days, fManager, username, phone, qty);
    PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
}

static void handleDisplayOrders(OrderManager& oManager) {
    cout << "\nFilter by status:\n"
            "1. PENDING\n"
            "2. IN_PROGRESS\n"
            "3. COMPLETED\n"
            "4. ALL\n";
    int choice = readIntRange("Your choice: ", 1, 4);

    switch (choice) {
        case 1: SearchDisplay::displayOrdersByStatus(oManager, OrderStatus::PENDING); break;
        case 2: SearchDisplay::displayOrdersByStatus(oManager, OrderStatus::IN_PROGRESS); break;
        case 3: SearchDisplay::displayOrdersByStatus(oManager, OrderStatus::COMPLETED); break;
        case 4: SearchDisplay::displayAllOrders(oManager); break;
    }
}

static void handleDeleteOrder(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager) {
    string oid = readLine("Delete OID: ");
    if (oid.empty()) {
        cout << "Don't have ID to delete!\n";
    } else if (oManager.deleteOrder(oid)) {
        cout << "Order deleted successfully.\n";
        PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
    } else {
        cout << "Don't have Order with ID: " << oid << "\n";
    }
}
// main.cpp - Add with other order handlers

static void handleDisplayAllInvoices(OrderManager& oManager) {
    const auto& invoices = oManager.getAllInvoices();
    if (invoices.empty()) {
        cout << "No invoices found.\n";
        return;
    }

    cout << "\n--- ALL INVOICES ---\n";
    cout << std::left
         << std::setw(15) << "Invoice ID"
         << std::setw(15) << "Order ID"
         << std::setw(12) << "Customer"
         << std::setw(18) << "Total Amount"
         << std::setw(12) << "Date"
         << std::setw(10) << "Paid"
         << "\n" << std::string(82, '-') << "\n";

    for (const auto& inv : invoices) {
        // Find customer username
        std::string customer = "Unknown";
        for (const auto& o : oManager.getOrders()) {
            if (o.getOrderID() == inv.getOrderID()) {
                customer = o.getCustomerUsername();
                break;
            }
        }
        cout << std::left
             << std::setw(15) << inv.getInvoiceID()
             << std::setw(15) << inv.getOrderID()
             << std::setw(12) << customer
             << std::setw(18) << std::fixed << std::setprecision(2) << inv.getTotalAmount()
             << std::setw(12) << inv.getCreatedDate().toString()
             << std::setw(10) << (inv.getIsPaid() ? "Yes" : "No")
             << "\n";
    }
}
// ===================== XỬ LÝ CHỨC NĂNG: CUSTOMER =====================

static void handleDisplayAllCustomers(AccountManager& aManager) {
    auto customers = aManager.getAllCustomers();
    if (customers.empty()) {
        cout << "No customer accounts found.\n";
        return;
    }
    cout << std::left << std::setw(20) << "Username" << std::setw(15) << "Phone Number" << "\n"
         << std::string(35, '-') << "\n";
    for (const auto& c : customers) {
        cout << std::left << std::setw(20) << c.username
             << std::setw(15) << (c.phoneNumber.empty() ? "(not set)" : c.phoneNumber) << "\n";
    }
}

static void handleSearchCustomer(AccountManager& aManager) {
    string username = readLine("Enter customer username: ");
    UserAccount* acc = aManager.findAccount(username);
    if (!acc || acc->role != UserRole::CUSTOMER) {
        cout << "Customer not found.\n";
        return;
    }
    cout << "Username: " << acc->username << "\n";
    cout << "Phone Number: " << (acc->phoneNumber.empty() ? "(not set)" : acc->phoneNumber) << "\n";
}

static void handleDeleteCustomer(AccountManager& aManager, FurnitureManager& fManager, OrderManager& oManager) {
    string username = readLine("Enter customer username to delete: ");
    UserAccount* acc = aManager.findAccount(username);
    if (!acc || acc->role != UserRole::CUSTOMER) {
        cout << "Customer not found.\n";
        return;
    }
    cout << "Are you sure you want to delete customer '" << username << "'? (y/n): ";
    string confirm = readLine("");
    if (confirm != "y" && confirm != "Y") {
        cout << "Cancelled.\n";
        return;
    }
    if (aManager.deleteCustomerAccount(username)) {
        cout << "Customer account deleted successfully.\n";
        PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, aManager, oManager, true);
    } else {
        cout << "Failed to delete customer account.\n";
    }
}

// ===================== VÒNG LẶP SUBMENU (ADMIN) =====================

static void runAdminFurnitureMenu(FurnitureManager& fManager, AccountManager& aManager, OrderManager& oManager) {
    while (true) {
        showAdminFurnitureMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleAddFurniture(fManager, aManager, oManager); break;
            case 2: handleFindFurnitureByName(fManager); break;
            case 3: SearchDisplay::displayAllFurniture(fManager); break;
            case 4: SearchDisplay::displayAllFurnitureSortedByPrice(fManager); break;
            case 5: UpdateFurniture::updateFurnitureFromInput(fManager, aManager, oManager); break;
            case 6: handleDeleteFurniture(fManager, aManager, oManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

static void runAdminOrderMenu(OrderManager& oManager, FurnitureManager& fManager, AccountManager& aManager, const string& username) {
    while (true) {
        showAdminOrderMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleCreateOrder(oManager, fManager, aManager, username); break;
            case 2: handleDisplayOrders(oManager); break;
            case 3: UpdateOrder::updateOrderFromInput(oManager, fManager, aManager); break;
            case 4: handleDeleteOrder(oManager, fManager, aManager); break;
            case 5: handleDisplayAllInvoices(oManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

static void runAdminCustomerMenu(AccountManager& aManager, FurnitureManager& fManager, OrderManager& oManager) {
    while (true) {
        showAdminCustomerMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleDisplayAllCustomers(aManager); break;
            case 2: handleSearchCustomer(aManager); break;
            case 3: handleDeleteCustomer(aManager, fManager, oManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

static void runAdminMenu(FurnitureManager& fManager, OrderManager& oManager, AccountManager& aManager, const string& username) {
    while (true) {
        showAdminMainMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) break;

        switch (choice) {
            case 1: runAdminFurnitureMenu(fManager, aManager, oManager); break;
            case 2: runAdminOrderMenu(oManager, fManager, aManager, username); break;
            case 3: runAdminCustomerMenu(aManager, fManager, oManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

// ===================== MAIN =====================

int main() {
    AccountManager authSystem;
    UserAccount* loggedInUser = nullptr;
    FurnitureManager fManager;
    OrderManager oManager;
    PersistenceManager::loadAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, authSystem, oManager);

    // --- CỔNG ĐĂNG NHẬP ---
    while (loggedInUser == nullptr) {
        cout << "\n===== WELCOME TO FURNITURE SYSTEM =====\n";
        cout << "1. Login\n2. Register Customer\n3. Register Admin\n0. Exit\n";
        int choice = readNumber("Choose: ");
        if (choice == 0) {
            // Lưu dữ liệu trước khi thoát, tránh mất dữ liệu (VD: vừa đăng ký xong rồi thoát ngay)
            PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, authSystem, oManager);
            return 0;
        }

        if (choice == 1) {
            string u, p;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> p;
            loggedInUser = authSystem.login(u, p);
            if (!loggedInUser) cout << "Login failed!\n";
        }
        else if (choice == 2 || choice == 3) {
            string u, p;
            cout << "New Username: "; cin >> u;
            cout << "New Password: "; cin >> p;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            UserRole r = (choice == 2) ? UserRole::CUSTOMER : UserRole::ADMIN;
            string phone;
            if (choice == 2) {
                phone = readPhoneNumber("Phone Number: ");
            }
            if (authSystem.registerAccount(u, p, r, phone)) {
                cout << "Account created successfully.\n";
                // Lưu ngay lập tức, không đợi đến khi đóng chương trình
                PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, authSystem, oManager, true);
            } else {
                cout << "Username already exists.\n";
            }
        }
    }

    // --- SAU KHI ĐĂNG NHẬP ---
    if (loggedInUser->role == UserRole::ADMIN) {
        runAdminMenu(fManager, oManager, authSystem, loggedInUser->username);
    } else {
        runCustomerMenu(fManager, oManager, authSystem, loggedInUser->username);
    }

    if (!PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", "invoice.txt", fManager, authSystem, oManager, true)) {
        std::cerr << "Warning: Some data may not have been saved.\n";
    } else {
        std::cout << "All data saved successfully.\n";
    }

    return 0;
}