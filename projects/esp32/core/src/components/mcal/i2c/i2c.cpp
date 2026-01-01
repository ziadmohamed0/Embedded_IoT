#include "i2c/i2c.h"

namespace i2c_module {
    i2c_t::i2c_t(cfg_t &cfg) {
        i2c_master_bus_config_t bus_config = {};
        bus_config.i2c_port = cfg.port;
        bus_config.sda_io_num = cfg.sda_pin;
        bus_config.scl_io_num = cfg.scl_pin;
        bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
        bus_config.glitch_ignore_cnt = 7;
        bus_config.flags.enable_internal_pullup = true;

        ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));
        ESP_LOGI(TAG, "I2C Master Bus initialized on SDA:%d, SCL:%d", cfg.sda_pin, cfg.scl_pin);
    }

    i2c_t::~i2c_t() {
        i2c_del_master_bus(bus_handle);
    }
}