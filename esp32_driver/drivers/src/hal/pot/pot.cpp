#include "pot.h"

const char *POT_TAG = "POTENTIOMETER_DRIVER";

adc_oneshot_unit_handle_t Potentiometer::adc1_unit = nullptr;

Potentiometer::Potentiometer(adc_unit_t adc_unit, 
                             adc_channel_t adc_channel,
                             adc_atten_t atten)
    : unit(adc_unit), 
      channel(adc_channel), 
      attenuation(atten),
      adc_handle(nullptr),
      cali_handle(nullptr),
      cali_enabled(false) {
    this->init();
}

void Potentiometer::init() {
    esp_err_t ret;

    if (this->unit == ADC_UNIT_1) {
        if (!adc1_unit) {
            adc_oneshot_unit_init_cfg_t init_config = {};
            init_config.unit_id = ADC_UNIT_1;
            init_config.ulp_mode = ADC_ULP_MODE_DISABLE;

            ret = adc_oneshot_new_unit(&init_config, &adc1_unit);
            if (ret != ESP_OK) {
                ESP_LOGE(POT_TAG, "Failed to initialize shared ADC1 unit: %s", esp_err_to_name(ret));
                return;
            }
        }
        this->adc_handle = adc1_unit;
    } else {
        adc_oneshot_unit_init_cfg_t init_config2 = {};
        init_config2.unit_id = ADC_UNIT_2;
        init_config2.ulp_mode = ADC_ULP_MODE_DISABLE;

        ret = adc_oneshot_new_unit(&init_config2, &this->adc_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(POT_TAG, "Failed to initialize ADC2 unit: %s", esp_err_to_name(ret));
            return;
        }
    }

    adc_oneshot_chan_cfg_t chan_cfg = {};
    chan_cfg.atten = this->attenuation;
    chan_cfg.bitwidth = ADC_BITWIDTH_12;

    ret = adc_oneshot_config_channel(this->adc_handle, this->channel, &chan_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(POT_TAG, "Failed to configure ADC channel: %s", esp_err_to_name(ret));
        return;
    }

    adc_cali_line_fitting_config_t cali_cfg = {};
    cali_cfg.unit_id = this->unit;
    cali_cfg.atten = this->attenuation;
    cali_cfg.bitwidth = ADC_BITWIDTH_12;
    cali_cfg.default_vref = 1100;  

    ret = adc_cali_create_scheme_line_fitting(&cali_cfg, &this->cali_handle);
    if (ret == ESP_OK) {
        this->cali_enabled = true;
        ESP_LOGI(POT_TAG, "Calibration enabled");
    } else {
        this->cali_enabled = false;
        ESP_LOGW(POT_TAG, "Calibration failed, using raw values: %s", esp_err_to_name(ret));
    }

    ESP_LOGI(POT_TAG, "Potentiometer initialized on ADC%d Channel %d", this->unit + 1, this->channel);
}

int Potentiometer::readRaw() {
    int raw_value = 0;
    esp_err_t ret = adc_oneshot_read(this->adc_handle, this->channel, &raw_value);
    
    if (ret != ESP_OK) {
        ESP_LOGE(POT_TAG, "Failed to read ADC: %s", esp_err_to_name(ret));
        return 0;
    }
    
    return raw_value;
}

float Potentiometer::readVoltage() {
    int raw = this->readRaw();
    int voltage_mv = 0;
    
    if (this->cali_enabled) {
        esp_err_t ret = adc_cali_raw_to_voltage(this->cali_handle, raw, &voltage_mv);
        if (ret != ESP_OK) {
            ESP_LOGE(POT_TAG, "Failed to convert to voltage: %s", esp_err_to_name(ret));
            return 0.0f;
        }
    } else {
        // Approximate voltage without calibration
        // For ADC_ATTEN_DB_12: 0-3100mV range
        voltage_mv = (raw * 3100) / ADC_MAX_VALUE;
    }
    
    return voltage_mv / 1000.0f;  // Convert to volts
}

int Potentiometer::readAngle() {
    int raw = readRaw();
    float angle = ((float)raw / ADC_MAX_VALUE) * ANGLE_MAX;
    return (int)angle;
}

float Potentiometer::readPercentage() {
    int raw = this->readRaw();
    float percentage = (raw * 100.0f) / static_cast<float>(ADC_MAX_VALUE);
    return percentage;
}

float Potentiometer::readMapped(float min_value, float max_value) {
    float percentage = this->readPercentage();
    float mapped = min_value + (percentage / 100.0f) * (max_value - min_value);
    return mapped;
}

float Potentiometer::readAveraged(uint8_t samples) {
    if (samples == 0) {
        ESP_LOGW(POT_TAG, "Invalid sample count, using default (10)");
        samples = 10;
    }
    
    float sum = 0.0f;
    for (uint8_t i = 0; i < samples; i++) {
        sum += this->readPercentage();
        vTaskDelay(pdMS_TO_TICKS(2));  // Small delay between samples
    }
    
    float average = sum / samples;
    return average;
}

Potentiometer::~Potentiometer() {
    if (this->cali_enabled && this->cali_handle) {
        adc_cali_delete_scheme_line_fitting(this->cali_handle);
    }

    if (this->unit == ADC_UNIT_1) {

    } 
    
    else {

        if (this->adc_handle) {
            adc_oneshot_del_unit(this->adc_handle);
        }
    }
}