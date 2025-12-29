#ifndef INC_HTTP_CLIENT_H_
#define INC_HTTP_CLIENT_H_

#include "common.h"

namespace http_module {
    class client_t {
    public:
        client_t() = default;
        std::string get(const std::string& url);
        esp_err_t post(const std::string& url, const std::string& data);
    private:
        static esp_err_t _event_handler(esp_http_client_event_t *evt);
    };
}
#endif