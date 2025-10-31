#include "main.h"

const char *APP_TAG = "MAIN_APP";

const char* ssid = "Mohamed Fathy";
const char* password = "341978341978";
const char* broker = "mqtt://192.168.100.25";
    
// Topics
const char* motor_topic = "motor/control";
const char* distance1_topic = "sensor/distanceForward";
const char* distance2_topic = "sensor/distanceBackward";

extern "C" void app_main(void) {
    ESP_LOGI(APP_TAG, "System start");

    ESP_ERROR_CHECK(nvs_flash_init());

    WiFiManager wifi;
    wifi.init_sta(ssid, password);
    wifi.connect();

    while (!wifi.is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    ESP_LOGI(APP_TAG, "WiFi connected, IP: %s", wifi.get_ip_address().c_str());

    MQTTClient mqtt;
    MQTTConfig cfg;
    cfg.broker_uri = broker;
    cfg.client_id = "ESP32_DC_MOTOR";
    cfg.port = 1883;
    cfg.keepalive = 60;
    mqtt.init(cfg);

    mqtt.on_connected([&]() {
        ESP_LOGI(APP_TAG, "Connected to MQTT Broker");
        mqtt.subscribe(motor_topic);
    });

    mqtt.on_message([&](std::string topic, std::string msg) {
        ESP_LOGI(APP_TAG, "Received: %s -> %s", topic.c_str(), msg.c_str());
    });

    mqtt.connect();

    while (!mqtt.is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    dc_motor motors({GPIO_NUM_16,
                     GPIO_NUM_17, 
                     GPIO_NUM_18, 
                     GPIO_NUM_19
                    });

    mqtt.on_topic(motor_topic, [&](std::string topic, std::string payload) {
        if (payload == "forward")
            motors.dc_move(DC_MOTOR_DIRECTION::forward);

        else if (payload == "backward")
            motors.dc_move(DC_MOTOR_DIRECTION::backward);
        
        else if (payload == "right")
            motors.dc_move(DC_MOTOR_DIRECTION::right);
    
        else if (payload == "left")
            motors.dc_move(DC_MOTOR_DIRECTION::left);
        
        else if (payload == "stop")
            motors.dc_move(DC_MOTOR_DIRECTION::stop);
    
        else
            ESP_LOGW(APP_TAG, "Unknown command: %s", payload.c_str());
    });

    ultrasonic sensorForward(GPIO_NUM_26, GPIO_NUM_25), sensorBackWard(GPIO_NUM_33, GPIO_NUM_32); 

    ESP_LOGI(APP_TAG, "System ready");
    
    while (true) {
        float data_1 = sensorForward.readDistance();
        float data_2 = sensorBackWard.readDistance();
        mqtt.publish(distance1_topic, std::to_string(data_1));
        mqtt.publish(distance2_topic, std::to_string(data_2));

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
