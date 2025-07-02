#include <string>
#include "mqtt_client.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

static esp_mqtt_client_handle_t client;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_DATA:
            std::string msg(event->data, event->data_len);
            if (strncmp(event->topic, "led/control", event->topic_len) == 0) {
                if (msg == "ON") {
                    gpio_set_level(LED_PIN, 1);
                } else if (msg == "OFF") {
                    gpio_set_level(LED_PIN, 0);
                }
            }
            break;
        default:
            break;
    }
}

extern "C" void app_main() {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://192.168.100.25:1883",
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
    esp_mqtt_client_subscribe(client, "led/control", 0);
}
