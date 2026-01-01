#include "potentiometer/potentiometer.h"

namespace pot_module {
    
    pot_t::pot_t(adc_module::adc_cfg_t &adc_cfg) : 
                adc_handle(adc_cfg), 
                POT_TAG("POT_MODULE") {}

    int pot_t::get_raw() {
        return adc_handle.read_raw();
    }

    int pot_t::get_voltage() {
        return adc_handle.read_voltage();
    }
}