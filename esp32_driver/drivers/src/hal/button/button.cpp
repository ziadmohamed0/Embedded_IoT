#include "button.h"

const char *BTN_TAG = "BUTTON_DRIVER";


Button::Button(gpio_num_t pin_number, gpio_pull_mode_t pull_mode) : 
                pin(pin_number),
                pull(pull_mode) {
    gpio_reset_pin(this->pin);
    gpio_set_direction(this->pin, GPIO_MODE_INPUT);
    switch(this->pull) {
    case GPIO_PULLUP_ONLY:
        gpio_set_pull_mode(this->pin, GPIO_PULLUP_ONLY);
        ESP_LOGI(BTN_TAG, "Button GPIO%d configure as input with pullup", this->pin);
        break;
    case GPIO_PULLDOWN_ONLY:
        gpio_set_pull_mode(this->pin, GPIO_PULLDOWN_ONLY);
        ESP_LOGI(BTN_TAG, "Button GPIO%d configure as input with pulldown", this->pin);
        break;
    case GPIO_PULLUP_PULLDOWN:
        gpio_set_pull_mode(this->pin, GPIO_PULLUP_PULLDOWN);
        ESP_LOGI(BTN_TAG, "Button GPIO%d configure as input with pullup pulldown", this->pin);
        break;
    case GPIO_FLOATING:
        gpio_set_pull_mode(this->pin, GPIO_FLOATING);
        ESP_LOGI(BTN_TAG, "Button GPIO%d configure as input with floating", this->pin);
        break;
    default:
        gpio_set_pull_mode(this->pin, GPIO_PULLUP_ONLY);
        ESP_LOGI(BTN_TAG, "Button GPIO%d configure as input with pullup", this->pin);
        break;
    }
}


bool Button::GET() {
    bool status = gpio_get_level(this->pin);
    return status;
}