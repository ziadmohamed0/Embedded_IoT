#ifndef INC_IWIFI_H_
#define INC_IWIFI_H_

#include "common.h"

namespace wifi_module {
    namespace cfg {
        enum class mode_t : uint8_t{
            sta, ap, sta_ap
        };
    }

    struct cfg_t {
        std::string ssid;
        std::string password;
        cfg::mode_t mode;
    };

    class Iwifi {
        public:
            virtual ~Iwifi() = default;
            virtual esp_err_t init() = 0;
            virtual esp_err_t connect() { return ESP_OK; }
            virtual esp_err_t disconnect() { return ESP_OK; }
        protected:
            const char* WIFI_TAG;
            EventGroupHandle_t s_wifi_event_group;
    };
}
#endif