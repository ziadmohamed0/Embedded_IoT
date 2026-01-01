#include "nvs/nvs.h"

namespace nvs_module {
    nvs_t::nvs_t() : NVS_TAG("NVS_MODULE") {
        //01. init nvs with cover logic
        esp_err_t err = nvs_flash_init(); 
        if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(err);
    }

    esp_err_t nvs_t::open(const char* namespace_name, nvs_open_mode_t mode) {
        esp_err_t err = nvs_open(namespace_name, mode, &this->handle);
        ESP_ERROR_CHECK_WITHOUT_ABORT(err);
        if(err == ESP_OK){
            ESP_LOGI(this->NVS_TAG, "opened NVS namespace: %s", namespace_name);
            return ESP_OK;
        }
        else {
            ESP_LOGE(this->NVS_TAG, "Failed to open NVS: %s", esp_err_to_name(err));
            return ESP_FAIL;
        }
    }

    esp_err_t nvs_t::close() {
        if (handle) {
            nvs_close(handle);
            handle = 0;
            ESP_LOGI(NVS_TAG, "NVS handle closed");
            return ESP_OK;
        }
        else {
            ESP_LOGE(this->NVS_TAG, "Failed to close NVS");
            return ESP_FAIL; 
        }
    }

    esp_err_t nvs_t::write_str(const char*key, const std::string &str) {
        esp_err_t err;
        auto h = this->handle;
        err = nvs_set_str(h, key, str.c_str());
        if (err != ESP_OK) {
            ESP_LOGE(this->NVS_TAG, "Failed to set string! Error: %s", esp_err_to_name(err));
            return err;
        }

        err = nvs_commit(h);
        if (err != ESP_OK) {
            ESP_LOGE(this->NVS_TAG, "Failed to commit changes! Error: %s", esp_err_to_name(err));
            return err;
        }

        return ESP_OK;
    }

    esp_err_t nvs_t::write_num(const char*key, const uint32_t num) {
        esp_err_t err;
        auto h = this->handle;
        err = nvs_set_u32(h, key, num);
        if (err != ESP_OK) {
            ESP_LOGE(this->NVS_TAG, "Failed to set number! Error: %s", esp_err_to_name(err));
            return err;
        }

        err = nvs_commit(h);
        if (err != ESP_OK) {
            ESP_LOGE(this->NVS_TAG, "Failed to commit changes! Error: %s", esp_err_to_name(err));
            return err;
        }
        
        return ESP_OK;
    }
                    
    esp_err_t nvs_t::write_float(const char* key, float num) {
        uint32_t temp_val;
        memcpy(&temp_val, &num, sizeof(float));
        return write_num(key, temp_val);
    }
    
    esp_err_t nvs_t::read_str(const char* key, std::string &str) {
        size_t required_size = 0; 
        auto h = this->handle;

        esp_err_t err = nvs_get_str(h, key, NULL, &required_size);
            
        if (err == ESP_OK) {
            char* buffer = new char[required_size];
            err = nvs_get_str(h, key, buffer, &required_size);
            if (err == ESP_OK) {
                str = std::string(buffer); 
            }
            delete[] buffer; 
        }

        if (err == ESP_ERR_NVS_NOT_FOUND)
            ESP_LOGW(this->NVS_TAG, "Key '%s' not found", key);
        else if (err != ESP_OK)
            ESP_LOGE(this->NVS_TAG, "Read key '%s' failed (%s)", key, esp_err_to_name(err));

        return err;
    }

    esp_err_t nvs_t::read_num(const char* key, uint32_t num) { 
        auto h = this->handle;
        esp_err_t err = nvs_get_u32(h, key, &num); 

        if (err == ESP_ERR_NVS_NOT_FOUND)
            ESP_LOGW(this->NVS_TAG, "Key '%s' not found", key);
        else if (err != ESP_OK)
            ESP_LOGE(this->NVS_TAG, "Read key '%s' failed (%s)", key, esp_err_to_name(err));

        return err;
    }

    esp_err_t nvs_t::read_float(const char* key, float &num) {
        uint32_t temp_val = 0;
        esp_err_t err = read_num(key, temp_val);
        if (err == ESP_OK) {
            memcpy(&num, &temp_val, sizeof(float));
        }

        return err;
    }
}