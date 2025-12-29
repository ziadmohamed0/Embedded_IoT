#include "http/http_server.h"

namespace http_module {
    esp_err_t server_t::start() {
        if (server_handle != nullptr) return ESP_OK; 

        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.stack_size = 8192; 

        esp_err_t ret = httpd_start(&server_handle, &config);
        if (ret == ESP_OK) {
            ESP_LOGI("HTTP_SERVER", "Server started on port: '%d'", config.server_port);
        }
        return ret;
    }

    void server_t::stop() {
        if (server_handle) {
            httpd_stop(server_handle);
            server_handle = nullptr;
        }
    }

    esp_err_t server_t::add_route(const char* uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *r)) {
        if (server_handle == nullptr) return ESP_ERR_INVALID_STATE;

        httpd_uri_t route_config = {
            .uri      = uri,
            .method   = method,
            .handler  = handler,
            .user_ctx = nullptr
        };

        return httpd_register_uri_handler(server_handle, &route_config);
    }
}