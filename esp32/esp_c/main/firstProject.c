#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERSION
#include "esp_log.h"
#define LOG_TAG "MAIN"

void app_main(void) {
    while(true) {
       ESP_LOGI(LOG_TAG, "hello");
       vTaskDelay(1000);
    }
}
