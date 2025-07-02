/**
 * @file mqtt_client_custom.h
 * @author Ziad Mohammed Fathy
 * @brief mqtt driver.
 * @version 0.1
 * @date 2025-05-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _INC_MQTT_CLIENT_CUSTOM_H_
#define _INC_MQTT_CLIENT_CUSTOM_H_

#include <string>
#include <functional>
#include "mqtt_client.h"

class mqtt_client_custom {
public:
    mqtt_client_custom(const std::string& copySsd, const std::string& copyPassword, const std::string& copyBroker);
    void begin();
    void publish(const std::string& copyTopic, const std::string& copyPayload);
    void subscrib(const std::string& copyTopic);
    void on_message(std::function<void(const char*, const char*)> cb);
    static std::function<void(const char*, const char*)> global_cb;

private:
    std::string ssd;
    std::string password;
    std::string broker;
    esp_mqtt_client_handle_t client;

    void init_wifi();
    void init_mqtt();

    static void mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data);
};

#endif
