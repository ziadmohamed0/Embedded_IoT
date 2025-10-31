#include "MQTT.h"

const char* MQTT_TAG = "MQTT_CLIENT";

MQTTClient::MQTTClient() 
    : client(nullptr), status(MQTT_STATUS::DISCONNECTED) {
    ESP_LOGI(MQTT_TAG, "MQTT Client created");
}

MQTTClient::~MQTTClient() {
    if (client) {
        esp_mqtt_client_destroy(client);
    }
}

void MQTTClient::init(const MQTTConfig& cfg) {
    this->config = cfg;
    
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = config.broker_uri.c_str();
    mqtt_cfg.broker.address.port = config.port;
    
    if (!config.client_id.empty()) {
        mqtt_cfg.credentials.client_id = config.client_id.c_str();
    }
    
    if (!config.username.empty()) {
        mqtt_cfg.credentials.username = config.username.c_str();
    }
    
    if (!config.password.empty()) {
        mqtt_cfg.credentials.authentication.password = config.password.c_str();
    }
    
    mqtt_cfg.session.keepalive = config.keepalive;
    mqtt_cfg.network.disable_auto_reconnect = false;
    
    client = esp_mqtt_client_init(&mqtt_cfg);
    
    if (client == nullptr) {
        ESP_LOGE(MQTT_TAG, "Failed to initialize MQTT client");
        status = MQTT_STATUS::ERROR;
        return;
    }
    
    esp_mqtt_client_register_event(client, 
                                   static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID),
                                   event_handler, 
                                   this);
    
    ESP_LOGI(MQTT_TAG, "MQTT Client initialized with broker: %s", config.broker_uri.c_str());
}

void MQTTClient::connect() {
    if (client == nullptr) {
        ESP_LOGE(MQTT_TAG, "MQTT client not initialized");
        return;
    }
    
    esp_err_t err = esp_mqtt_client_start(client);
    if (err == ESP_OK) {
        status = MQTT_STATUS::CONNECTING;
        ESP_LOGI(MQTT_TAG, "Connecting to MQTT broker...");
    } else {
        ESP_LOGE(MQTT_TAG, "Failed to start MQTT client: %s", esp_err_to_name(err));
        status = MQTT_STATUS::ERROR;
    }
}

void MQTTClient::disconnect() {
    if (client && status != MQTT_STATUS::DISCONNECTED) {
        esp_mqtt_client_stop(client);
        status = MQTT_STATUS::DISCONNECTED;
        ESP_LOGI(MQTT_TAG, "Disconnected from MQTT broker");
    }
}

int MQTTClient::publish(const std::string& topic, const std::string& data, int qos, bool retain) {
    if (!is_connected()) {
        ESP_LOGW(MQTT_TAG, "Cannot publish: not connected");
        return -1;
    }
    
    int msg_id = esp_mqtt_client_publish(client, 
                                         topic.c_str(), 
                                         data.c_str(), 
                                         data.length(), 
                                         qos, 
                                         retain ? 1 : 0);
    
    if (msg_id >= 0) {
        ESP_LOGI(MQTT_TAG, "Published to '%s' (msg_id=%d)", topic.c_str(), msg_id);
    } else {
        ESP_LOGE(MQTT_TAG, "Failed to publish to '%s'", topic.c_str());
    }
    
    return msg_id;
}

int MQTTClient::subscribe(const std::string& topic, int qos) {
    if (!is_connected()) {
        ESP_LOGW(MQTT_TAG, "Cannot subscribe: not connected");
        return -1;
    }
    
    int msg_id = esp_mqtt_client_subscribe(client, topic.c_str(), qos);
    
    if (msg_id >= 0) {
        ESP_LOGI(MQTT_TAG, "Subscribed to '%s' (msg_id=%d)", topic.c_str(), msg_id);
    } else {
        ESP_LOGE(MQTT_TAG, "Failed to subscribe to '%s'", topic.c_str());
    }
    
    return msg_id;
}

int MQTTClient::unsubscribe(const std::string& topic) {
    if (!is_connected()) {
        ESP_LOGW(MQTT_TAG, "Cannot unsubscribe: not connected");
        return -1;
    }
    
    int msg_id = esp_mqtt_client_unsubscribe(client, topic.c_str());
    
    if (msg_id >= 0) {
        ESP_LOGI(MQTT_TAG, "Unsubscribed from '%s' (msg_id=%d)", topic.c_str(), msg_id);
        topic_callbacks.erase(topic);
    } else {
        ESP_LOGE(MQTT_TAG, "Failed to unsubscribe from '%s'", topic.c_str());
    }
    
    return msg_id;
}

MQTT_STATUS MQTTClient::get_status() const {
    return status;
}

bool MQTTClient::is_connected() const {
    return status == MQTT_STATUS::CONNECTED;
}

void MQTTClient::on_message(std::function<void(std::string, std::string)> callback) {
    message_callback = callback;
}

void MQTTClient::on_connected(std::function<void()> callback) {
    connected_callback = callback;
}

void MQTTClient::on_disconnected(std::function<void()> callback) {
    disconnected_callback = callback;
}

void MQTTClient::on_topic(const std::string& topic, std::function<void(std::string, std::string)> callback) {
    topic_callbacks[topic] = callback;
}

void MQTTClient::event_handler(void* handler_args, esp_event_base_t base,
                               int32_t event_id, void* event_data) {
    MQTTClient* self = static_cast<MQTTClient*>(handler_args);
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
    
    switch (static_cast<esp_mqtt_event_id_t>(event_id)) {
        case MQTT_EVENT_CONNECTED:
            self->status = MQTT_STATUS::CONNECTED;
            ESP_LOGI(MQTT_TAG, "Connected to MQTT broker");
            
            if (self->connected_callback) {
                self->connected_callback();
            }
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            self->status = MQTT_STATUS::DISCONNECTED;
            ESP_LOGI(MQTT_TAG, "Disconnected from MQTT broker");
            
            if (self->disconnected_callback) {
                self->disconnected_callback();
            }
            break;
            
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(MQTT_TAG, "Subscription successful (msg_id=%d)", event->msg_id);
            break;
            
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(MQTT_TAG, "Unsubscription successful (msg_id=%d)", event->msg_id);
            break;
            
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(MQTT_TAG, "Message published (msg_id=%d)", event->msg_id);
            break;
            
        case MQTT_EVENT_DATA: {
            std::string topic(event->topic, event->topic_len);
            std::string data(event->data, event->data_len);
            
            ESP_LOGI(MQTT_TAG, "Message received on '%s': %s", topic.c_str(), data.c_str());
            
            // Check for topic-specific callbacks
            auto it = self->topic_callbacks.find(topic);
            if (it != self->topic_callbacks.end()) {
                it->second(topic, data);
            }
            
            // Call general message callback
            if (self->message_callback) {
                self->message_callback(topic, data);
            }
            break;
        }
            
        case MQTT_EVENT_ERROR:
            ESP_LOGE(MQTT_TAG, "MQTT error occurred");
            self->status = MQTT_STATUS::ERROR;
            break;
            
        default:
            break;
    }
}