#include "ir_sensor/ir_sensor.h"

namespace sensor_module {
    ir_sensor_t::ir_sensor_t(gpio_module::cfg_t &cfg_) : 
                                cfg(cfg_), 
                                IR_SENSOR_TAG("IR_SENSOR_MODULE") {}

    bool ir_sensor_t::read() {
        return this->cfg.get();
    }
}