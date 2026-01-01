#include "button/button.h"

namespace button_module {
    button_t::button_t(gpio_module::cfg_t &cfg_) : 
                        cfg(cfg_), 
                        BUTTON_TAG("BUTTON_MODULE") {}

    bool button_t::read() {
        return this->cfg.get();
    }
}