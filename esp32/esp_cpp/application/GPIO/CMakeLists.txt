set(SOURCE GPIO.cpp)

idf_component_register(SRCS ${SOURCE}
                        INCLUDE_DIRS .
                        REQUIRES driver)