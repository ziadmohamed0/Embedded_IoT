#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERSION
#include "esp_log.h"
#define LOG_TAG "MAIN"

static Main     my_main;

extern "C" void app_main(void) {
    ESP_ERROR_CHECK(my_main.setUp());
    while(true) {
        my_main.loop();
    }
}

esp_err_t Main::setUp() {
    esp_err_t status{ESP_OK};
    ESP_LOGI(LOG_TAG, "setUp!");
    Led = new GPIO::GpioOutput(GPIO_NUM_19, true);
    status |= Led->init();
    return status;
}

void Main::loop() {
    ESP_LOGI(LOG_TAG, "hello esp-idf-c++");
    Led->set(true);
    ESP_LOGI(LOG_TAG, "Led is OFF");
    vTaskDelay(1000);
    Led->set(false);
    ESP_LOGI(LOG_TAG, "Led is ON");
    vTaskDelay(1000);
}