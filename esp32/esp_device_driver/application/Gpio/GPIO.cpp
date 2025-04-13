/**
 * @file GPIO.cpp
 * @author Ziad Mohammed Fathy
 * @brief Source file for GPIO configuration and control methods
 * @version 0.1
 * @date 2025-04-13
 */

#include "GPIO.h"

namespace GPIO {

	/**
	 * @brief Initializes the GPIO using the stored configuration
	 * 
	 * @return esp_err_t ESP_OK if successful, error otherwise
	 */
	[[nodiscard]] esp_err_t GPIO_Base::init(void) {
		esp_err_t stat{ESP_OK};
		stat |= gpio_config(&cfg);
		return stat;
	}

	/**
	 * @brief Initializes the GPIO output and sets it to default logic
	 * 
	 * @return esp_err_t ESP_OK if successful, error otherwise
	 */
	[[nodiscard]] esp_err_t GPIO_Output::init(void) {
		esp_err_t stat{GPIO_Base::init()};
		if (ESP_OK == stat) {
			stat |= set(invertedLogic);
		}
		return stat;
	}

	/**
	 * @brief Sets the output level of the GPIO pin
	 * 
	 * @param copyStatus Desired logic level
	 * @return esp_err_t ESP_OK if successful
	 */
	esp_err_t GPIO_Output::set(const bool copyStatus) {
		GPIO_Output::status = copyStatus;
		return gpio_set_level(GPIO_Base::pinNumber, 
							  GPIO_Base::invertedLogic ? !copyStatus : copyStatus);
	}

}

