#include "servo.h"

const char *SERVO_TAG = "SERVO_DRIVER";

Servo::Servo(gpio_num_t pin_number, SERVO_TYPE type, ledc_channel_t channel, ledc_timer_t timer)
    : pin(pin_number),
      pwm_channel(channel),
      pwm_timer(timer),
      servo_type(type),
      current_angle(90.0f),
      current_speed(0.0f),
      current_direction(ROTATION_DIRECTION::STOP),
      min_pulse_us(500),
      max_pulse_us(2500),
      center_pulse_us(1500),
      is_attached(false) {
    this->init();
}

esp_err_t Servo::init() {
    // Configure LEDC timer
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,  // 14-bit resolution
        .timer_num = this->pwm_timer,
        .freq_hz = 50,  // 50Hz for servo (20ms period)
        .clk_cfg = LEDC_AUTO_CLK
    };
    esp_err_t ret = ledc_timer_config(&timer_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(SERVO_TAG, "Failed to configure LEDC timer");
        return ret;
    }

    // Configure LEDC channel
    ledc_channel_config_t channel_conf = {
        .gpio_num = this->pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = this->pwm_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = this->pwm_timer,
        .duty = 0,
        .hpoint = 0,
        .flags = {0}
    };
    ret = ledc_channel_config(&channel_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(SERVO_TAG, "Failed to configure LEDC channel");
        return ret;
    }

    this->is_attached = true;
    
    if (this->servo_type == SERVO_TYPE::STANDARD_180) {
        ESP_LOGI(SERVO_TAG, "Standard 180° Servo configured on GPIO%d", this->pin);
        this->setAngle(90.0f);  // Center position
    } else {
        ESP_LOGI(SERVO_TAG, "Continuous 360° Servo configured on GPIO%d", this->pin);
        this->stop();  // Stop rotation
    }
    
    return ESP_OK;
}

uint32_t Servo::angleToDuty(float angle) {
    // For standard 180° servo
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    
    uint32_t pulse_us = this->min_pulse_us + 
                        (angle / 180.0f) * (this->max_pulse_us - this->min_pulse_us);
    
    // Convert pulse width to duty cycle
    uint32_t duty = (pulse_us * 16384) / 20000;
    
    return duty;
}

uint32_t Servo::speedToDuty(float speed, ROTATION_DIRECTION direction) {
    // For continuous 360° servo
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    
    uint32_t pulse_us;
    
    switch(direction) {
        case ROTATION_DIRECTION::CLOCKWISE:
            // CW: 1500us (stop) to 2500us (max speed)
            pulse_us = this->center_pulse_us + 
                      (speed / 100.0f) * (this->max_pulse_us - this->center_pulse_us);
            break;
            
        case ROTATION_DIRECTION::COUNTER_CLOCKWISE:
            // CCW: 1500us (stop) to 500us (max speed)
            pulse_us = this->center_pulse_us - 
                      (speed / 100.0f) * (this->center_pulse_us - this->min_pulse_us);
            break;
            
        case ROTATION_DIRECTION::STOP:
        default:
            pulse_us = this->center_pulse_us;  // 1500us = stop
            break;
    }
    
    // Convert pulse width to duty cycle
    uint32_t duty = (pulse_us * 16384) / 20000;
    
    return duty;
}

// ===== Standard 180° Servo Functions =====

void Servo::setAngle(float angle) {
    if (!this->is_attached) {
        ESP_LOGW(SERVO_TAG, "Servo is detached. Attach first.");
        return;
    }
    
    if (this->servo_type != SERVO_TYPE::STANDARD_180) {
        ESP_LOGW(SERVO_TAG, "This function is for standard 180° servo only");
        return;
    }
    
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    
    this->current_angle = angle;
    uint32_t duty = this->angleToDuty(angle);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel);
    
    ESP_LOGI(SERVO_TAG, "Servo angle set to %.1f degrees", angle);
}

