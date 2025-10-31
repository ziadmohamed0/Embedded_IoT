/**
 * @file main.cpp
 * @author Ziad Mohamed Fathy
 * @brief esp32 suit outputs and subscription data from mqtt broker
 * @version 0.1
 * @date 2025-10-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "main.h"

const char *TAG = "main_task";

// global variables (objects)
WiFiManager     *wifi = nullptr;
MQTTClient      *mqtt = nullptr;
NVS             *nvs  = nullptr; 

// motor pins


// rover dc-motor
BTS7960 *right_motors = nullptr;
BTS7960 *left_motors  = nullptr;

extern "C" void app_main(void){
    setup_HW();
    setup_wifi();
    setup_mqtt();
    
    while(true) {

    }
}

void setup_wifi() {
    wifi = new WiFiManager();

    wifi->on_connected([](std::string ip) {
        ESP_LOGI(TAG, "WiFi connected ip: %s", ip.c_str());
    });

    wifi->on_disconnected([](){
        ESP_LOGW(TAG, "WiFi Disconnected !");
    });

    wifi->init_sta(ssid, password, 5);
    wifi->connect();
}

void setup_mqtt() {
    mqtt = new MQTTClient();

    MQTTConfig cfg;
    cfg.broker_uri  = mqtt_broker;
    cfg.port        = mqtt_port;
    cfg.client_id = "ESP32_test_" + std::to_string(rand() % 10000);
    cfg.keepalive = 60;

    mqtt->init(cfg);

    mqtt->on_connected([&]() {
        ESP_LOGI(TAG, "Connected to MQTT Broker");
    });

    mqtt->on_message([&](std::string topic, std::string msg) {
        ESP_LOGI(TAG, "Received: %s -> %s", topic.c_str(), msg.c_str());
    });

    mqtt->connect();

    while (!mqtt->is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup_HW() {
    
    nvs = new NVS();
}
