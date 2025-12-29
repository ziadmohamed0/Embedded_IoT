#include "main.h"

extern "C" void app_main(void) {
    nvs_module::nvs_t nvs_;    
    while(true) {

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}