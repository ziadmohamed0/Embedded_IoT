set(SOURCE WiFi.cpp)

idf_component_register(SRCS ${SOURCE}
                      INCLUDE_DIRS .
                      REQUIRES esp_wifi esp_event esp_netif lwip)