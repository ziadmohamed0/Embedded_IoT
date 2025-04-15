/**
 * @file main.cpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "Library/Items/item.hpp"
#include "Library/Books/books.hpp"
#include "Library/Magazines/Magazine.hpp"
#include <vector>
#include <memory>

int main () {
    std::vector<items::LibraryItem*> library;

    library.push_back(new Books::Books("The Great Gatsby", "F. Scott Fitzgerald"));
    library.push_back(new Books::Books("1984", "George Orwell"));
    library.push_back(new Magazine::Magazine("National Geographic", 256));
    library.push_back(new Magazine::Magazine("Scientific American", 123));

    std::cout << "Library Items:\n";
    for (const auto& item : library) {
        item->displayInfo();
    }

    std::cout << "\nBorrowing items:\n";
    library[0]->borrowItem();  
    library[2]->borrowItem();  
    library[0]->borrowItem();  

    std::cout << "\nReturning items:\n";
    library[0]->returnItem();  

    std::cout << "\nFinal status:\n";
    for (const auto& item : library) {
        item->displayInfo();
    }

    for (auto item : library) {
        delete item;
    }

    library.clear();

    return 0;
    return 0;
}