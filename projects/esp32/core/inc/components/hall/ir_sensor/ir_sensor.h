#ifndef INC_HALL_IR_SENSOR_H_
#define INC_HALL_IR_SENSOR_H_

#include "mcal/mcal_dfs.h"

namespace sensor_module {
    class ir_sensor_t {
        public:
            ir_sensor_t(gpio_module::cfg_t &cfg_);
            ~ir_sensor_t() = default;
            bool read();
        private:
            gpio_module::gpio_t cfg;
            const char* IR_SENSOR_TAG;
    };
}

#endif