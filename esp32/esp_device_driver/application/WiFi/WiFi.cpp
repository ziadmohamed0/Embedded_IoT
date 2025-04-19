/**
 * @file WiFi.cpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-20
 */

#include "WiFi.h"

char WiFi::WiFi::mac_add_cstr[13]{};    
bool WiFi::WiFi::firstCall = false;
std::mutex WiFi::WiFi::firstCallMutex;

esp_err_t WiFi::WiFi::_get_mac(void) {
    uint8_t mac_byte_buffer[13]{};
    esp_err_t state = esp_read_mac(mac_byte_buffer, ESP_MAC_WIFI_STA);
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
