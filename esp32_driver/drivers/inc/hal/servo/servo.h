#ifndef SERVO_H_
#define SERVO_H_

#include "common.h"

extern const char *SERVO_TAG;

enum class SERVO_TYPE : uint8_t {
    STANDARD_180,      // Standard servo (0-180 degrees)
    CONTINUOUS_360     // Continuous rotation servo (360 degrees)
};

enum class ROTATION_DIRECTION : uint8_t {
    CLOCKWISE,         // CW rotation
    COUNTER_CLOCKWISE, // CCW rotation
    STOP               // Stop rotation
};

class Servo {
public:
    /**
     * @brief Construct a new Servo object
     * 
     * @param pin_number GPIO pin for servo signal
     * @param type Servo type (STANDARD_180 or CONTINUOUS_360)
     * @param channel LEDC channel (0-7)
     * @param timer LEDC timer (0-3)
     */
    Servo(gpio_num_t pin_number, SERVO_TYPE type = SERVO_TYPE::STANDARD_180, 
          ledc_channel_t channel = LEDC_CHANNEL_0, ledc_timer_t timer = LEDC_TIMER_0);
    
    // ===== Standard Servo Functions (180 degree) =====
    /**
     * @brief Set servo angle (for standard 180° servo only)
     * 
     * @param angle Angle in degrees (0-180)
     */
    void setAngle(float angle);
    
    /**
     * @brief Get current angle (for standard 180° servo only)
     * 
     * @return float Current angle in degrees
     */
    float getAngle();
    
    // ===== Continuous Servo Functions (360 degree) =====
    /**
     * @brief Set rotation speed and direction (for 360° servo only)
     * 
     * @param speed Speed percentage (0-100)
     * @param direction Rotation direction (CW, CCW, STOP)
     */
    void setSpeed(float speed, ROTATION_DIRECTION direction);
    
    /**
     * @brief Rotate clockwise at specified speed (for 360° servo only)
     * 
     * @param speed Speed percentage (0-100)
     */
    void rotateCW(float speed);
    
    /**
     * @brief Rotate counter-clockwise at specified speed (for 360° servo only)
     * 
     * @param speed Speed percentage (0-100)
     */
    void rotateCCW(float speed);
    
    /**
     * @brief Stop rotation (for 360° servo only)
     */
    void stop();
    
    // ===== Common Functions =====
    /**
     * @brief Set servo pulse width in microseconds
     * 
     * @param pulse_us Pulse width (typically 500-2500 us)
     */
    void setPulse(uint32_t pulse_us);
    
    /**
     * @brief Detach servo (stop PWM)
     */
    void detach();
    
    /**
     * @brief Attach servo (restart PWM)
     */
    void attach();
    
    /**
     * @brief Get servo type
     * 
     * @return SERVO_TYPE Current servo type
     */
    SERVO_TYPE getType();
    
    ~Servo() = default;

private:
    gpio_num_t pin;
    ledc_channel_t pwm_channel;
    ledc_timer_t pwm_timer;
    SERVO_TYPE servo_type;
    float current_angle;        // For 180° servo
    float current_speed;        // For 360° servo
    ROTATION_DIRECTION current_direction;
    uint32_t min_pulse_us;      // Minimum pulse width (default 500us)
    uint32_t max_pulse_us;      // Maximum pulse width (default 2500us)
    uint32_t center_pulse_us;   // Center/stop pulse (default 1500us)
    bool is_attached;
    
    esp_err_t init();
    uint32_t angleToDuty(float angle);
    uint32_t speedToDuty(float speed, ROTATION_DIRECTION direction);
};

#endif