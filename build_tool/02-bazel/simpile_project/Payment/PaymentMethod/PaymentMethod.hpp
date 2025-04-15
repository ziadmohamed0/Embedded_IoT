/**
 * @file PaymentMethod.hpp
 * @author Ziad Mohammed fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#ifndef _PAYMENT_PAYMENT_METHOD_HPP_
#define _PAYMENT_PAYMENT_METHOD_HPP_
#include <iostream>

namespace Payment{
    class PaymentMethod {
        protected:
            double amount{0};
        public:
            PaymentMethod(double copyAmount) : amount{copyAmount} {}
            void set_amount(double copyNewAmount);
            [[nodiscard]] int getter_amount(void) {return amount;}
            virtual void processPayment(void) const;
    };
}
#endif