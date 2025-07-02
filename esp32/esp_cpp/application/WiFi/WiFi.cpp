#include "WiFi.h"

namespace WiFi {
    char WiFi::mac_add_cstr[13] = {0};
    std::mutex WiFi::firstCall_mutex;
    bool WiFi::firstCall = false;

    esp_err_t WiFi::_getMac(void) {
        uint8_t mac_byte_buffer[6]{};
        const esp_err_t state = esp_read_mac(mac_byte_buffer, ESP_MAC_WIFI_STA);
        
        if(ESP_OK == state) {
            snprintf(mac_add_cstr, sizeof(mac_add_cstr), "%02X%02X%02X%02X%02X%02X", 
                mac_byte_buffer[0],
                mac_byte_buffer[1],
                mac_byte_buffer[2],
                mac_byte_buffer[3],
                mac_byte_buffer[4],
                mac_byte_buffer[5]);
        }
        return state;
    }
}