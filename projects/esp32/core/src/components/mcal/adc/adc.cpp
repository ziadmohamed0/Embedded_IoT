#include "adc/adc.h"

namespace adc_module {
    adc_oneshot_unit_handle_t adc_t::adc1_handle = nullptr;

    adc_t::adc_t(adc_cfg_t copy_cfg) : 
                ADC_TAG("ADC_MODULE"), 
                cfg(copy_cfg), 
                cali_handle(nullptr), 
                do_calibration(false) {
        
        if (adc1_handle == nullptr) {
            init_adc_unit();
        }

        adc_oneshot_chan_cfg_t chan_cfg = {
            .atten = cfg.attenuation,
            .bitwidth = cfg.resolution,
        };
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, cfg.channel, &chan_cfg));

        init_adc_calibration();
    }

    esp_err_t adc_t::init_adc_unit() {
        adc_oneshot_unit_init_cfg_t init_cfg = {
            .unit_id = cfg.unit,
            .clk_src = (adc_oneshot_clk_src_t)0, 
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };
        return adc_oneshot_new_unit(&init_cfg, &adc1_handle);
    }

    esp_err_t adc_t::init_adc_calibration() {
        
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = cfg.unit,
            .atten = cfg.attenuation,
            .bitwidth = cfg.resolution,
            .default_vref = 0, 
        };
        
        esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle);
        if (ret == ESP_OK) {
            do_calibration = true;
        }
        return ret;
    }

    int adc_t::read_raw() {
        int raw_out = 0; 
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, cfg.channel, &raw_out));
        return raw_out;
    }

    int adc_t::read_voltage() {
        int raw = read_raw();
        int voltage = 0;
        if (do_calibration && cali_handle != nullptr) {
            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw, &voltage));
        } else {
            voltage = (raw * 3300) / 4095; 
        }
        return voltage;
    }

    adc_t::~adc_t() {
        if (do_calibration && cali_handle != nullptr) {
            adc_cali_delete_scheme_line_fitting(cali_handle);
        }
    }
}