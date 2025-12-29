#include "http/http_client.h"

namespace http_module {
    esp_err_t client_t::_event_handler(esp_http_client_event_t *evt) {
        if (evt->event_id == HTTP_EVENT_ON_DATA) {
            std::string *response_buffer = (std::string *)evt->user_data;
            if (response_buffer) {
                response_buffer->append((char*)evt->data, evt->data_len);
            }
        }
        return ESP_OK;
    }

    std::string client_t::get(const std::string& url) {
        std::string response_data;
        esp_http_client_config_t config = {};
        config.url = url.c_str();
        config.event_handler = _event_handler;
        config.user_data = &response_data; // هنخزن الرد هنا

        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_err_t err = esp_http_client_perform(client);
        
        if (err != ESP_OK) ESP_LOGE("HTTP_CLIENT", "GET request failed");
        
        esp_http_client_cleanup(client);
        return response_data;
    }

    esp_err_t client_t::post(const std::string& url, const std::string& data) {
        esp_http_client_config_t config = {};
        config.url = url.c_str();
        config.method = HTTP_METHOD_POST;

        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_http_client_set_post_field(client, data.c_str(), data.length());
        esp_http_client_set_header(client, "Content-Type", "application/json");

        esp_err_t err = esp_http_client_perform(client);
        esp_http_client_cleanup(client);
        return err;
    }
}