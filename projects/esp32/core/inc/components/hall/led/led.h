#ifndef INC_HALL_LED_H_
#define INC_HALL_LED_H_

#include "mcal/mcal_dfs.h"

namespace led_module {
    class led_t {
        public:
            led_t(gpio_module::cfg_t &cfg_);
            ~led_t() = default;
            esp_err_t write(gpio_module::cfg::Status status);
        private:
            gpio_module::gpio_t cfg;
            const char* LED_TAG;
    };
}

#endif