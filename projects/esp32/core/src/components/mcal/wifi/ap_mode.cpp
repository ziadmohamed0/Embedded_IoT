#include "wifi/ap_mode.h"

namespace wifi_module {

ap_mode_t::ap_mode_t(cfg_t &copy_cfg) : cfg(copy_cfg) {
    this->WIFI_TAG = "WIFI_AP";
}

esp_err_t ap_mode_t::init() {
    ESP_ERROR_CHECK(esp_netif_init());
    
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_ERROR_CHECK(err);
    }

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    wifi_config_t wifi_config = {};
    strlcpy((char*)wifi_config.ap.ssid, cfg.ssid.c_str(), sizeof(wifi_config.ap.ssid));
    strlcpy((char*)wifi_config.ap.password, cfg.password.c_str(), sizeof(wifi_config.ap.password));
    wifi_config.ap.ssid_len = strlen(cfg.ssid.c_str());
    wifi_config.ap.channel = 1;
    wifi_config.ap.max_connection = 4;
    wifi_config.ap.authmode = (cfg.password.empty()) ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    
    ESP_LOGI(this->WIFI_TAG, "AP Mode Started. SSID: %s", cfg.ssid.c_str());
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

} // namespace wifi_module