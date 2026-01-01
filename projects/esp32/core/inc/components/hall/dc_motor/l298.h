#ifndef INC_L298_DRIVER_H_
#define INC_L298_DRIVER_H_

#include "mcal/mcal_dfs.h"

namespace motor_module {
    
    enum class Direction {
        FORWARD,
        BACKWARD,
        STOP
    };

    class l298_motor_t {
    public:
        l298_motor_t(pwm_module::pwm_cfg_t &pwm_cfg, 
                     gpio_module::cfg_t &in1_cfg, 
                     gpio_module::cfg_t &in2_cfg);
        
        void drive(Direction dir, uint32_t speed);
        void stop();

    private:
        pwm_module::pwm_t speed_ctrl; 
        gpio_module::gpio_t pin_in1;    
        gpio_module::gpio_t pin_in2;    
        const char* MOTOR_TAG;
    };
}

#endif