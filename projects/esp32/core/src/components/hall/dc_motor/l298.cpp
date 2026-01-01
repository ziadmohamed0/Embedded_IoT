#include "dc_motor/l298.h"

namespace motor_module {

    l298_motor_t::l298_motor_t(pwm_module::pwm_cfg_t &pwm_cfg, 
                               gpio_module::cfg_t &in1_cfg, 
                               gpio_module::cfg_t &in2_cfg) : 
                               speed_ctrl(pwm_cfg), 
                               pin_in1(in1_cfg), 
                               pin_in2(in2_cfg),
                               MOTOR_TAG("L298_MOTOR") {}

    void l298_motor_t::drive(Direction dir, uint32_t speed) {
        switch (dir) {
            case Direction::FORWARD:
                pin_in1.write(gpio_module::cfg::Status::High);
                pin_in2.write(gpio_module::cfg::Status::Low);
                break;
            case Direction::BACKWARD:
                pin_in1.write(gpio_module::cfg::Status::Low);
                pin_in2.write(gpio_module::cfg::Status::High);
                break;
            case Direction::STOP:
                pin_in1.write(gpio_module::cfg::Status::Low);
                pin_in2.write(gpio_module::cfg::Status::Low);
                speed = 0;
                break;
        }
        speed_ctrl.set_duty(speed);
    }

    void l298_motor_t::stop() {
        this->drive(Direction::STOP, 0);
    }
}