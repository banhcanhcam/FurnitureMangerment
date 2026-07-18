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
            "2. Search furniture by ID\n"
            "3. Find furniture by name\n"
            "4. Display all furniture items\n"
            "5. Update furniture\n"
            "6. Delete a furniture item\n"
            "0. Back to Admin Menu\n"
            "-------------------------------------------\n";
}

static void showAdminOrderMenu() {
    cout << "\n------------ ORDER MANAGEMENT -------------\n"
            "1. Create manufacturing order\n"
            "2. Track orders by status\n"
            "3. Update order\n"
            "4. Delete an order\n"
            "0. Back to Admin Menu\n"
            "-------------------------------------------\n";
}

static void showAdminCustomerMenu() {
    cout << "\n----------- CUSTOMER MANAGEMENT -----------\n"
            "1. Display all customers\n"
            "2. Search customer by username\n"
            "0. Back to Admin Menu\n"
            "-------------------------------------------\n";
}

// ===================== XỬ LÝ CHỨC NĂNG: FURNITURE =====================

static void handleAddFurniture(FurnitureManager& fManager) {
    string id = readLine("ID: ");
    string name = readValidName("Enter Product Name: ");
    int m = readInt("Material (0:Wood, 1:Metal, 2:Aluminum): ");
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
    }
    catch (const std::exception& e) {
        cout << "[Error] " << e.what() << "\n";
    }
}

static void handleSearchFurnitureById(const FurnitureManager& fManager) {
    string searchID = readLine("ID: ");
    auto item = fManager.searchById(searchID);
    if (item) {
        cout << "Found item:" << endl;
        item->display();
    } else {
        cout << "Item not found." << endl;
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

static void handleDeleteFurniture(FurnitureManager& fManager) {
    string fid = readLine("Delete FID: ");
    if (fid.empty()) {
        cout << "Don't have ID to delete!\n";
    } else if (fManager.deleteProduct(fid)) {
        cout << "Furniture item deleted successfully.\n";
    } else {
        cout << "Don't have Furniture Item with ID: " << fid << "\n";
    }
}

// ===================== XỬ LÝ CHỨC NĂNG: ORDER =====================

static void handleCreateOrder(OrderManager& oManager, FurnitureManager& fManager, const string& username) {
    string oid = readLine("OID: ");
    string fid = readLine("FID: ");
    string carpenter = readLineAlpha("Carpenter: ");
    Date date = readDate("Start date");
    int days = readDay("Days: ");
    string phone = readPhoneNumber("Customer contact phone number: ");
    oManager.createOrder(oid, fid, carpenter, date, days, fManager, username, phone);
}

static void handleDeleteOrder(OrderManager& oManager) {
    string oid = readLine("Delete OID: ");
    if (oid.empty()) {
        cout << "Don't have ID to delete!\n";
    } else if (oManager.deleteOrder(oid)) {
        cout << "Order deleted successfully.\n";
    } else {
        cout << "Don't have Order with ID: " << oid << "\n";
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

// ===================== VÒNG LẶP SUBMENU (ADMIN) =====================

static void runAdminFurnitureMenu(FurnitureManager& fManager) {
    while (true) {
        showAdminFurnitureMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleAddFurniture(fManager); break;
            case 2: handleSearchFurnitureById(fManager); break;
            case 3: handleFindFurnitureByName(fManager); break;
            case 4: SearchDisplay::displayAllFurniture(fManager); break;
            case 5: UpdateFurniture::updateFurnitureFromInput(fManager); break;
            case 6: handleDeleteFurniture(fManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

static void runAdminOrderMenu(OrderManager& oManager, FurnitureManager& fManager, const string& username) {
    while (true) {
        showAdminOrderMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleCreateOrder(oManager, fManager, username); break;
            case 2: SearchDisplay::displayOrdersByStatus(oManager, OrderStatus::PENDING); break;
            case 3: UpdateOrder::updateOrderFromInput(oManager); break;
            case 4: handleDeleteOrder(oManager); break;
            default: cout << "Invalid option!\n";
        }
    }
}

static void runAdminCustomerMenu(AccountManager& aManager) {
    while (true) {
        showAdminCustomerMenu();
        int choice = readNumber("Option: ");
        if (choice == 0) return;

        switch (choice) {
            case 1: handleDisplayAllCustomers(aManager); break;
            case 2: handleSearchCustomer(aManager); break;
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
            case 1: runAdminFurnitureMenu(fManager); break;
            case 2: runAdminOrderMenu(oManager, fManager, username); break;
            case 3: runAdminCustomerMenu(aManager); break;
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
    PersistenceManager::loadAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", fManager, authSystem, oManager);

    // --- CỔNG ĐĂNG NHẬP ---
    while (loggedInUser == nullptr) {
        cout << "\n===== WELCOME TO FURNITURE SYSTEM =====\n";
        cout << "1. Login\n2. Register Customer\n3. Register Admin\n0. Exit\n";
        int choice = readNumber("Choose: ");
        if (choice == 0) return 0;

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

    if (!PersistenceManager::saveAllData("furniture.txt", "admin.txt", "customer.txt", "order.txt", fManager, authSystem, oManager)) {
        std::cerr << "Warning: Some data may not have been saved.\n";
    } else {
        std::cout << "All data saved successfully.\n";
    }

    return 0;
}