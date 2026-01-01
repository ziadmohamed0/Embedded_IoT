#ifndef INC_I2C_MASTER_H_
#define INC_I2C_MASTER_H_

#include "common.h"

namespace i2c_module {

    struct cfg_t {
        i2c_port_t port;
        gpio_num_t sda_pin;
        gpio_num_t scl_pin;
        uint32_t clk_speed;
    };

    class i2c_t {
    public:
        i2c_t(cfg_t &cfg);
        ~i2c_t();

        i2c_master_bus_handle_t get_bus_handle() { return bus_handle; }

    private:
        i2c_master_bus_handle_t bus_handle;
        static constexpr const char* TAG = "I2C_MASTER";
    };
}

#endif