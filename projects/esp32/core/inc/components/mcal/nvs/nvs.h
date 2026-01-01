#ifndef INC_NVS_H_
#define INC_NVS_H_

#include "common.h"

namespace nvs_module {
    class nvs_t {
        public:
            nvs_t();
            ~nvs_t() = default;
            esp_err_t open(const char* namespace_name, nvs_open_mode_t mode);
            esp_err_t close();

            esp_err_t write_str(const char*key, const std::string &str);
            esp_err_t write_num(const char*key, const uint32_t num);
            esp_err_t write_float(const char* key, float num);
            
            esp_err_t read_str(const char*key, std::string &str);
            esp_err_t read_num(const char*key, const uint32_t num);
            esp_err_t read_float(const char* key, float &num);

        private:
            nvs_handle_t handle;
            nvs_handle_t get_handle(){return this->handle;}

            const char* NVS_TAG;
    };
}

#endif