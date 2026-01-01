#include "ultrasonic/ultrasonic.h"

namespace sensor_module {

    ultrasonic_t::ultrasonic_t(gpio_module::cfg_t &trig_cfg, gpio_module::cfg_t &echo_cfg) 
        : trig_pin(trig_cfg), echo_pin(echo_cfg) {
        ESP_LOGI(ULTRASONIC_TAG, "Ultrasonic sensor initialized successfully.");
    }

    float ultrasonic_t::get_distance_cm() {
        trig_pin.write(gpio_module::cfg::Status::Low);
        ets_delay_us(2);
        trig_pin.write(gpio_module::cfg::Status::High);
        ets_delay_us(10);
        trig_pin.write(gpio_module::cfg::Status::Low);

        uint64_t start_time = esp_timer_get_time();
        while (echo_pin.get() == false) {
            if (esp_timer_get_time() - start_time > TIMEOUT_US) {
                ESP_LOGW(ULTRASONIC_TAG, "Echo wait timeout! Check connections.");
                return -1.0;
            }
        }

        uint64_t echo_start = esp_timer_get_time();
        while (echo_pin.get() == true) {
            if (esp_timer_get_time() - echo_start > TIMEOUT_US) {
                ESP_LOGW(ULTRASONIC_TAG, "Pulse too long! Sensor out of range.");
                return -1.0;
            }
        }
        uint64_t duration = esp_timer_get_time() - echo_start;

        float distance = (duration * SOUND_SPEED) / 2.0;

        ESP_LOGD(ULTRASONIC_TAG, "Measured distance: %.2f cm", distance);

        return distance;
    }
}