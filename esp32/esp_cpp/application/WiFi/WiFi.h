#ifndef _APPLICATION_WIFI_H_
#define _APPLICATION_WIFI_H_

#include "esp_wifi.h"
#include "esp_mac.h"  // For esp_read_mac()
#include <mutex>

namespace WiFi {
    class WiFi {
    private:
        void stateMachine(void);
        static char mac_add_cstr[13];
        esp_err_t _getMac(void);
        static std::mutex firstCall_mutex;
        static bool firstCall;
        
    public:
        WiFi(void) {
            std::lock_guard<std::mutex> guard(firstCall_mutex);
            if(!firstCall) {
                if(ESP_OK != _getMac()) esp_restart();
                firstCall = true;
            }                 
        }

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

        esp_err_t init(void);
        esp_err_t begin(void);
        state_e getState(void);
        const char* getMac(void) { return mac_add_cstr; }
    };
}

#endif