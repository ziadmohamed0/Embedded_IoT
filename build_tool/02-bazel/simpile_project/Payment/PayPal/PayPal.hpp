/**
 * @file PayPal.hpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */


#include "../PaymentMethod/PaymentMethod.hpp"

namespace PayPal {
    class PayPal : public Payment::PaymentMethod {
        private: 
            std::string email;
        public:
            /**
             * @brief Construct a new Pay Pal object
             * 
             * @param copyAmount 
             * @param copyEmail 
             */
            PayPal(double copyAmount, std::string copyEmail) : 
                Payment::PaymentMethod(copyAmount),
                email(copyEmail) {

                }

            void processPayment(void) const override;
    };
}