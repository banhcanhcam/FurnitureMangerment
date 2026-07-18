#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <memory>
#include "AccountManager.h"
#include "Validation.h"
#include "DerivedFurniture.h"
#include "FurnitureManager.h"
#include "OrderManager.h"
#include "Product.h"
#include "SearchDisplay.h"
#include "PersistenceManager.h"
#include "CustomerMenu.h"
#include <cctype>

using namespace std;

// --- CÁC HÀM HỖ TRỢ NHẬP LIỆU ---
static int readDay(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value <= 0) {
                cout << "Error: Days must be greater than 0. Please try again.\n";
                continue;
            }
            return value;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a valid integer number.\n";
        }
    }
}
static int readNumber(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            // KIỂM TRA: Nếu là số âm hoặc bằng 0 thì bắt nhập lại
            if (value < 0) {
                cout << "Error: Value must be greater than 0. Please try again.\n";
                continue;
			}
			return value;
		}
	}
}
static int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            
            if (value < 0) {
                cout << "Error: Value must be greater than 0. Please try again.\n";
                continue;
			}
            else if (value > 2){ 
                cout << "Error: Value must be smaller than 3. Please try again.\n";
                continue; 
			}
            }
            return value;
        }
        cout << "Invalid! Please enter a valid integer number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

static double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            // KIỂM TRA: Chặn kích thước âm hoặc bằng 0
            if (value <= 0) {
                cout << "Error: Dimension must be greater than 0. Please try again.\n";
                continue; 
            }
            return value;
        }
        cout << "Invalid! Please enter a valid decimal number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static string readLineAlpha(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        
        bool allLetters = true;
        // Kiểm tra từng ký tự trong chuỗi người dùng nhập
        for (char c : line) {
            // !isalpha(c) nghĩa là "không phải chữ"
            // !isspace(c) nghĩa là "không phải khoảng trắng"
            // Nếu không phải chữ và không phải khoảng trắng -> Báo lỗi
            if (!isalpha(c) && !isspace(c)) { 
                allLetters = false;
                break;
            }
        }
        
        // Nếu tất cả đều là chữ và chuỗi không bị rỗng thì trả về
        if (allLetters && !line.empty()) {
            return line;
        }
        cout << "Invalid input, please enter letters only (no numbers).\n";
    }
}

// --- MENU CŨ CỦA BẠN (Đã tích hợp) ---
static void showAdminMenu() {
    cout << "\n================= ADMIN MENU =================\n"
            "1. Add new Furniture Item\n"
            "2. Search Furniture by ID\n"
            "3. Display all Furniture Items\n"
            "4. Create Manufacturing Order\n"
            "5. Track Orders by Status\n"
            "6. Delete a Furniture Item\n"
            "7. Delete a Manufacturing Order\n"
            "8. Find Furniture\n"
            "0. Logout\n"
            "==============================================\n";
}

