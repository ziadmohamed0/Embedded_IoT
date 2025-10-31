/**
 * @file main.cpp
 * @author Ziad Mohamed Fathy
 * @brief esp32 suit inputs and publish data to mqtt broker
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

// pots right hand I/P
Potentiometer   *pot_right_shoulder1    = nullptr;
Potentiometer   *pot_right_shoulder2    = nullptr;
// Potentiometer   *pot_right_elbow        = nullptr;
// Potentiometer   *pot_right_wrist        = nullptr;
Button          *btn_right_griper       = nullptr;

// pots left hand I/P
Potentiometer   *pot_left_shoulder1     = nullptr;
Potentiometer   *pot_left_shoulder2     = nullptr;
Potentiometer   *pot_left_elbow         = nullptr;
Potentiometer   *pot_left_wrist         = nullptr;
Button          *btn_left_griper        = nullptr;

// btns of rover
Button          *btn_forward            = nullptr;  
Button          *btn_backward           = nullptr;  
Button          *btn_right              = nullptr;  
Button          *btn_left               = nullptr;  

// prototyping functions
void setup_wifi();
void setup_mqtt();
void setup_HW();

extern "C" void app_main(void) {
    setup_HW();
    setup_wifi();
    setup_mqtt();

    while(true) {
        mqtt->publish(topic_forward, std::to_string(btn_forward->GET()));
        mqtt->publish(topic_backward, std::to_string(btn_backward->GET()));
        mqtt->publish(topic_right, std::to_string(btn_right->GET()));
        mqtt->publish(topic_left, std::to_string(btn_left->GET()));

        mqtt->publish(topic_right_shoulder1, std::to_string(pot_right_shoulder1->readRaw()));
        mqtt->publish(topic_right_shoulder2, std::to_string(pot_right_shoulder2->readRaw()));
        // mqtt->publish(topic_right_elbow, std::to_string(pot_right_elbow->readRaw()));
        // mqtt->publish(topic_right_wrist, std::to_string(pot_right_wrist->readRaw()));
        mqtt->publish(topic_right_griper, std::to_string(btn_right_griper->GET()));

        mqtt->publish(topic_left_shoulder1, std::to_string(pot_left_shoulder1->readRaw()));
        mqtt->publish(topic_left_shoulder2, std::to_string(pot_left_shoulder2->readRaw()));
        mqtt->publish(topic_left_elbow, std::to_string(pot_left_elbow->readRaw()));
        mqtt->publish(topic_left_wrist, std::to_string(pot_left_wrist->readRaw()));
        mqtt->publish(topic_left_griper, std::to_string(btn_left_griper->GET()));

        vTaskDelay(pdMS_TO_TICKS(250));
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
    btn_forward         = new Button(GPIO_NUM_0);
    btn_backward        = new Button(GPIO_NUM_4);
    btn_right           = new Button(GPIO_NUM_16);
    btn_left            = new Button(GPIO_NUM_17);

    pot_left_shoulder1  = new Potentiometer(ADC_UNIT_1, ADC_CHANNEL_0); 
    pot_left_shoulder2  = new Potentiometer(ADC_UNIT_1, ADC_CHANNEL_3);
    pot_left_elbow      = new Potentiometer(ADC_UNIT_1, ADC_CHANNEL_6);
    pot_left_wrist      = new Potentiometer(ADC_UNIT_1, ADC_CHANNEL_7);
    btn_left_griper     = new Button(GPIO_NUM_5);

    pot_right_shoulder1 = new Potentiometer(ADC_UNIT_1, ADC_CHANNEL_4);
    pot_right_shoulder2 = new Potentiometer(ADC_UNIT_1, ADC_CHANNEL_5);
    // pot_right_elbow     = new Potentiometer(ADC_UNIT_2, ADC_CHANNEL_8);
    // pot_right_wrist     = new Potentiometer(ADC_UNIT_2, ADC_CHANNEL_9);
    btn_right_griper    = new Button(GPIO_NUM_18);

    nvs = new NVS();
}