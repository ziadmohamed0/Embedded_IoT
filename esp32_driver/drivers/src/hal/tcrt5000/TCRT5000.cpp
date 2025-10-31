#include "TCRT5000.h"

tcrt_sensor::tcrt_sensor(gpio_num_t d_pin, adc1_channel_t a_pin) : 
                            out_pin(d_pin), 
                            adc_channel(a_pin) {
    gpio_set_direction(this->out_pin, GPIO_MODE_INPUT);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(this->adc_channel, ADC_ATTEN_DB_11);
}

bool tcrt_sensor::readDigital() {
    return gpio_get_level(this->out_pin);
}

float tcrt_sensor::readAnaloge() {
    return adc1_get_raw(this->adc_channel);
}