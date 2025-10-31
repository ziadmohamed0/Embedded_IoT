#ifndef NVS_H_
#define NVS_H_

#include "common.h"

extern const char* NVS_TAG;

class NVS {
    public:
        NVS();
        ~NVS();

        void open(const char* namespace_name, nvs_open_mode_t mode);
        void close();

        template<typename T>
        esp_err_t write(const char* key, const T& value);

        template<typename T>
        T read(const char* key, const T& default_value);

    private:
        void init();
        nvs_handle_t get_handle() const { return handle; }
        nvs_handle_t handle;
};

#include "NVS.tpp"

#endif