#include "gpio/gpio.h"


namespace gpio_module{
    gpio_t::gpio_t(cfg_t copy_cfg) : 
                    GPIO_TAG("GPIO_MODULE"),
                    cfg(copy_cfg) {
        this->pin_number = static_cast<gpio_num_t>(this->cfg.pin);
        this->mode_type = static_cast<gpio_mode_t>(this->cfg.mode);
        this->pulling_mode  = static_cast<gpio_pull_mode_t>(this->cfg.pulling);

        if(!GPIO_IS_VALID_GPIO(this->pin_number)){
            ESP_LOGI(this->GPIO_TAG, "GPIO Number Erorr: pin %d is not valid\n", this->pin_number);
            return;
        }

        else {
            // 01. reset pin.
            if(gpio_reset_pin(this->pin_number) != ESP_OK) {
                ESP_LOGE(this->GPIO_TAG, "Failed to reset pin %d", this->pin_number);
                return;
            }
            //02. set pin direction (input, output).
            if(gpio_set_direction(this->pin_number, this->mode_type) != ESP_OK) {
                ESP_LOGE(this->GPIO_TAG, "Failed to set direction for pin %d", this->pin_number);
                return;
            }
            //03. set pulling mode
            if(gpio_set_pull_mode(this->pin_number, this->pulling_mode) != ESP_OK) {
                ESP_LOGE(this->GPIO_TAG, "Failed to set pulling for pin %d", this->pin_number);
                return;
            }
        }
    }

    esp_err_t gpio_t::write(cfg::Status copy_status) {
        uint32_t level = static_cast<uint32_t>(copy_status);
        if(gpio_set_level(this->pin_number, level) != ESP_OK){
            ESP_LOGE(this->GPIO_TAG, "Failed to set level for pin %d", this->pin_number);
            return ESP_FAIL;
        }
        return ESP_OK;
    }

    bool gpio_t::get() {
        int ret = gpio_get_level(this->pin_number);
        return (bool)ret;
    }
}
