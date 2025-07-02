/**
 * @file mqtt_client_custom.cpp
 * @author Ziad Mohammed Fathy
 * @brief Custom MQTT client for ESP32:
 *        - Initializes WiFi connection with given SSID and password
 *        - Sets up MQTT client with broker address and credentials
 *        - Supports publishing and subscribing to topics
 *        - Allows user-defined callback for incoming MQTT messages
 *        - Handles MQTT events internally (connect, data, errors)
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../inc/mqtt_client_custom.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0

static EventGroupHandle_t wifi_event_group;
std::function<void(const char*, const char*)> mqtt_client_custom::global_cb = nullptr;
static const char* TAG = "MQTTClient";

/**
 * @brief Construct a new mqtt_client_custom object
 * 
 * @param copySsd WiFi SSID
 * @param copyPassword WiFi password
 * @param copyBroker MQTT broker URI
 */
mqtt_client_custom::mqtt_client_custom(const std::string& copySsd, 
                                       const std::string& copyPassword, 
                                       const std::string& copyBroker) 
    : ssd(copySsd), password(copyPassword), broker(copyBroker), client(nullptr) {}

void mqtt_client_custom::begin() {
    this->init_wifi();
    this->init_mqtt();
}

/**
 * @brief Publish payload to a topic
 * 
 * @param copyTopic Topic to publish to
 * @param copyPayload Message payload
 */
void mqtt_client_custom::publish(const std::string& copyTopic, const std::string& copyPayload) {
    if (client) {
        esp_mqtt_client_publish(client, copyTopic.c_str(), copyPayload.c_str(), 0, 1, 0);
    }
}

/**
 * @brief Subscribe to a topic
 * 
 * @param copyTopic Topic to subscribe to
 */
void mqtt_client_custom::subscrib(const std::string& copyTopic) {
    if (client) {
        esp_mqtt_client_subscribe(client, copyTopic.c_str(), 1);
    }
}

/**
 * @brief Register callback for incoming MQTT messages
 * 
 * @param cb Callback function taking topic and payload
 */
void mqtt_client_custom::on_message(std::function<void(const char*, const char*)> cb) {
    global_cb = cb;
}

/**
 * @brief Initialize WiFi connection
 */
void mqtt_client_custom::init_wifi() {
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {};
    strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), ssd.c_str(), sizeof(wifi_config.sta.ssid));
    strncpy(reinterpret_cast<char*>(wifi_config.sta.password), password.c_str(), sizeof(wifi_config.sta.password));

    wifi_event_group = xEventGroupCreate();

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, [](void*, esp_event_base_t base, int32_t id, void*) {
        if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START)
            esp_wifi_connect();
        else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED)
            esp_wifi_connect();
    }, nullptr);

    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, [](void*, esp_event_base_t, int32_t, void*) {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }, nullptr);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "WiFi connected to %s", ssd.c_str());
}

/**
 * @brief Initialize MQTT client and register event handlers
 */
void mqtt_client_custom::init_mqtt()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address = {
                .uri = broker.c_str()
            }
        },
        .credentials = {
            .username = "Mohamed Fathy",
            .authentication = {
                .password = "341978341978"
            }
        }
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to create MQTT client");
        return;
    }

    esp_mqtt_event_id_t events[] = {
        MQTT_EVENT_CONNECTED,
        MQTT_EVENT_DISCONNECTED,
        MQTT_EVENT_SUBSCRIBED,
        MQTT_EVENT_UNSUBSCRIBED,
        MQTT_EVENT_PUBLISHED,
        MQTT_EVENT_DATA,
        MQTT_EVENT_ERROR
    };

    for (size_t i = 0; i < sizeof(events) / sizeof(events[0]); ++i) {
        esp_err_t err = esp_mqtt_client_register_event(client, events[i], mqtt_event_handler, this);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "Failed to register event %d", events[i]);
        }
    }

    esp_err_t start_err = esp_mqtt_client_start(client);
    if (start_err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MQTT client: %s", esp_err_to_name(start_err));
    }
}

/**
 * @brief MQTT event handler
 * 
 * @param handler_args Pointer to this object
 * @param base Event base
 * @param event_id Event ID
 * @param event_data Event data
 */
void mqtt_client_custom::mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected");
            break;

        case MQTT_EVENT_DATA:
            if (global_cb) {
                std::string topic(event->topic, event->topic_len);
                std::string data(event->data, event->data_len);
                global_cb(topic.c_str(), data.c_str());
            }
            break;

        default:
            break;
    }
}
