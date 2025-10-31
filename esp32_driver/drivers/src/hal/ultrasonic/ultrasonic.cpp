#include "ultrasonic.h"

ultrasonic::ultrasonic(gpio_num_t trig, gpio_num_t echo_) : 
                        trigger(trig),
                        echo(echo_){
    gpio_reset_pin(this->trigger);
    gpio_reset_pin(this->echo);
    gpio_set_direction(this->trigger, GPIO_MODE_OUTPUT);
    gpio_set_direction(this->echo, GPIO_MODE_INPUT);
    gpio_set_level(this->trigger, static_cast<uint32_t>(state::Low));
}

float ultrasonic::readDistance() {
    const int samples = 3;
    float total = 0;

    for (int i = 0; i < samples; ++i) {
        this->pulseTrigger();

        int timeout = 30000;
        while (gpio_get_level(this->echo) == 0 && timeout--) {
            esp_rom_delay_us(1);
        }
        if (timeout <= 0) 
            return -1;

        uint64_t start_time = esp_timer_get_time();

        timeout = 30000;
        while (gpio_get_level(this->echo) == 1 && timeout--) {
            esp_rom_delay_us(1);
        }
        if (timeout <= 0) 
            return -1;

        uint64_t end_time = esp_timer_get_time();
        float duration = static_cast<float>(end_time - start_time);

        total += (duration * 0.0343f) / 2.0f;
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms delay between samples
    }

    return total / samples;
}

void ultrasonic::pulseTrigger() {
    gpio_set_level(this->trigger, static_cast<uint32_t>(state::Low));
    esp_rom_delay_us(2);
    gpio_set_level(this->trigger, static_cast<uint32_t>(state::High));
    esp_rom_delay_us(10);
    gpio_set_level(this->trigger, static_cast<uint32_t>(state::Low));
}
