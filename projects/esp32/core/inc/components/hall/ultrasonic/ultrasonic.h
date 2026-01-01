#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "mcal/mcal_dfs.h"

namespace sensor_module {

    class ultrasonic_t {
    public:
        ultrasonic_t(gpio_module::cfg_t &trig_cfg, gpio_module::cfg_t &echo_cfg);
        
        float get_distance_cm(); 

    private:
        gpio_module::gpio_t trig_pin;
        gpio_module::gpio_t echo_pin;
        static constexpr const char* ULTRASONIC_TAG = "ULTRASONIC_DRIVER";
        const float SOUND_SPEED = 0.0343; 
        const uint32_t TIMEOUT_US = 30000; 
    };
}

#endif