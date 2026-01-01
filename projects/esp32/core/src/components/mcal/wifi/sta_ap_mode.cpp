#include "wifi/sta_ap_mode.h"

constexpr uint8_t WIFI_MAX_RETRY = 5;
constexpr uint8_t WIFI_CONNECT_BITS = BIT0;
constexpr uint8_t WIFI_FAIL_BIT = BIT1;

namespace wifi_module {

dual_mode_t::dual_mode_t(cfg_t &copy_cfg) : cfg(copy_cfg), retry_connect(0) {
    this->WIFI_TAG = "WIFI_DUAL";
    this->s_wifi_event_group = nullptr;
}

esp_err_t dual_mode_t::init() {
    this->s_wifi_event_group = xEventGroupCreate();
    
    ESP_ERROR_CHECK(esp_netif_init());
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_ERROR_CHECK(err);
    }

    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &dual_mode_t::wifi_event_handler, this, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &dual_mode_t::wifi_event_handler, this, nullptr));

    wifi_config_t wifi_config_sta = {};
    strlcpy((char*)wifi_config_sta.sta.ssid, cfg.ssid.c_str(), sizeof(wifi_config_sta.sta.ssid));
    strlcpy((char*)wifi_config_sta.sta.password, cfg.password.c_str(), sizeof(wifi_config_sta.sta.password));

    wifi_config_t wifi_config_ap = {};
    std::string ap_ssid = cfg.ssid + "_Config";
    strlcpy((char*)wifi_config_ap.ap.ssid, ap_ssid.c_str(), sizeof(wifi_config_ap.ap.ssid));
    strlcpy((char*)wifi_config_ap.ap.password, "12345678", sizeof(wifi_config_ap.ap.password));
    wifi_config_ap.ap.channel = 1;
    wifi_config_ap.ap.max_connection = 4;
    wifi_config_ap.ap.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sta));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_ap));
    
    ESP_LOGI(this->WIFI_TAG, "Dual Mode Initialized. AP SSID: %s", ap_ssid.c_str());
    ESP_ERROR_CHECK(esp_wifi_start());
    
    return ESP_OK;
}

esp_err_t dual_mode_t::connect() {
    this->init();

    EventBits_t bits = xEventGroupWaitBits(this->s_wifi_event_group,
                WIFI_CONNECT_BITS | WIFI_FAIL_BIT, 
                pdFALSE,
                pdFALSE,
                portMAX_DELAY);

    if (bits & WIFI_CONNECT_BITS) {
        ESP_LOGI(this->WIFI_TAG, "STA Connected successfully.");
        return ESP_OK;
    } else {
        ESP_LOGE(this->WIFI_TAG, "STA Connection failed.");
        return ESP_FAIL;
    }
}

esp_err_t dual_mode_t::disconnect() {
    esp_wifi_stop();
    esp_wifi_deinit();
    if (this->s_wifi_event_group) {
        vEventGroupDelete(this->s_wifi_event_group);
        this->s_wifi_event_group = nullptr;
    }
    return ESP_OK;
}

void dual_mode_t::wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    dual_mode_t* obj = static_cast<dual_mode_t*>(arg);

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI("WIFI_DUAL", "STA connecting...");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (obj->retry_connect < WIFI_MAX_RETRY) {
            esp_wifi_connect();
            obj->retry_connect++;
            ESP_LOGI("WIFI_DUAL", "Retry STA connection %d/%d...", obj->retry_connect, WIFI_MAX_RETRY);
        } else {
            xEventGroupSetBits(obj->s_wifi_event_group, WIFI_FAIL_BIT);
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("WIFI_DUAL", "STA Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        obj->retry_connect = 0;
        xEventGroupSetBits(obj->s_wifi_event_group, WIFI_CONNECT_BITS);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI("WIFI_DUAL", "Device joined AP: " MACSTR, MAC2STR(event->mac));
    }
}

} // namespace wifi_module