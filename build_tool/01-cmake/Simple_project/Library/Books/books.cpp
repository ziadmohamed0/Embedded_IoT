/**
 * @file books.cpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "books.hpp"
#include <iostream>

void Books::Books::displayInfo() const {
    std::cout << "Book: " << this->title << " by " << this->author 
              << " (" << (this->available ? "Available" : "Borrowed") << ")\n";
}

bool Books::Books::isAvailable() const {
    return this->available;
}

void Books::Books::borrowItem(void) {
    if (this->available) {
        this->available = false;
        std::cout << "You have borrowed: " << this->title << "\n";
    } 
    else {
        std::cout << "Sorry, " << this->title << " is already borrowed.\n";
    }
}

void Books::Books::returnItem(void) {
    this->available = true;
    std::cout << "Thank you for returning " << this->title << "\n";
}