#ifndef INC_HALL_BUTTON_H_
#define INC_HALL_BUTTON_H_

#include "mcal/mcal_dfs.h"

namespace button_module {
    class button_t {
        public:
            button_t(gpio_module::cfg_t &cfg_);
            ~button_t() = default;
            bool read();
        private:
            gpio_module::gpio_t cfg;
            const char* BUTTON_TAG;
    };
}

#endif