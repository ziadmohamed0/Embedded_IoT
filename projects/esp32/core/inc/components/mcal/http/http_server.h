#ifndef INC_HTTP_SERVER_H_
#define INC_HTTP_SERVER_H_

#include "common.h" 

namespace http_module {

    class server_t {
    public:
        server_t() = default;
        ~server_t();

        esp_err_t start();
        void stop();
        esp_err_t add_route(const char* uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *r));

    private:
        httpd_handle_t server_handle = nullptr;
    };

} // namespace http_module

#endif