#ifndef BTS7960_POSITION_H_
#define BTS7960_POSITION_H_

#include "common.h"

// Forward declaration
class BTS7960;

extern const char *BTS7960_POS_TAG;

class BTS7960_Position {
public:
    /**
     * @brief Construct a new BTS7960 Position Control object
     * 
     * @param motor Pointer to BTS7960 motor object
     * @param encoder_a Encoder channel A pin
     * @param encoder_b Encoder channel B pin
     * @param ppr Pulses per revolution of encoder
     * @param gear_ratio Gear ratio (output/input)
     */
    BTS7960_Position(BTS7960* motor, 
                     gpio_num_t encoder_a, 
                     gpio_num_t encoder_b,
                     uint16_t ppr = 360,
                     float gear_ratio = 1.0f);

    /**
     * @brief Set target angle
     * 
     * @param angle Target angle in degrees
     */
    void setAngle(float angle);

    /**
     * @brief Get current angle
     * 
     * @return float Current angle in degrees
     */
    float getAngle();

    /**
     * @brief Get target angle
     * 
     * @return float Target angle in degrees
     */
    float getTargetAngle();

    /**
     * @brief Reset encoder position to zero
     */
    void resetPosition();

    /**
     * @brief Update PID control loop (call in task)
     */
    void update();

    /**
     * @brief Set PID gains
     * 
     * @param kp Proportional gain
     * @param ki Integral gain
     * @param kd Derivative gain
     */
    void setPID(float kp, float ki, float kd);

    /**
     * @brief Check if motor reached target
     * 
     * @return true if within tolerance
     */
    bool isAtTarget();

    ~BTS7960_Position() = default;

private:
    BTS7960* motor_driver;

    // Encoder pins
    gpio_num_t enc_a;
    gpio_num_t enc_b;

    // Encoder specs
    uint16_t pulses_per_rev;
    float gear_ratio;

    // Position tracking
    volatile int32_t encoder_count;
    float current_angle;
    float target_angle;

    // PID variables
    float kp, ki, kd;
    float prev_error;
    float integral;

    // Control parameters
    float angle_tolerance;
    float max_speed;
    float min_speed;

    // Encoder interrupt handlers
    static void IRAM_ATTR encoder_isr_handler(void* arg);
    void IRAM_ATTR handleEncoderInterrupt();
    
    void init();
    float calculatePID(float error);
};

#endif // BTS7960_POSITION_H_