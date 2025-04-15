/**
 * @file CreditCard.cpp
 * @author Ziad Mohammed fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "CreditCard.hpp"

/**
 * @brief 
 * 
 */
void CreditCard::CreditCard::processPayment(void) const{
    std::cout << "=> Number of card: " << this->cardNumber << std::endl;
    std::cout << "=> Processing credit card payment of $: " << this->amount << std::endl;
}