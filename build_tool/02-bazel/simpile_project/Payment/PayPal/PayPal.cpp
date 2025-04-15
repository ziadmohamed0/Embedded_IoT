/**
 * @file PayPal.cpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "PayPal.hpp"

void PayPal::PayPal::processPayment(void) const {
    std::cout << "=> E-mail: " << this->email << std::endl;
    std::cout << "=> Processing credit card payment of $: " << this->amount << std::endl;
}
