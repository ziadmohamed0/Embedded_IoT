#ifndef INC_UART_H_
#define INC_UART_H_

#include "common.h"

namespace uart_module {
    namespace cfg {
        enum class uart_index_t : uint8_t {
            uart0, uart1, uart2
        };
    }

    struct cfg_t {
        cfg::uart_index_t index;
        uint32_t baud_rate;
    };

    class uart_t {
        public:
            uart_t(cfg_t &copy_cfg);
            ~uart_t() = default;
            esp_err_t init();
            
            esp_err_t send(const char* str);
            esp_err_t send(const uint8_t* data, size_t len);
        private:
            const char* UART_TAG;
            cfg_t cfg;
            std::array<gpio_num_t, 2> pins;

            QueueHandle_t uart_queue;
            static void uart_handler(void *arg);

            static constexpr int RX_BUF_SIZE = 1024;
            static constexpr int TX_BUF_SIZE = 1024;
            
            uart_port_t port;
    };
}

#endif