#include "MPU6050.h"

MPU6050::MPU6050(i2c_port_t port_i2c, gpio_num_t sda_, gpio_num_t scl_) : 
                    port(port_i2c), 
                    sda(sda_),
                    scl(scl_) {
    this->init();
}

esp_err_t MPU6050::init() {
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = this->sda,
        .scl_io_num = this->scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {.clk_speed = 400000},
    };
    i2c_param_config(this->port, &cfg);
    i2c_driver_install(this->port, I2C_MODE_MASTER, 0, 0, 0);
    this->writeByte(0x6B, 0x00);
    this->writeByte(0x1B, 0x00); 
    this->writeByte(0x1C, 0x00); 
    return ESP_OK;
}

esp_err_t MPU6050::writeByte(uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t MPU6050::readBytes(uint8_t reg, uint8_t* buffer, size_t length) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, buffer, length, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t MPU6050::readAccel(float& ax, float& ay, float& az) {
    uint8_t data[6];
    if (readBytes(0x3B, data, 6) != ESP_OK)
        return ESP_FAIL;

    int16_t raw_ax = (data[0] << 8) | data[1];
    int16_t raw_ay = (data[2] << 8) | data[3];
    int16_t raw_az = (data[4] << 8) | data[5];

    ax = raw_ax / 16384.0f;
    ay = raw_ay / 16384.0f;
    az = raw_az / 16384.0f;
    return ESP_OK;
}

esp_err_t MPU6050::readGyro(float& gx, float& gy, float& gz) {
    uint8_t data[6];
    if (readBytes(0x43, data, 6) != ESP_OK)
        return ESP_FAIL;

    int16_t raw_gx = (data[0] << 8) | data[1];
    int16_t raw_gy = (data[2] << 8) | data[3];
    int16_t raw_gz = (data[4] << 8) | data[5];

    gx = raw_gx / 131.0f;
    gy = raw_gy / 131.0f;
    gz = raw_gz / 131.0f;
    return ESP_OK;
}

float MPU6050::readTemprature() {
    uint8_t data[2];
    if (readBytes(0x41, data, 2) != ESP_OK)
        return 0;

    int16_t raw_temp = (data[0] << 8) | data[1];
    return (raw_temp / 340.0f) + 36.53f;
}

void MPU6050::updateAngles(float dt) {
    float ax, ay, az, gx, gy, gz;

    readAccel(ax, ay, az);
    readGyro(gx, gy, gz);

    float roll_acc = atan2(ay, az) * 180 / M_PI;
    float pitch_acc = atan2(-ax, sqrt(ay*ay + az*az)) * 180 / M_PI;

    roll = alpha * (roll + gx * dt) + (1 - alpha) * roll_acc;
    pitch = alpha * (pitch + gy * dt) + (1 - alpha) * pitch_acc;

    yaw += gz * dt; 
}

float MPU6050::getRoll() {
    return this->roll;
}

float MPU6050::getPitch() {
    return this->pitch;
}

float MPU6050::getYaw() {
    return this->yaw;
}