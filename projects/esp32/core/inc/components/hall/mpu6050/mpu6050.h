#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "mcal/mcal_dfs.h"

namespace sensor_module {

    struct mpu_data_t {
        float accel_x, accel_y, accel_z;
        float gyro_x, gyro_y, gyro_z;
        float angle_pitch, angle_roll;
    };

    class mpu6050_t {
    public:
        mpu6050_t(i2c_module::i2c_t &bus, uint8_t dev_addr = 0x68);
        
        esp_err_t update();
        mpu_data_t get_data() { return data; }

    private:
        i2c_master_dev_handle_t dev_handle;
        mpu_data_t data;
        static constexpr const char* TAG = "MPU6050_DRIVER";

        esp_err_t write_reg(uint8_t reg, uint8_t val);
        esp_err_t read_regs(uint8_t reg, uint8_t *buffer, size_t len);
    };
}

#endif