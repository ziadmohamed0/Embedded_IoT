#ifndef WIFI_MANAGER_H_
#define WIFI_MANAGER_H_

#include "common.h"
#include <functional>

extern const char* WIFI_TAG;

enum class WIFI_STATUS : uint8_t {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    FAILED
};

class WiFiManager {
public:
    WiFiManager();
    ~WiFiManager();
    
    /**
     * @brief Initialize WiFi in Station mode
     * @param ssid WiFi network name
     * @param password WiFi password
     * @param max_retry Maximum connection retry attempts (default: 5)
     */
    void init_sta(const std::string& ssid, const std::string& password, uint8_t max_retry = 5);
    
    /**
     * @brief Start WiFi connection
     */
    void connect();
    
    /**
     * @brief Disconnect from WiFi
     */
    void disconnect();
    
    /**
     * @brief Get current WiFi status
     */
    WIFI_STATUS get_status() const;
    
    /**
     * @brief Check if connected to WiFi
     */
    bool is_connected() const;
    
    /**
     * @brief Get IP address as string
     */
    std::string get_ip_address() const;
    
    /**
     * @brief Set callback for connection events
     * @param callback Function to call when WiFi connects (receives IP as string)
     */
    void on_connected(std::function<void(std::string)> callback);
    
    /**
     * @brief Set callback for disconnection events
     */
    void on_disconnected(std::function<void()> callback);

private:
    static void event_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data);
    
    std::string ssid;
    std::string password;
    uint8_t max_retry_num;
    uint8_t retry_count;
    WIFI_STATUS status;
    esp_netif_t* netif;
    
    std::function<void(std::string)> connected_callback;
    std::function<void()> disconnected_callback;
};

#endif // WIFI_MANAGER_H_