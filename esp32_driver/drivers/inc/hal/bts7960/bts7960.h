#ifndef BTS7960_H_
#define BTS7960_H_

#include "common.h"

extern const char *BTS7960_TAG;

enum class MOTOR_DIRECTION : uint8_t {
    FORWARD,
    BACKWARD,
    BRAKE,
    STOP
};

class BTS7960 {
public:
    /**
     * @brief Construct a new BTS7960 object
     * 
     * @param r_en Right enable pin (active high)
     * @param l_en Left enable pin (active high)
     * @param r_pwm Right PWM pin (forward)
     * @param l_pwm Left PWM pin (backward)
     * @param pwm_channel_r PWM channel for forward
     * @param pwm_channel_l PWM channel for backward
     * @param pwm_timer PWM timer
     */
    BTS7960(gpio_num_t r_en, gpio_num_t l_en,
            gpio_num_t r_pwm, gpio_num_t l_pwm,
            ledc_channel_t pwm_channel_r = LEDC_CHANNEL_0,
            ledc_channel_t pwm_channel_l = LEDC_CHANNEL_1,
            ledc_timer_t pwm_timer = LEDC_TIMER_0);

    /**
     * @brief Set motor speed and direction
     * 
     * @param speed Speed percentage (0-100)
     * @param direction Motor direction (FORWARD, BACKWARD, BRAKE, STOP)
     */
    void setSpeed(float speed, MOTOR_DIRECTION direction);

    /**
     * @brief Move forward at specified speed
     * 
     * @param speed Speed percentage (0-100)
     */
    void forward(float speed);

    /**
     * @brief Move backward at specified speed
     * 
     * @param speed Speed percentage (0-100)
     */
    void backward(float speed);

    /**
     * @brief Stop motor (coast)
     */
    void stop();

    /**
     * @brief Brake motor (active braking)
     */
    void brake();

    /**
     * @brief Enable motor driver
     */
    void enable();

    /**
     * @brief Disable motor driver
     */
    void disable();

    ~BTS7960() = default;

private:
    gpio_num_t R_EN;      // Right enable
    gpio_num_t L_EN;      // Left enable
    gpio_num_t RPWM;      // Right PWM (forward)
    gpio_num_t LPWM;      // Left PWM (backward)
    
    ledc_channel_t pwm_channel_forward;
    ledc_channel_t pwm_channel_backward;
    ledc_timer_t timer;
    
    bool is_enabled;

    void init();
    uint32_t speedToDuty(float speed);
};

#endif // BTS7960_H_