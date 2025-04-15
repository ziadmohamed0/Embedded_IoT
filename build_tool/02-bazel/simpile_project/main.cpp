/**
 * @file main.cpp
 * @author Ziad Mohammed fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#include "Payment/PaymentMethod/PaymentMethod.hpp"
#include "Payment/CreditCard/CreditCard.hpp"
#include "Payment/PayPal/PayPal.hpp"

int main(void) {
    double amount{0.0};    
    std::string email, CreditCard;

    std::cout << "\n--------------------\n";
    std::cout << "Enter amount: ";
    std::cin >> amount;
    Payment::PaymentMethod PaymentObj(amount);
    
    PaymentObj.set_amount(amount);
    std::cout << "=> Processing credit card payment of $: " << PaymentObj.getter_amount() << std::endl;

    std::cout << "\n\n--------------------\n";
    amount = 0;
    std::cout << "Enter Credit Card Number: ";
    std::cin >> CreditCard;
    std::cout << "Enter amount: ";
    std::cin >> amount;

    CreditCard::CreditCard CreditCardObj(amount, CreditCard);
    CreditCardObj.set_amount(amount);
    CreditCardObj.processPayment();

    std::cout << "\n\n--------------------\n";
    amount = 0;
    std::cout << "Enter E-mail: ";
    std::cin >> email;
    std::cout << "Enter amount: ";
    std::cin >> amount;

    PayPal::PayPal PayPalObj(amount, email);
    CreditCardObj.set_amount(amount);
    PayPalObj.processPayment();

    return 0;
}
