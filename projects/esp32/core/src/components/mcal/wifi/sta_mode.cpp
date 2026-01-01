#include "wifi/sta_mode.h"

constexpr uint8_t WIFI_MAX_RETRY = 5;
constexpr uint8_t WIFI_CONNECT_BITS = BIT0;
constexpr uint8_t WIFI_FAIL_BIT = BIT1;

namespace wifi_module {

sta_mode_t::sta_mode_t(cfg_t &copy_cfg) : cfg(copy_cfg), retry_connect(0) {
    this->WIFI_TAG = "WIFI_STA";
    this->s_wifi_event_group = nullptr;
}

esp_err_t sta_mode_t::init() {
    this->s_wifi_event_group = xEventGroupCreate();
    
    ESP_ERROR_CHECK(esp_netif_init());
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_ERROR_CHECK(err);
    }

    esp_netif_t* sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &sta_mode_t::wifi_event_handler, this, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &sta_mode_t::wifi_event_handler, this, nullptr));

    wifi_config_t wifi_config = {};
    strlcpy((char*)wifi_config.sta.ssid, cfg.ssid.c_str(), sizeof(wifi_config.sta.ssid));
    strlcpy((char*)wifi_config.sta.password, cfg.password.c_str(), sizeof(wifi_config.sta.password));
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    ESP_LOGI(this->WIFI_TAG, "Initialized with SSID: %s", cfg.ssid.c_str());
    ESP_ERROR_CHECK(esp_wifi_start());
    return ESP_OK;        
}

esp_err_t sta_mode_t::connect() {
    this->init();

    EventBits_t bits = xEventGroupWaitBits(this->s_wifi_event_group,
                WIFI_CONNECT_BITS | WIFI_FAIL_BIT, 
                pdFALSE,
                pdFALSE,
                portMAX_DELAY);

    if (bits & WIFI_CONNECT_BITS) {
        ESP_LOGI(this->WIFI_TAG, "Connected successfully.");
        return ESP_OK;
    } else {
        ESP_LOGE(this->WIFI_TAG, "Failed to connect.");
        return ESP_FAIL;
    }
}

esp_err_t sta_mode_t::disconnect() {
    esp_wifi_stop();
    esp_wifi_deinit();
    if (this->s_wifi_event_group) {
        vEventGroupDelete(this->s_wifi_event_group);
        this->s_wifi_event_group = nullptr;
    }
    return ESP_OK;        
}

void sta_mode_t::wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    sta_mode_t* obj = static_cast<sta_mode_t*>(arg);

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (obj->retry_connect < WIFI_MAX_RETRY) {
            esp_wifi_connect();
            obj->retry_connect++;
            ESP_LOGI("WIFI_STA", "Retry %d/%d...", obj->retry_connect, WIFI_MAX_RETRY);
        } else {
            xEventGroupSetBits(obj->s_wifi_event_group, WIFI_FAIL_BIT);
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("WIFI_STA", "IP: " IPSTR, IP2STR(&event->ip_info.ip));
        obj->retry_connect = 0;
        xEventGroupSetBits(obj->s_wifi_event_group, WIFI_CONNECT_BITS);
    }
}

} // namespace wifi_module