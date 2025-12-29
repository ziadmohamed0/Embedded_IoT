#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "common.h"

namespace gpio_module {
    namespace cfg  {
        enum class Mode_t : uint8_t {
            OutPut, InPut
        };

        enum class PinIndex_t : uint8_t {
            _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, 
            _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, 
            _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, 
            _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, 
            _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, 
            _50, _51, _52, _53, _54 
        };

        enum class Pulling_t : uint8_t {
            pull_up, pull_down, pull_up_down, floating
        };

        enum class Status : uint8_t {
            High, Low
        };
    }

    struct cfg_t {
        cfg::PinIndex_t pin;
        cfg::Mode_t mode;
        cfg::Pulling_t pulling;
    };
    

    class gpio_t {
        public:
            gpio_t() : GPIO_TAG("GPIO_MODULE"){}
            gpio_t(cfg_t copy_cfg);
            ~gpio_t() = default;

            esp_err_t write(cfg::Status copy_status);
            bool get();
        private:
            const char* GPIO_TAG;
            cfg_t cfg;
            gpio_num_t pin_number;
            gpio_mode_t mode_type;
            gpio_pull_mode_t pulling_mode;
            esp_err_t set_pulling();
    };
    
}

#endif