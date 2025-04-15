/**
 * @file Magazine.cpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "Magazine.hpp"

void Magazine::Magazine::displayInfo() const{
    std::cout << "Magazine: " << this->title << " (Issue #" << this->issueNumber 
        << ") (" << (available ? "Available" : "Borrowed") << ")\n";
}

bool Magazine::Magazine::isAvailable() const {
    return this->available;
}

void Magazine::Magazine::borrowItem() {
    if (this->available) {
        this->available = false;
        std::cout << "You have borrowed: " << this->title << " (Issue #" << this->issueNumber << ")\n";
    } 
    else {
        std::cout << "Sorry, " << this->title << " (Issue #" << this->issueNumber << ") is already borrowed.\n";
    }
}

void Magazine::Magazine::returnItem() {
    this->available = true;
    std::cout << "Thank you for returning " << this->title << " (Issue #" << this->issueNumber << ")\n";
}