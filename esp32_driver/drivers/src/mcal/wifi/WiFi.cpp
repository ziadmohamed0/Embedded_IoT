#include "WiFi.h"

const char* WIFI_TAG = "WIFI_MANAGER";

WiFiManager::WiFiManager() 
    : max_retry_num(5), retry_count(0), status(WIFI_STATUS::DISCONNECTED), netif(nullptr) {
    
    // Initialize TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    
    // Create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    ESP_LOGI(WIFI_TAG, "WiFi Manager initialized");
}

WiFiManager::~WiFiManager() {
    disconnect();
    if (netif) {
        esp_netif_destroy(netif);
    }
    esp_event_loop_delete_default();
}

void WiFiManager::init_sta(const std::string& ssid_param, const std::string& password_param, uint8_t max_retry) {
    this->ssid = ssid_param;
    this->password = password_param;
    this->max_retry_num = max_retry;
    
    // Create default WiFi station
    netif = esp_netif_create_default_wifi_sta();
    
    // Initialize WiFi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Register event handlers
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &WiFiManager::event_handler,
        this,
        nullptr));
    
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &WiFiManager::event_handler,
        this,
        nullptr));
    
    // Configure WiFi
    wifi_config_t wifi_config = {};
    std::copy(ssid.begin(), ssid.end(), (char*)wifi_config.sta.ssid);
    std::copy(password.begin(), password.end(), (char*)wifi_config.sta.password);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    ESP_LOGI(WIFI_TAG, "WiFi Station initialized with SSID: %s", ssid.c_str());
}

void WiFiManager::connect() {
    ESP_ERROR_CHECK(esp_wifi_start());
    status = WIFI_STATUS::CONNECTING;
    ESP_LOGI(WIFI_TAG, "Connecting to WiFi...");
}

void WiFiManager::disconnect() {
    if (status != WIFI_STATUS::DISCONNECTED) {
        esp_wifi_disconnect();
        esp_wifi_stop();
        status = WIFI_STATUS::DISCONNECTED;
        ESP_LOGI(WIFI_TAG, "Disconnected from WiFi");
    }
}

WIFI_STATUS WiFiManager::get_status() const {
    return status;
}

bool WiFiManager::is_connected() const {
    return status == WIFI_STATUS::CONNECTED;
}

std::string WiFiManager::get_ip_address() const {
    if (!is_connected() || !netif) {
        return "0.0.0.0";
    }
    
    esp_netif_ip_info_t ip_info;
    if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        char ip_str[16];
        snprintf(ip_str, sizeof(ip_str), IPSTR, IP2STR(&ip_info.ip));
        return std::string(ip_str);
    }
    
    return "0.0.0.0";
}

void WiFiManager::on_connected(std::function<void(std::string)> callback) {
    connected_callback = callback;
}

void WiFiManager::on_disconnected(std::function<void()> callback) {
    disconnected_callback = callback;
}

void WiFiManager::event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data) {
    WiFiManager* self = static_cast<WiFiManager*>(arg);
    
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(WIFI_TAG, "WiFi started, attempting connection...");
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (self->retry_count < self->max_retry_num) {
            esp_wifi_connect();
            self->retry_count++;
            self->status = WIFI_STATUS::CONNECTING;
            ESP_LOGI(WIFI_TAG, "Retry connecting to WiFi (%d/%d)...", 
                    self->retry_count, self->max_retry_num);
        } else {
            self->status = WIFI_STATUS::FAILED;
            ESP_LOGE(WIFI_TAG, "Failed to connect to WiFi after %d attempts", 
                    self->max_retry_num);
            
            if (self->disconnected_callback) {
                self->disconnected_callback();
            }
        }
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        char ip_str[16];
        snprintf(ip_str, sizeof(ip_str), IPSTR, IP2STR(&event->ip_info.ip));
        
        self->status = WIFI_STATUS::CONNECTED;
        self->retry_count = 0;
        
        ESP_LOGI(WIFI_TAG, "Connected! IP Address: %s", ip_str);
        
        if (self->connected_callback) {
            self->connected_callback(std::string(ip_str));
        }
    }
}