/**
 * @file GPIO.h
 * @author Ziad Mohammed Fathy
 * @brief Declaration of GPIO classes for input/output control
 * @version 0.1
 * @date 2025-04-13
 */

#ifndef _APPLICATION_GPIO_H_
#define _APPLICATION_GPIO_H_

#include "driver/gpio.h"

namespace GPIO {

	/**
	 * @brief Abstract base class for GPIO configuration and control
	 */
	class GPIO_Base {
		protected:
			const gpio_num_t pinNumber;        ///< GPIO pin number
			const gpio_config_t cfg;           ///< Configuration struct
			const bool invertedLogic{false};   ///< Invert logic flag (true = logic reversed)

		public:
			/**
			 * @brief Construct a new GPIO_Base object
			 * 
			 * @param copyPinNumber GPIO pin
			 * @param copyCFG Configuration object
			 * @param copyInvertedLogic Inverted logic option
			 */
			constexpr GPIO_Base(const gpio_num_t copyPinNumber, 
								const gpio_config_t & copyCFG, 
								const bool copyInvertedLogic = false) :
					pinNumber{copyPinNumber},
					cfg{copyCFG},
					invertedLogic{copyInvertedLogic} {}

			/**
			 * @brief Get the current state of the GPIO pin
			 * 
			 * @return true or false based on pin level
			 */
			virtual bool state(void) = 0;

			/**
			 * @brief Set the GPIO pin output level
			 * 
			 * @param copyStatus Desired output level
			 * @return esp_err_t ESP_OK if successful
			 */
			virtual esp_err_t set(const bool copyStatus) = 0;

			/**
			 * @brief Initialize the GPIO with its configuration
			 * 
			 * @return esp_err_t ESP_OK if successful
			 */
			[[nodiscard]] esp_err_t init(void);
	};

	/**
	 * @brief Class for controlling GPIO output pins
	 */
	class GPIO_Output : public GPIO_Base {
		private:
			bool status{false};  ///< Stores current pin status

		public:
			/**
			 * @brief Construct a new GPIO_Output object
			 * 
			 * @param copyPinNumber GPIO pin
			 * @param copyInvertedLogic Inverted logic option
			 */
			constexpr GPIO_Output(const gpio_num_t copyPinNumber, 
								const bool copyInvertedLogic = false) : 
								GPIO_Base(copyPinNumber,
								gpio_config_t {
									.pin_bit_mask = static_cast<uint64_t>(1) << copyPinNumber,
									.mode = GPIO_MODE_OUTPUT,
									.pull_up_en = GPIO_PULLUP_DISABLE,
									.pull_down_en = GPIO_PULLDOWN_ENABLE,
									.intr_type = GPIO_INTR_DISABLE
								},
								copyInvertedLogic) {}

			/**
			 * @brief Initialize the GPIO output pin
			 * 
			 * @return esp_err_t ESP_OK if successful
			 */
			[[nodiscard]] esp_err_t init(void);

			/**
			 * @brief Set output value of the pin
			 * 
			 * @param copyStatus Desired state
			 * @return esp_err_t ESP_OK if successful
			 */
			esp_err_t set(const bool copyStatus);

			/**
			 * @brief Get the current output status
			 * 
			 * @return true if on, false if off
			 */
			bool state(void) { return status; }
	};

}

#endif  // _APPLICATION_GPIO_H_

