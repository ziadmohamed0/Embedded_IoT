#ifndef UART_H_
#define UART_H_

#include "common.h"
#include <vector>

extern const char* UART_TAG; 
constexpr uint16_t buffer_size = 1024;


enum class uart_index_t {
    UART0, UART1, UART2
};

enum class parity_t {
    NON, EVEN, ODD
};

enum class buad_rate_t{
    _115200, _9600
};

struct uart_device_t {
    uart_index_t uart_index;
    buad_rate_t buad_rate;
    parity_t parity;
    uint8_t data_bits;
    char* buffer[buffer_size];
    std::vector<gpio_num_t> pins;
};

class uart {
    public:
        uart(const uart_device_t &cfg);
        void write();
        void read();
        ~uart() = default;
    private:
        uart_device_t uart_cfg;
};

#endif