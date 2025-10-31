#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

#include "common.h"
#include <functional>
#include <map>

extern const char* MQTT_TAG;

enum class MQTT_STATUS : uint8_t {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    ERROR
};

struct MQTTConfig {
    std::string broker_uri;
    std::string client_id;
    std::string username;
    std::string password;
    uint16_t port;
    uint16_t keepalive;
    
    MQTTConfig() : port(1883), keepalive(120) {}
};

class MQTTClient {
public:
    MQTTClient();
    ~MQTTClient();
    
    /**
     * @brief Initialize MQTT client with configuration
     * @param config MQTT configuration structure
     */
    void init(const MQTTConfig& config);
    
    /**
     * @brief Start MQTT client connection
     */
    void connect();
    
    /**
     * @brief Disconnect from MQTT broker
     */
    void disconnect();
    
    /**
     * @brief Publish message to topic
     * @param topic Topic to publish to
     * @param data Message data
     * @param qos Quality of Service (0, 1, or 2)
     * @param retain Retain flag
     * @return Message ID or -1 on error
     */
    int publish(const std::string& topic, const std::string& data, int qos = 0, bool retain = false);
    
    /**
     * @brief Subscribe to topic
     * @param topic Topic to subscribe to
     * @param qos Quality of Service (0, 1, or 2)
     * @return Message ID or -1 on error
     */
    int subscribe(const std::string& topic, int qos = 0);
    
    /**
     * @brief Unsubscribe from topic
     * @param topic Topic to unsubscribe from
     * @return Message ID or -1 on error
     */
    int unsubscribe(const std::string& topic);
    
    /**
     * @brief Get current MQTT status
     */
    MQTT_STATUS get_status() const;
    
    /**
     * @brief Check if connected to broker
     */
    bool is_connected() const;
    
    /**
     * @brief Set callback for incoming messages
     * @param callback Function(topic, data)
     */
    void on_message(std::function<void(std::string, std::string)> callback);
    
    /**
     * @brief Set callback for connection events
     */
    void on_connected(std::function<void()> callback);
    
    /**
     * @brief Set callback for disconnection events
     */
    void on_disconnected(std::function<void()> callback);
    
    /**
     * @brief Set callback for specific topic
     * @param topic Topic to listen for
     * @param callback Function(topic, data)
     */
    void on_topic(const std::string& topic, std::function<void(std::string, std::string)> callback);

private:
    static void event_handler(void* handler_args, esp_event_base_t base,
                            int32_t event_id, void* event_data);
    
    esp_mqtt_client_handle_t client;
    MQTT_STATUS status;
    MQTTConfig config;
    
    std::function<void(std::string, std::string)> message_callback;
    std::function<void()> connected_callback;
    std::function<void()> disconnected_callback;
    std::map<std::string, std::function<void(std::string, std::string)>> topic_callbacks;
};

#endif // MQTT_CLIENT_H_