#ifndef INC_HALL_LED_H_
#define INC_HALL_LED_H_

#include "mcal/mcal_dfs.h"

namespace relay_module {
    class relay_t {
        public:
            relay_t(gpio_module::cfg_t &cfg_);
            ~relay_t() = default;
            esp_err_t write(gpio_module::cfg::Status status);
        private:
            gpio_module::gpio_t cfg;
            const char* RELAY_TAG;
    };
}

#endif