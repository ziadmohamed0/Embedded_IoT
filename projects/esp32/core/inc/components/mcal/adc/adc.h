#ifndef INC_ADC_MODULE_H_
#define INC_ADC_MODULE_H_

#include "common.h"

namespace adc_module {
    namespace cfg {
        enum class Unit_t : uint8_t {
            ADC_1 = ADC_UNIT_1,
            ADC_2 = ADC_UNIT_2
        };

        enum class Resolution_t : uint8_t {
            BIT_9  = ADC_BITWIDTH_9,
            BIT_10 = ADC_BITWIDTH_10,
            BIT_11 = ADC_BITWIDTH_11,
            BIT_12 = ADC_BITWIDTH_12,
            BIT_DEFAULT = ADC_BITWIDTH_DEFAULT
        };

        enum class Atten_t : uint8_t {
            DB_0  = ADC_ATTEN_DB_0,
            DB_2_5 = ADC_ATTEN_DB_2_5, 
            DB_6  = ADC_ATTEN_DB_6, 
            DB_11 = ADC_ATTEN_DB_12   
        };
    }

    struct adc_cfg_t {
        adc_unit_t unit;
        adc_channel_t channel;
        adc_atten_t attenuation;
        adc_bitwidth_t resolution;
    };

    class adc_t {
    public:
        adc_t(adc_cfg_t copy_cfg);
        ~adc_t();

        int read_raw();        
        int read_voltage();    

    private:
        const char* ADC_TAG;
        adc_cfg_t cfg;
        
        static adc_oneshot_unit_handle_t adc1_handle; 
        adc_cali_handle_t cali_handle;
        bool do_calibration;

        esp_err_t init_adc_unit();
        esp_err_t init_adc_calibration();
    };
}

#endif