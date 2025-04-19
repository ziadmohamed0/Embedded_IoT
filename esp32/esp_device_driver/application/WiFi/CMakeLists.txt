set(SRC_FILES WiFi.cpp)
idf_component_register(SRCS ${SRC_FILES}
						INCLUDE_DIRS .
						REQUIRES esp_wifi esp_event esp_netif lwip)
