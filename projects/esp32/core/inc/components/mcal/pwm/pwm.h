#ifndef INC_PWM_MODULE_H_
#define INC_PWM_MODULE_H_

#include "common.h"

namespace pwm_module {
    namespace cfg {
        enum class Timer_t : uint8_t { _0 = LEDC_TIMER_0, _1 = LEDC_TIMER_1 };
        enum class Channel_t : uint8_t { _0 = LEDC_CHANNEL_0, _1 = LEDC_CHANNEL_1 };
        enum class Mode_t : uint8_t { LowSpeed = LEDC_LOW_SPEED_MODE };
    }

    struct pwm_cfg_t {
        gpio_num_t pin;
        uint32_t freq;
        ledc_timer_bit_t resolution;
        pwm_module::cfg::Timer_t timer_sel;
        pwm_module::cfg::Channel_t channel_sel;
    };

    class pwm_t {
    public:
        pwm_t(pwm_cfg_t copy_cfg);
        ~pwm_t() = default;

        esp_err_t set_duty(uint32_t duty);
        esp_err_t set_frequency(uint32_t freq);
        void stop();

    private:
        pwm_cfg_t cfg;
        const char* PWM_TAG;
        esp_err_t init_timer();
        esp_err_t init_channel();
    };
}

#endif