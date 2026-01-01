#ifndef INC_AP_MODE_H_
#define INC_AP_MODE_H_

#include "Iwifi.h"

namespace wifi_module {

    class ap_mode_t : public Iwifi {
    public:
        ap_mode_t(cfg_t &copy_cfg);
        
        esp_err_t init() override;
        
    private:
        cfg_t& cfg;
    };

} // namespace wifi_module

#endif