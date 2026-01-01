#ifndef INC_WIFI_MANAGER_H_
#define INC_WIFI_MANAGER_H_

#include "sta_mode.h"
#include "ap_mode.h"
#include "sta_ap_mode.h"

namespace wifi_module {

    class wifi_t {
    public:
        wifi_t(cfg_t &config);
        ~wifi_t();

        esp_err_t init();
        esp_err_t connect();
        esp_err_t disconnect();

    private:
        cfg_t& cfg;
        Iwifi* mode_impl; 
    };

}

#endif