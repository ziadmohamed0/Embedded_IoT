/**
 * @file CreditCard.hpp
 * @author Ziad Mohammed fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#ifndef _PAYMENT_CREDIT_CARD_HPP_
#define _PAYMENT_CREDIT_CARD_HPP_

#include "../PaymentMethod/PaymentMethod.hpp"

namespace CreditCard {

    class CreditCard : public Payment::PaymentMethod {
        private:
            std::string  cardNumber; // A private data member to store the credit card number
        public:
            /**
            * @brief Construct a new Credit Card object
            * 
            * @param copyAmount 
            * @param copyCardNumber 
            */
            CreditCard(double copyAmount, std::string copyCardNumber) :
                Payment::PaymentMethod(copyAmount),
                cardNumber(copyCardNumber) {}
            
            void processPayment(void)const override;
    };
}

#endif