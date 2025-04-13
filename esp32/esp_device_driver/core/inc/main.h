/**
 * @file main.h
 * @author Ziad Mohammed Fathy
 * @brief Header file for Main class managing application logic
 * @version 0.1
 * @date 2025-04-13
 */

#ifndef _CORE_INC_MAIN_H_
#define _CORE_INC_MAIN_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "GPIO.h"

/**
 * @brief Main class controlling application setup and loop
 */
class Main final {
	public:
		/**
		 * @brief Setup GPIOs and other system components
		 * 
		 * @return esp_err_t ESP_OK if successful
		 */
		esp_err_t setUp(void);

		/**
		 * @brief Main loop to run application logic
		 */
		void Loop(void);

		GPIO::GPIO_Output *Led;  ///< Pointer to GPIO output object (LED control)
};

#endif

