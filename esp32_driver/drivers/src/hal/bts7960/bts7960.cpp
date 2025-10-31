#include "bts7960.h"

const char *BTS7960_TAG = "BTS7960_DRIVER";

BTS7960::BTS7960(gpio_num_t r_en, gpio_num_t l_en,
                 gpio_num_t r_pwm, gpio_num_t l_pwm,
                 ledc_channel_t pwm_channel_r,
                 ledc_channel_t pwm_channel_l,
                 ledc_timer_t pwm_timer)
    : R_EN(r_en), L_EN(l_en),
      RPWM(r_pwm), LPWM(l_pwm),
      pwm_channel_forward(pwm_channel_r),
      pwm_channel_backward(pwm_channel_l),
      timer(pwm_timer),
      is_enabled(false) {
    this->init();
}

void BTS7960::init() {
    // Configure enable pins as outputs
    gpio_reset_pin(this->R_EN);
    gpio_reset_pin(this->L_EN);
    gpio_set_direction(this->R_EN, GPIO_MODE_OUTPUT);
    gpio_set_direction(this->L_EN, GPIO_MODE_OUTPUT);
    
    // Initially disabled
    gpio_set_level(this->R_EN, 0);
    gpio_set_level(this->L_EN, 0);
    
    // Configure LEDC timer
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = this->timer,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    esp_err_t ret = ledc_timer_config(&timer_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(BTS7960_TAG, "Failed to configure LEDC timer");
        return;
    }
    
    // Configure forward PWM channel (RPWM)
    ledc_channel_config_t channel_conf_forward = {
        .gpio_num = this->RPWM,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = this->pwm_channel_forward,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = this->timer,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {
            .output_invert = 0
        }
    };
    ret = ledc_channel_config(&channel_conf_forward);
    if (ret != ESP_OK) {
        ESP_LOGE(BTS7960_TAG, "Failed to configure forward PWM channel");
        return;
    }
    
    // Configure backward PWM channel (LPWM)
    ledc_channel_config_t channel_conf_backward = {
        .gpio_num = this->LPWM,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = this->pwm_channel_backward,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = this->timer,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {
            .output_invert = 0
        }
    };
    ret = ledc_channel_config(&channel_conf_backward);
    if (ret != ESP_OK) {
        ESP_LOGE(BTS7960_TAG, "Failed to configure backward PWM channel");
        return;
    }
    
    ESP_LOGI(BTS7960_TAG, "BTS7960 initialized on RPWM: GPIO%d, LPWM: GPIO%d", 
             this->RPWM, this->LPWM);
}

uint32_t BTS7960::speedToDuty(float speed) {
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    
    // Convert percentage to 10-bit duty cycle (0-1023)
    uint32_t duty = (uint32_t)((speed / 100.0f) * 1023);
    return duty;
}

void BTS7960::setSpeed(float speed, MOTOR_DIRECTION direction) {
    if (!this->is_enabled) {
        ESP_LOGW(BTS7960_TAG, "Motor driver is disabled. Enable first.");
        return;
    }
    
    uint32_t duty = this->speedToDuty(speed);
    
    switch(direction) {
        case MOTOR_DIRECTION::FORWARD:
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward);
            
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward);
            
            ESP_LOGI(BTS7960_TAG, "Forward at %.1f%% speed", speed);
            break;
            
        case MOTOR_DIRECTION::BACKWARD:
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward);
            
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward);
            
            ESP_LOGI(BTS7960_TAG, "Backward at %.1f%% speed", speed);
            break;
            
        case MOTOR_DIRECTION::BRAKE:
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward, 1023);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward);
            
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward, 1023);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward);
            
            ESP_LOGI(BTS7960_TAG, "Brake engaged");
            break;
            
        case MOTOR_DIRECTION::STOP:
        default:
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_forward);
            
            ledc_set_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, this->pwm_channel_backward);
            
            ESP_LOGI(BTS7960_TAG, "Motor stopped");
            break;
    }
}

void BTS7960::forward(float speed) {
    this->setSpeed(speed, MOTOR_DIRECTION::FORWARD);
}

void BTS7960::backward(float speed) {
    this->setSpeed(speed, MOTOR_DIRECTION::BACKWARD);
}

void BTS7960::stop() {
    this->setSpeed(0, MOTOR_DIRECTION::STOP);
}

void BTS7960::brake() {
    this->setSpeed(0, MOTOR_DIRECTION::BRAKE);
}

void BTS7960::enable() {
    gpio_set_level(this->R_EN, 1);
    gpio_set_level(this->L_EN, 1);
    this->is_enabled = true;
    ESP_LOGI(BTS7960_TAG, "Motor driver enabled");
}

void BTS7960::disable() {
    this->stop();
    gpio_set_level(this->R_EN, 0);
    gpio_set_level(this->L_EN, 0);
    this->is_enabled = false;
    ESP_LOGI(BTS7960_TAG, "Motor driver disabled");
}