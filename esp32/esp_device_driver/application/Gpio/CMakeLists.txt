set(SRC_FILES GPIO.cpp)
idf_component_register(SRCS ${SRC_FILES}
						INCLUDE_DIRS .
						REQUIRES driver)
