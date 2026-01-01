#ifndef INC_POTENTIOMETER_H_
#define INC_POTENTIOMETER_H_

#include "mcal/adc/adc.h"

namespace pot_module {
    class pot_t {
    public:
        pot_t(adc_module::adc_cfg_t &adc_cfg);
        ~pot_t() = default;

        int get_raw();   
        int get_voltage();    

    private:
        adc_module::adc_t adc_handle;
        const char* POT_TAG;
        
        const int MAX_VOLTAGE_MV = 3100; 
    };
}

#endif