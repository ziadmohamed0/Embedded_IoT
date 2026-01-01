#include "mpu6050/mpu6050.h"

namespace sensor_module {

    mpu6050_t::mpu6050_t(i2c_module::i2c_t &bus, uint8_t dev_addr) {
        i2c_device_config_t dev_cfg = {};
        dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
        dev_cfg.device_address = dev_addr;
        dev_cfg.scl_speed_hz = 400000; 

        ESP_ERROR_CHECK(i2c_master_bus_add_device(bus.get_bus_handle(), &dev_cfg, &dev_handle));

        write_reg(0x6B, 0x00); 
        ESP_LOGI(TAG, "MPU6050 initialized and wake-up command sent.");
    }

    esp_err_t mpu6050_t::update() {
        uint8_t raw_buffer[14];
        esp_err_t ret = read_regs(0x3B, raw_buffer, 14);
        if (ret != ESP_OK) return ret;

        int16_t raw_ax = (raw_buffer[0] << 8) | raw_buffer[1];
        int16_t raw_ay = (raw_buffer[2] << 8) | raw_buffer[3];
        int16_t raw_az = (raw_buffer[4] << 8) | raw_buffer[5];
        int16_t raw_gx = (raw_buffer[8] << 8) | raw_buffer[9];
        int16_t raw_gy = (raw_buffer[10] << 8) | raw_buffer[11];
        int16_t raw_gz = (raw_buffer[12] << 8) | raw_buffer[13];

        data.accel_x = raw_ax / 16384.0f;
        data.accel_y = raw_ay / 16384.0f;
        data.accel_z = raw_az / 16384.0f;
        data.gyro_x  = raw_gx / 131.0f;
        data.gyro_y  = raw_gy / 131.0f;
        data.gyro_z  = raw_gz / 131.0f;

        data.angle_pitch = atan2(-data.accel_x, sqrt(data.accel_y * data.accel_y + data.accel_z * data.accel_z)) * 180 / M_PI;
        data.angle_roll = atan2(data.accel_y, data.accel_z) * 180 / M_PI;

        return ESP_OK;
    }

    esp_err_t mpu6050_t::write_reg(uint8_t reg, uint8_t val) {
        uint8_t data[] = {reg, val};
        return i2c_master_transmit(dev_handle, data, sizeof(data), -1);
    }

    esp_err_t mpu6050_t::read_regs(uint8_t reg, uint8_t *buffer, size_t len) {
        return i2c_master_transmit_receive(dev_handle, &reg, 1, buffer, len, -1);
    }
}