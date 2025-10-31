#include "led.h"

const char *LED_TAG = "LED_DRIVER";

Led::Led(gpio_num_t pin_number) 
            : pin(pin_number) {
    gpio_reset_pin(this->pin);
    gpio_set_direction(this->pin, GPIO_MODE_OUTPUT);

    ESP_LOGI(LED_TAG, "Led GPIO%d configure as Output", this->pin);
}

void Led::ON() {
    gpio_set_level(this->pin, static_cast<uint32_t>(state::High));
    ESP_LOGI(LED_TAG, "Led is ON");
}

void Led::OFF() {
    gpio_set_level(this->pin, static_cast<uint32_t>(state::Low));
    ESP_LOGI(LED_TAG, "Led is OFF");
}

void Led::TOGGLE(uint32_t toggle_time) {
    gpio_set_level(this->pin, static_cast<uint32_t>(state::High));
    vTaskDelay(pdMS_TO_TICKS(toggle_time));
    gpio_set_level(this->pin, static_cast<uint32_t>(state::Low));
    vTaskDelay(pdMS_TO_TICKS(toggle_time));
    ESP_LOGI(LED_TAG, "Led is TOGGLE");
}