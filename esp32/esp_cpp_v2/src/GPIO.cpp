/**
 * @file GPIO.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-13
 */

#include "GPIO.h"

namespace GPIO{
    [[nodiscard]] esp_err_t GpioBase::init(void) {
        esp_err_t status{ESP_OK};
        status |= gpio_config(&_cfg);
        return status;
    }

    [[nodiscard]] esp_err_t GpioOutput::init(void) {
        esp_err_t status{GpioBase::init()};
        if(ESP_OK == status) {
            status |= set(_inverted_logic);
        }
        return status;
    }

    esp_err_t GpioOutput::set(const bool copyState) {
        GpioOutput::_state = copyState;
        return gpio_set_level(GpioBase::_pinNumber, GpioOutput::_inverted_logic ? !copyState : copyState);
    }
}