#ifndef LED_H_
#define LED_H_

#include "common.h"

extern const char *LED_TAG; 

class Led {
    public: 
        Led(gpio_num_t pin_number);
        void ON();
        void OFF();
        void TOGGLE(uint32_t toggle_time);
        ~Led() = default;
    private:
        gpio_num_t pin;
};

#endif