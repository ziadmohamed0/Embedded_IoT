#include "led/led.h"


namespace led_module {
    led_t::led_t(gpio_module::cfg_t &cfg_) : 
                        cfg(cfg_), 
                        LED_TAG("LED_MODULE") {}

    esp_err_t led_t::write(gpio_module::cfg::Status status) {
       esp_err_t err = this->cfg.write(status);
       return err;
    }
}