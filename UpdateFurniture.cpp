#include "UpdateFurniture.h"
#include "Validation.h"
#include <iostream>

namespace UpdateFurniture {
    void updateFurnitureFromInput(FurnitureManager& fManager) {
        std::string id = readLine("Enter Furniture ID to update: ");
        auto item = fManager.searchById(id);
        if (!item) {
            std::cout << "Furniture ID not found.\n";
            return;
        }

        // Hiển thị thông tin hiện tại
        std::cout << "\nCurrent Furniture Information:\n";
        std::cout << "ID: " << item->getFurnitureID() << "\n";
        std::cout << "Name: " << item->getName() << "\n";
        std::cout << "Price: " << item->getPrice() << "\n";
        std::cout << "Quantity: " << item->getQuantity() << "\n";
        std::cout << "Width: " << item->getWidth() << "\n";
        std::cout << "Height: " << item->getHeight() << "\n";
        std::cout << "Depth: " << item->getDepth() << "\n";
        std::cout << "Color: " << item->getBaseColor() << "\n";

        std::cout << "\nWhich field do you want to update?\n";
        std::cout << "1. Name\n2. Price\n3. Quantity\n4. Width\n5. Height\n6. Depth\n7. Color\n0. Cancel\n";
        int choice = readNumber("Your choice: ");

        std::string name = item->getName();
        double price = item->getPrice();
        int qty = item->getQuantity();
        double w = item->getWidth(), h = item->getHeight(), d = item->getDepth();
        std::string color = item->getBaseColor();

        switch (choice) {
            case 1: name = readLine("New name: "); break;
            case 2: price = readDouble("New price: "); break;
            case 3: qty = readNumber("New quantity: "); break;
            case 4: w = readDouble("New width: "); break;
            case 5: h = readDouble("New height: "); break;
            case 6: d = readDouble("New depth: "); break;
            case 7: color = readLine("New color: "); break;
            case 0: std::cout << "Update cancelled.\n"; return;
            default: std::cout << "Invalid choice.\n"; return;
        }

        if (fManager.updateFurniture(id, name, price, qty, w, h, d, color)) {
            std::cout << "Furniture updated successfully.\n";
        } else {
            std::cout << "Update failed.\n";
        }
    }
}