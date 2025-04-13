set(SOURCE ./src/main.cpp)

idf_component_register(SRCS ${SOURCE}
                        INCLUDE_DIRS . inc
                        REQUIRES freertos esp_common GPIO)