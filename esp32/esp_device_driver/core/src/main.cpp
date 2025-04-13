/**
 * @file main.cpp
 * @author Ziad Mohammed Fathy
 * @brief Entry point and logic for the embedded application using ESP-IDF
 * @version 0.1
 * @date 2025-04-13
 */

#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERSION
#include "esp_log.h"
#define LOG_TAG "MAIN"

static Main my_main;

/**
 * @brief Main entry point for the ESP-IDF application
 * 
 */
extern "C" void app_main(void) {
	ESP_ERROR_CHECK(my_main.setUp());
	while(true) {
		my_main.Loop();
	}
}

/**
 * @brief Initializes system components (GPIO, peripherals, etc.)
 * 
 * @return esp_err_t ESP_OK if successful, error code otherwise
 */
esp_err_t Main::setUp() {
	esp_err_t status{ESP_OK};
	ESP_LOGI(LOG_TAG, "hello, setUp is done !");
	Led = new GPIO::GPIO_Output(GPIO_NUM_19, true);
	status |= Led->init();
	return status;
}

/**
 * @brief Main loop function, toggles the LED with log messages
 */
void Main::Loop() {
	ESP_LOGI(LOG_TAG, "hello, I'm Ziad mohammed");
	ESP_LOGI(LOG_TAG, "Led is OFF");
	Led->set(true);
	vTaskDelay(250);
	ESP_LOGI(LOG_TAG, "Led is ON");
	Led->set(false);
	vTaskDelay(250);
}

