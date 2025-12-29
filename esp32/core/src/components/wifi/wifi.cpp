#include "wifi/wifi.h"

namespace wifi_module {

    wifi_t::wifi_t(cfg_t &config) : cfg(config), mode_impl(nullptr) {
        if (cfg.mode == cfg::mode_t::sta) {
            mode_impl = new sta_mode_t(cfg);
        } 
        else if (cfg.mode == cfg::mode_t::ap) {
            mode_impl = new ap_mode_t(cfg);
        }
        else if (cfg.mode == cfg::mode_t::sta_ap) {
            mode_impl = new dual_mode_t(cfg);        
        }
    }

    wifi_t::~wifi_t() {
        if (mode_impl) {
            delete mode_impl;
        }
    }

    esp_err_t wifi_t::init() {
        if (mode_impl) {
            return mode_impl->init();
        }
        return ESP_FAIL;
    }

    esp_err_t wifi_t::connect() {
        if (mode_impl) {
            return mode_impl->connect();
        }
        return ESP_FAIL;
    }

    esp_err_t wifi_t::disconnect() {
        if (mode_impl) {
            return mode_impl->disconnect();
        }
        return ESP_FAIL;
    }

}