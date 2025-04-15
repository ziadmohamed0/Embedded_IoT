/**
 * @file PaymentMethod.cpp
 * @author Ziad Mohammed fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "PaymentMethod.hpp"

void Payment::PaymentMethod::set_amount(double copyNewAmount) {
    this->amount = copyNewAmount;
}

void Payment::PaymentMethod::processPayment(void) const {
    std::cout << "=> Processing credit card payment of $: " << this->amount << std::endl;
}
