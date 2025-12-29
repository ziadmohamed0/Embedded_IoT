#ifndef INC_DUAL_MODE_H_
#define INC_DUAL_MODE_H_

#include "Iwifi.h"

namespace wifi_module {

    class dual_mode_t : public Iwifi {
    public:
        dual_mode_t(cfg_t &copy_cfg);
        
        esp_err_t init() override;
        esp_err_t connect() override; 
        esp_err_t disconnect() override;

    private:
        cfg_t& cfg;
        uint8_t retry_connect;
        static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
    };

} // namespace wifi_module

#endif