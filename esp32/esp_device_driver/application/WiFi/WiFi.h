/**
 * @file WiFi.h
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-19
 */

#ifndef _APPLICATION_WIFI_H_
#define _APPLICATION_WIFI_H_

#include "esp_wifi.h"
#include <mutex>
#include "esp_mac.h"

namespace WiFi{
    class WiFi {
        private:
            esp_err_t _get_mac(void);
            static char mac_add_cstr[13];
            static std::mutex firstCallMutex;
            static bool firstCall;
        public:
            enum class state_e {
                NOT_INITIALISED,
                INITIALISED,
                WAITING_FOR_CREDENTIALS,
                READY_TO_CONNECT,
                CONNECTING,
                WAITING_FOR_IP,
                CONNECTED,
                DISCONNECTED,
                ERROR
            };

            WiFi(void) {
                std::lock_guard<std::mutex> guard(firstCallMutex);
                if(!firstCall){
                    if(ESP_OK != _get_mac()) {
                        esp_restart();
                    }
                    firstCall = true;
                }
            }
            esp_err_t init(void);
            const char* getMac(void);
    };
}


#endif