#include "pwm/pwm.h"

namespace pwm_module {
    pwm_t::pwm_t(pwm_cfg_t copy_cfg) : 
                cfg(copy_cfg), 
                PWM_TAG("PWM_MODULE") {
        init_timer();
        init_channel();
    }

    esp_err_t pwm_t::init_timer() {
        ledc_timer_config_t ledc_timer = {}; 
        ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE;
        ledc_timer.duty_resolution  = cfg.resolution;
        ledc_timer.timer_num        = static_cast<ledc_timer_t>(cfg.timer_sel);
        ledc_timer.freq_hz          = cfg.freq;
        ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
        
        return ledc_timer_config(&ledc_timer);
    }

    esp_err_t pwm_t::init_channel() {
        ledc_channel_config_t ledc_channel = {};
        ledc_channel.gpio_num       = cfg.pin;
        ledc_channel.speed_mode     = LEDC_LOW_SPEED_MODE;
        ledc_channel.channel        = static_cast<ledc_channel_t>(cfg.channel_sel);
        ledc_channel.intr_type      = LEDC_INTR_DISABLE;
        ledc_channel.timer_sel      = static_cast<ledc_timer_t>(cfg.timer_sel);
        ledc_channel.duty           = 0;
        ledc_channel.hpoint         = 0;
        
        return ledc_channel_config(&ledc_channel);
    }

    esp_err_t pwm_t::set_duty(uint32_t duty) {
        esp_err_t ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(cfg.channel_sel), duty);
        if(ret == ESP_OK) {
            ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(cfg.channel_sel));
        }
        return ret;
    }

    esp_err_t pwm_t::set_frequency(uint32_t freq) {
        return ledc_set_freq(LEDC_LOW_SPEED_MODE, static_cast<ledc_timer_t>(cfg.timer_sel), freq);
    }

    void pwm_t::stop() {
        ledc_stop(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(cfg.channel_sel), 0);
    }
}