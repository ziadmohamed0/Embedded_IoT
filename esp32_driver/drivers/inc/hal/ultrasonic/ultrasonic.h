#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "common.h"

class ultrasonic {
    public:
        ultrasonic(gpio_num_t trig, gpio_num_t echo_);
        float readDistance();
    private:
        void pulseTrigger();
        gpio_num_t trigger;
        gpio_num_t echo;
};

#endif