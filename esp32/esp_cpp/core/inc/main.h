#ifndef _CORE_INC_MAIN_H_
#define _CORE_INC_MAIN_H_ 

// #include <stdio.h>
// #include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "GPIO.h"

class Main final {
public:
    esp_err_t setUp(void);
    void loop(void);
    GPIO::GpioOutput *Led;
};

#endif