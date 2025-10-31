#ifndef POT_H_
#define POT_H_

#include "common.h"

extern const char *POT_TAG;

class Potentiometer {
public:
    static adc_oneshot_unit_handle_t adc1_unit; // shared

    /**
     * @brief Construct a new Potentiometer object
     * 
     * @param adc_unit ADC unit (ADC_UNIT_1 or ADC_UNIT_2)
     * @param adc_channel ADC channel for potentiometer reading
     * @param atten Attenuation level (default ADC_ATTEN_DB_12 for 0-3.3V)
     */
    Potentiometer(adc_unit_t adc_unit, 
                  adc_channel_t adc_channel,
                  adc_atten_t atten = ADC_ATTEN_DB_12);

    /**
     * @brief Read raw ADC value
     * 
     * @return int Raw ADC value (0-4095 for 12-bit)
     */
    int readRaw();


    int readAngle();

    /**
     * @brief Read voltage value
     * 
     * @return float Voltage in millivolts
     */
    float readVoltage();

    /**
     * @brief Read percentage value
     * 
     * @return float Percentage (0.0 - 100.0)
     */
    float readPercentage();

    /**
     * @brief Read mapped value to custom range
     * 
     * @param min_value Minimum output value
     * @param max_value Maximum output value
     * @return float Mapped value
     */
    float readMapped(float min_value, float max_value);

    /**
     * @brief Read averaged value over multiple samples
     * 
     * @param samples Number of samples to average (default 10)
     * @return float Averaged percentage value
     */
    float readAveraged(uint8_t samples = 10);

    ~Potentiometer();

private:
    adc_unit_t unit;
    adc_channel_t channel;
    adc_atten_t attenuation;
    
    adc_oneshot_unit_handle_t adc_handle;
    adc_cali_handle_t cali_handle;
    
    bool cali_enabled;
    
    const int ADC_MAX_VALUE = 4095;  // 12-bit ADC
    const int ANGLE_MAX = 300;
    
    void init();
};

#endif // POT_H_