float Servo::getAngle() {
    if (this->servo_type != SERVO_TYPE::STANDARD_180) {
        ESP_LOGW(SERVO_TAG, "This function is for standard 180° servo only");
        return 0;
    }
    return this->current_angle;
}

// ===== Continuous 360° Servo Functions =====

void Servo::setSpeed(float speed, ROTATION_DIRECTION direction) {
    if (!this->is_attached) {
        ESP_LOGW(SERVO_TAG, "Servo is detached. Attach first.");
        return;
    }
    
    if (this->servo_type != SERVO_TYPE::CONTINUOUS_360) {
        ESP_LOGW(SERVO_TAG, "This function is for continuous 360° servo only");
        return;
    }
    
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    
    this->current_speed = speed;
    this->current_direction = direction;
    
    uint32_t duty = this->speedToDuty(speed, direction);
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel);
    
    const char* dir_str = (direction == ROTATION_DIRECTION::CLOCKWISE) ? "CW" : 
                          (direction == ROTATION_DIRECTION::COUNTER_CLOCKWISE) ? "CCW" : "STOP";
    ESP_LOGI(SERVO_TAG, "Servo speed set to %.1f%% %s", speed, dir_str);
}

void Servo::rotateCW(float speed) {
    this->setSpeed(speed, ROTATION_DIRECTION::CLOCKWISE);
}

void Servo::rotateCCW(float speed) {
    this->setSpeed(speed, ROTATION_DIRECTION::COUNTER_CLOCKWISE);
}

void Servo::stop() {
    this->setSpeed(0, ROTATION_DIRECTION::STOP);
}

// ===== Common Functions =====

void Servo::setPulse(uint32_t pulse_us) {
    if (!this->is_attached) {
        ESP_LOGW(SERVO_TAG, "Servo is detached. Attach first.");
        return;
    }
    
    if (pulse_us < 500) pulse_us = 500;
    if (pulse_us > 2500) pulse_us = 2500;
    
    // Update internal state based on servo type
    if (this->servo_type == SERVO_TYPE::STANDARD_180) {
        this->current_angle = ((float)(pulse_us - this->min_pulse_us) / 
                              (this->max_pulse_us - this->min_pulse_us)) * 180.0f;
    } else {
        // For 360° servo, calculate speed and direction from pulse
        if (pulse_us > this->center_pulse_us) {
            this->current_direction = ROTATION_DIRECTION::CLOCKWISE;
            this->current_speed = ((float)(pulse_us - this->center_pulse_us) / 
                                  (this->max_pulse_us - this->center_pulse_us)) * 100.0f;
        } else if (pulse_us < this->center_pulse_us) {
            this->current_direction = ROTATION_DIRECTION::COUNTER_CLOCKWISE;
            this->current_speed = ((float)(this->center_pulse_us - pulse_us) / 
                                  (this->center_pulse_us - this->min_pulse_us)) * 100.0f;
        } else {
            this->current_direction = ROTATION_DIRECTION::STOP;
            this->current_speed = 0;
        }
    }
    
    uint32_t duty = (pulse_us * 16384) / 20000;
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel);
    
    ESP_LOGI(SERVO_TAG, "Servo pulse set to %lu us", pulse_us);
}

void Servo::detach() {
    if (this->is_attached) {
        ledc_stop(LEDC_LOW_SPEED_MODE, this->pwm_channel, 0);
        this->is_attached = false;
        ESP_LOGI(SERVO_TAG, "Servo detached");
    }
}

void Servo::attach() {
    if (!this->is_attached) {
        this->is_attached = true;
        if (this->servo_type == SERVO_TYPE::STANDARD_180) {
            this->setAngle(this->current_angle);
        } else {
            this->setSpeed(this->current_speed, this->current_direction);
        }
        ESP_LOGI(SERVO_TAG, "Servo attached");
    }
}

SERVO_TYPE Servo::getType() {
    return this->servo_type;
}