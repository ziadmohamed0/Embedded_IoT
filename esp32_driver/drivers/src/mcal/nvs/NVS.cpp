#include "NVS.h"

const char* NVS_TAG = "NVS_DRIVER";

NVS::NVS() : handle(0) {
    init();
}

void NVS::init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void NVS::open(const char* namespace_name, nvs_open_mode_t mode) {
    esp_err_t err = nvs_open(namespace_name, mode, &handle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    if (err == ESP_OK)
        ESP_LOGI(NVS_TAG, "Opened NVS namespace: %s", namespace_name);
    else
        ESP_LOGE(NVS_TAG, "Failed to open NVS: %s", esp_err_to_name(err));
}

void NVS::close() {
    if (handle) {
        nvs_close(handle);
        handle = 0;
        ESP_LOGI(NVS_TAG, "NVS handle closed");
    }
}

NVS::~NVS() {
    close();
}