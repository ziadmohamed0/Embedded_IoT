/**
 * @file GPIO.h
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-13
 */

#ifndef APPLICATION_GPIO_H_
#define APPLICATION_GPIO_H_

#include "driver/gpio.h"

namespace GPIO{
  class GpioBase {
    protected:
        const bool _inverted_logic{false};
        const gpio_num_t  _pinNumber;
        const gpio_config_t _cfg;
    public:
        constexpr GpioBase(const gpio_num_t copyPinNumber, 
                    const gpio_config_t &copyCfg, 
                    const bool copyInvertedLogic = false) :
        _inverted_logic{copyInvertedLogic},
        _pinNumber{copyPinNumber},
        _cfg{copyCfg} {

        }
        virtual bool state(void) = 0;
        virtual esp_err_t set(const bool copyState) = 0;
        [[nodiscard]] esp_err_t init(void);
  };
  
  class GpioOutput : public GpioBase {
    private:
        bool _state{false};
    public:
        constexpr GpioOutput(const gpio_num_t copyPinNumber, 
                                const bool copyInvertedLogic = false) : 
        GpioBase{copyPinNumber, 
                    gpio_config_t {
                        .pin_bit_mask   = static_cast<uint64_t>(1) << copyPinNumber,
                        .mode           = GPIO_MODE_OUTPUT,
                        .pull_up_en     = GPIO_PULLUP_DISABLE,
                        .pull_down_en   = GPIO_PULLDOWN_ENABLE,
                        .intr_type      = GPIO_INTR_DISABLE 
                    }, 
                    copyInvertedLogic} {

        }
        [[nodiscard]] esp_err_t init(void);
        esp_err_t set(const bool copyState);
        // esp_err_t toogle(void);
        bool state(void) {return _state;}
  };
/*
  class GpioInput {
    private:
        const bool _inverted_logic{false};
        const gpio_num_t  _pinNumber;
    public:
        esp_err_t init(void);
        bool state(void);
  };
  */
} 

#endif
