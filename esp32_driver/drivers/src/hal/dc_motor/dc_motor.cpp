#include "dc_motor.h"

const char *DC_MOTOR_TAG = "DC_MOTOR_DRIVER";

dc_motor::dc_motor(std::vector<gpio_num_t> p) : pins(p) {
    for(auto &motorPins : this->pins) {
        gpio_reset_pin(motorPins);
        gpio_set_direction(motorPins, GPIO_MODE_OUTPUT);
        ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is output", motorPins);
    }
}

void dc_motor::dc_move(DC_MOTOR_DIRECTION movement) {
    switch(movement) {
        case DC_MOTOR_DIRECTION::forward:{
            for(int index = 0; index < this->pins.size(); index++) {
                if(index%2 == false) {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::High));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is high.", this->pins[index]);
                }
                else {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
                }
            }
            ESP_LOGI(DC_MOTOR_TAG, "motor is forward.");
            break;
        }

        case DC_MOTOR_DIRECTION::backward:{
            for(int index = 0; index < this->pins.size(); index++) {
                if(index%2 == false) {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
                }
                else {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::High));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is high.", this->pins[index]);
                }
            }
            ESP_LOGI(DC_MOTOR_TAG, "motor is backward.");
            break;
        }

        case DC_MOTOR_DIRECTION::right:{
            for(int index = (this->pins.size() / 2); index < this->pins.size(); index++) {
                if(index%2 == false) {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::High));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is high.", this->pins[index]);
                }
                else {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
                }
            }
            for(int index = 0; index < (this->pins.size() / 2); index++) {
                if(index%2 == false) {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
                }
                else {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::High));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is high.", this->pins[index]);
                }
            }
            ESP_LOGI(DC_MOTOR_TAG, "motor is right.");
            break;
        }

        case DC_MOTOR_DIRECTION::left: {
            for(int index = 0; index < (this->pins.size() / 2); index++) {
                if(index%2 == false) {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::High));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is high.", this->pins[index]);
                }
                else {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
                }
            }
            for(int index = (this->pins.size() / 2); index < this->pins.size(); index++) {
                if(index%2 == false) {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
                }
                else {
                    gpio_set_level(this->pins[index], static_cast<uint32_t>(state::High));
                    ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is high.", this->pins[index]);
                }
            }
            ESP_LOGI(DC_MOTOR_TAG, "motor is left.");
            break;
        }

        case DC_MOTOR_DIRECTION::stop:{
            for(int index = 0; index < this->pins.size(); index++) {
                gpio_set_level(this->pins[index], static_cast<uint32_t>(state::Low));
                ESP_LOGI(DC_MOTOR_TAG, "GPIO%d is low.", this->pins[index]);
            }            
            ESP_LOGI(DC_MOTOR_TAG, "motor is stoped.");
            break;
        }
    }
}