int main() {
    AccountManager authSystem;
    UserAccount* loggedInUser = nullptr;
    FurnitureManager fManager;
    OrderManager oManager;

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
        } else if (choice == 2 || choice == 3) {
            string u, p;
            cout << "New Username: "; cin >> u;
            cout << "New Password: "; cin >> p;
            UserRole r = (choice == 2) ? UserRole::CUSTOMER : UserRole::ADMIN;
            authSystem.registerAccount(u, p, r);
        }
    }

    // --- SAU KHI ĐĂNG NHẬP ---
    
    if (loggedInUser->role == UserRole::ADMIN) {
        while (true) {
            showAdminMenu();
            int choice = readNumber("Option: ");
            if (choice == 0) break;

            switch (choice) {
                case 1: {
                    string id = readLine("ID: ");
                    string name = readValidName("Enter Product Name: ");
                    int m = readInt("Material (0:Wood, 1:Metal, 2:Aluminum): ");
                    double w = readDouble("Width(cm): ");
                    double h = readDouble("Height(cm): ");
                    double d = readDouble("Depth(cm): "); 
                    string c = readLineAlpha("Color: ");
                    double price = readDouble("Price($): ");
                    int quantity = readNumber ("Quantity: ");
                    shared_ptr<FurnitureBase> f;
                    try {
                    if (m == 0) f = make_shared<WoodFurniture>(id, w, h, d, c, name, price, quantity);
                    else if (m == 1) f = make_shared<MetalFurniture>(id, name, w, h, d, price, quantity, c);
                    else f = make_shared<AluminumFurniture>(id, name, w, h, d, price, quantity, c); 
                    fManager.addFurniture(f);
                    cout << "\n>>> Success: Added new furniture item (Name: " << name << ", "<< "ID: "<<  id <<") successfully! <<<\n";
                }   
				catch (const DuplicateIdException& e) {
                cout << "[Error] " << e.what() << "\n";
                
                }
                break;
            }
                // Ví dụ đoạn code trong main.cpp hoặc menu handler
                case 2: {
                    std::string searchID;
                    std::cout << "ID: ";
                    std::cin >> searchID;
    
                        auto item = fManager.searchById(searchID); // Giả sử hàm này trả về shared_ptr<FurnitureBase>
                        if (item) {
                        std::cout << "Found item:" << std::endl;
                        item->display(); // <--- GỌI HÀM NÀY ĐỂ HIỂN THỊ THÔNG SỐ
                    } else {
                        std::cout << "Item not found." << std::endl;
                    }
                break;
           }
                case 3: fManager.displayAll(); break;
                // Gọi với 7 tham số: OID, FID, Carpenter, Date, Days, fManager, username
                case 4: 
                    oManager.createOrder(readLine("OID: "), readLine("FID: "), 
                                         readLineAlpha("Carpenter: "), readLine("Date: "), 
                                         readDay("Days: "), fManager, loggedInUser->username);
                    break;
                case 5: oManager.trackByStatus(OrderStatus::PENDING); break;
                case 8: {
    string searchName = readLine("Enter Name to search: ");
    auto results = SearchDisplay::searchFurnitureByName(fManager, searchName);
    
    if (results.empty()) {
        cout << "No furniture found with name: " << searchName << "\n";
    } else {
        cout << "\nFound " << results.size() << " items matching '" << searchName << "':\n";
        for (const auto& item : results) {
            // Hiển thị chi tiết từng sản phẩm
            cout << "ID: " << item->getFurnitureID() 
                 << " | Name: " << item->getName() 
                 << " | Price: " << item->getPrice() 
                 << " | Qty: " << item->getQuantity() 
                 << " | Color: " << item->getBaseColor() 
                 << " | WxHxD: " << item->getWidth() << "x" 
                 << item->getHeight() << "x" << item->getDepth() << "\n";
        }
    }
    break;
}
                case 9: {
                    cout << "--- STATISTICS / SORT ---\n";
                    // Gọi hàm thống kê hoặc sắp xếp của fManager
                    cout << "Tính năng thống kê đang được phát triển...\n";
                    break;
                }
                case 6: {
    string fid = readLine("Delete FID: ");
    if (fid.empty()) {
        cout << "Don't have ID to delete!\n";
    } else if (fManager.deleteProduct(fid)) {
        cout << "Furniture item deleted successfully.\n";
    } else {
        cout << "Don't have Furniture Item with ID: " << fid << "\n";
    }
    break;
}
                case 7: {
    string oid = readLine("Delete OID: ");
    if (oid.empty()) {
        cout << "Don't have ID to delete!\n";
    } else if (oManager.deleteOrder(oid)) {
        cout << "Order deleted successfully.\n";
    } else {
        cout << "Don't have Order with ID: " << oid << "\n";
    }
    break;
}
                default: cout << "Invalid!\n";
            }
        }
    } else {
        runCustomerMenu(fManager, oManager, loggedInUser->username);
    }
PersistenceManager::saveAllData("furniture.txt", "account.txt", fManager, authSystem);
std::cout << "Data has been save succesfully" << std::endl;

    return 0;
}