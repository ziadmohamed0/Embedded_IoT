set(SRC_FILES ./src/main.cpp)
idf_component_register(SRCS ${SRC_FILES}
			INCLUDE_DIRS . inc
			REQUIRES freertos esp_common Gpio WiFi)
