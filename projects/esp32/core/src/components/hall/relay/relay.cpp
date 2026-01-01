#include "relay/relay.h"


namespace relay_module {
    relay_t::relay_t(gpio_module::cfg_t &cfg_) : 
                        cfg(cfg_), 
                        RELAY_TAG("RELAY_MODULE") {}

    esp_err_t relay_t::write(gpio_module::cfg::Status status) {
       esp_err_t err = this->cfg.write(status);
       return err;
    }
}