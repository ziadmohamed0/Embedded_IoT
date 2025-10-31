#include "bts7960_position.h"
#include "bts7960.h"  // Include بعد ما عملنا forward declaration

const char *BTS7960_POS_TAG = "BTS7960_POSITION";

BTS7960_Position::BTS7960_Position(BTS7960* motor, 
                                   gpio_num_t encoder_a, 
                                   gpio_num_t encoder_b,
                                   uint16_t ppr,
                                   float gear_ratio)
    : motor_driver(motor),
      enc_a(encoder_a),
      enc_b(encoder_b),
      pulses_per_rev(ppr),
      gear_ratio(gear_ratio),
      encoder_count(0),
      current_angle(0),
      target_angle(0),
      kp(2.0f),
      ki(0.1f),
      kd(0.5f),
      prev_error(0),
      integral(0),
      angle_tolerance(2.0f),
      max_speed(80.0f),
      min_speed(15.0f) {
    this->init();
}

void BTS7960_Position::init() {
    // Configure encoder pins
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << this->enc_a) | (1ULL << this->enc_b);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    
    // Install ISR service
    gpio_install_isr_service(0);
    
    // Add ISR handler for encoder A
    gpio_isr_handler_add(this->enc_a, encoder_isr_handler, (void*)this);
    
    ESP_LOGI(BTS7960_POS_TAG, "Position control initialized");
    ESP_LOGI(BTS7960_POS_TAG, "PPR: %d, Gear Ratio: %.2f", 
             this->pulses_per_rev, this->gear_ratio);
}

void IRAM_ATTR BTS7960_Position::encoder_isr_handler(void* arg) {
    BTS7960_Position* instance = (BTS7960_Position*)arg;
    instance->handleEncoderInterrupt();
}

void IRAM_ATTR BTS7960_Position::handleEncoderInterrupt() {
    int a_state = gpio_get_level(this->enc_a);
    int b_state = gpio_get_level(this->enc_b);
    
    // Quadrature decoding
    if (a_state == b_state) {
        this->encoder_count++;
    } else {
        this->encoder_count--;
    }
}

float BTS7960_Position::getAngle() {
    // Convert encoder pulses to degrees
    float total_pulses = this->pulses_per_rev * this->gear_ratio;
    this->current_angle = (this->encoder_count * 360.0f) / total_pulses;
    return this->current_angle;
}

void BTS7960_Position::setAngle(float angle) {
    this->target_angle = angle;
    this->integral = 0;  // Reset integral on new target
    ESP_LOGI(BTS7960_POS_TAG, "New target: %.2f degrees", angle);
}

float BTS7960_Position::getTargetAngle() {
    return this->target_angle;
}

void BTS7960_Position::resetPosition() {
    this->encoder_count = 0;
    this->current_angle = 0;
    this->target_angle = 0;
    this->integral = 0;
    this->prev_error = 0;
    ESP_LOGI(BTS7960_POS_TAG, "Position reset to zero");
}

float BTS7960_Position::calculatePID(float error) {
    // Proportional
    float p_term = this->kp * error;
    
    // Integral
    this->integral += error;
    // Anti-windup
    if (this->integral > 100) this->integral = 100;
    if (this->integral < -100) this->integral = -100;
    float i_term = this->ki * this->integral;
    
    // Derivative
    float derivative = error - this->prev_error;
    float d_term = this->kd * derivative;
    this->prev_error = error;
    
    // PID output
    float output = p_term + i_term + d_term;
    
    return output;
}

void BTS7960_Position::update() {
    float current = this->getAngle();
    float error = this->target_angle - current;
    
    // Check if at target
    if (fabs(error) < this->angle_tolerance) {
        this->motor_driver->stop();
        return;
    }
    
    // Calculate PID output
    float pid_output = this->calculatePID(error);
    
    // Convert to motor speed and direction
    float speed = fabs(pid_output);
    
    // Limit speed
    if (speed > this->max_speed) speed = this->max_speed;
    if (speed < this->min_speed && fabs(error) > this->angle_tolerance) {
        speed = this->min_speed;
    }
    
    // Apply to motor
    if (pid_output > 0) {
        this->motor_driver->forward(speed);
    } else if (pid_output < 0) {
        this->motor_driver->backward(speed);
    } else {
        this->motor_driver->stop();
    }
}

void BTS7960_Position::setPID(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    ESP_LOGI(BTS7960_POS_TAG, "PID updated: Kp=%.2f, Ki=%.2f, Kd=%.2f", 
             kp, ki, kd);
}

bool BTS7960_Position::isAtTarget() {
    float error = fabs(this->target_angle - this->getAngle());
    return (error < this->angle_tolerance);
}