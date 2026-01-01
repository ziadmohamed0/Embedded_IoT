#include "main.h"

TaskHandle_t pot_task_handle = nullptr;
TaskHandle_t mqtt_task_handle = nullptr;

using namespace sensor_module;
using namespace pot_module;
using namespace gpio_module;

int raw_dist = 0;
int raw_speed = 0;

void pot_data_task(void* pdParam) {
    adc_module::adc_cfg_t cfg1 = {ADC_UNIT_1, ADC_CHANNEL_0, ADC_ATTEN_DB_12, ADC_BITWIDTH_12};
    adc_module::adc_cfg_t cfg2 = {ADC_UNIT_1, ADC_CHANNEL_3, ADC_ATTEN_DB_12, ADC_BITWIDTH_12};
    
    pot_t pot1(cfg1);
    pot_t pot2(cfg2);

    while(true) {
        raw_dist = pot1.get_raw();
        raw_speed = pot2.get_raw();
        
        float dist_percent = (raw_dist * 100.0) / 4095.0;
        float speed_percent = (raw_speed * 100.0) / 4095.0;

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void mqtt_publish_task(void* pdParam) {
    MQTTClient* mqtt_client = (MQTTClient*)pdParam;

    while(true) {
        if (mqtt_client->is_connected()) {
            std::string payload = "{\"dist\":" + std::to_string((raw_dist * 100) / 4095) + 
                                  ",\"speed\":" + std::to_string((raw_speed * 100) / 4095) + "}";
            
            mqtt_client->publish("/factory/sensors", payload);
            printf("MQTT Sent: %s\n", payload.c_str());
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

extern "C" void app_main(void) {
    nvs_module::nvs_t nvs_;

    wifi_module::cfg_t wifi_cfg;
    wifi_cfg.ssid = "Mohamed Fathy";     
    wifi_cfg.password = "341978341978";    
    wifi_cfg.mode = wifi_module::cfg::mode_t::sta; 

    wifi_module::wifi_t my_wifi(wifi_cfg);
    // my_wifi.init();
    my_wifi.connect(); 

    vTaskDelay(pdMS_TO_TICKS(1000));

    MQTTConfig config;
    config.broker_uri = "mqtt://192.168.100.25";
    static MQTTClient mqtt_node;
    mqtt_node.init(config);
    mqtt_node.connect();

    xTaskCreate(pot_data_task, "PotTask", 4096, NULL, 5, &pot_task_handle);
    xTaskCreate(mqtt_publish_task, "MqttTask", 4096, &mqtt_node, 5, &mqtt_task_handle);
}   