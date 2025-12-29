#include "uart/uart.h"

namespace uart_module {
    uart_t::uart_t(cfg_t &copy_cfg) : UART_TAG("UART_TAG"), cfg(copy_cfg) {
        this->port = static_cast<uart_port_t>(this->cfg.index);
        switch (this->port) {
            case UART_NUM_0:
                this->pins[0] = GPIO_NUM_1;  // TX0
                this->pins[1] = GPIO_NUM_3;  // RX0
                break;

            case UART_NUM_1:
                this->pins[0] = GPIO_NUM_10; // TX1 
                this->pins[1] = GPIO_NUM_9;  // RX1
                break;

            case UART_NUM_2:
                this->pins[0] = GPIO_NUM_17; // TX2
                this->pins[1] = GPIO_NUM_16; // RX2
                break;

            default:
                ESP_LOGW(UART_TAG, "Invalid UART port! Using default pins for UART0");
                this->pins[0] = GPIO_NUM_1;
                this->pins[1] = GPIO_NUM_3;
                break;
        }
        
        ESP_LOGI(UART_TAG, "UART %d initialized with TX:%d, RX:%d", 
                this->port, this->pins[0], this->pins[1]);
        this->init();
    }

    esp_err_t uart_t::init() {
        uart_config_t uart_config = {}; 
            
            uart_config.baud_rate = (int)cfg.baud_rate;
            uart_config.data_bits = UART_DATA_8_BITS;
            uart_config.parity    = UART_PARITY_DISABLE;
            uart_config.stop_bits = UART_STOP_BITS_1;
            uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
            uart_config.source_clk = UART_SCLK_DEFAULT;

            esp_err_t ret = uart_driver_install(port, RX_BUF_SIZE * 2, TX_BUF_SIZE * 2, 20, &this->uart_queue, 0);
            if (ret != ESP_OK) return ret;

            ret = uart_param_config(port, &uart_config);
            if (ret != ESP_OK) return ret;

            ret = uart_set_pin(port, pins[0], pins[1], UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
            if (ret != ESP_OK) return ret;

            xTaskCreate(uart_t::uart_handler, "uart_task", 4096, this, 12, nullptr);

            return ESP_OK;
    }


    esp_err_t uart_t::send(const char* str) {
        if (str == nullptr) 
            return ESP_ERR_INVALID_ARG;
        int len = strlen(str);
        return (uart_write_bytes(port, str, len) == len) ? ESP_OK : ESP_FAIL;
    }

    esp_err_t uart_t::send(const uint8_t* data, size_t len) {
        if (data == nullptr || len == 0) 
            return ESP_ERR_INVALID_ARG;
        return (uart_write_bytes(port, data, len) == len) ? ESP_OK : ESP_FAIL;
    }


    void uart_t::uart_handler(void* arg) {
        uart_event_t event;
        uart_t* obj = static_cast<uart_t*>(arg);
        uint8_t* dtmp = (uint8_t*)malloc(RX_BUF_SIZE);
        
        if(dtmp == nullptr) {
            ESP_LOGE("UART_TAG", "Failed to allocate memory");
            vTaskDelete(nullptr);
            return;
        }

        for(;;) {
            if (xQueueReceive(obj->uart_queue, (void*)&event, (TickType_t)portMAX_DELAY)) {
                bzero(dtmp, RX_BUF_SIZE);
                
                switch (event.type) {
                    case UART_DATA:{
                        size_t length = 0;
                        ESP_ERROR_CHECK(uart_get_buffered_data_len(obj->port, (size_t*)&length));
                        int rx_len = uart_read_bytes(obj->port, dtmp, length, portMAX_DELAY);

                        if (rx_len > 0) {
                            ESP_LOGI("UART_TAG", "As String: %.*s", rx_len, dtmp);

                            printf("As Hex: ");
                            for (int i = 0; i < rx_len; i++) {
                                printf("%02X ", dtmp[i]); 
                            }
                            printf("\n");
                        }
                        break;
                    }
                    case UART_FIFO_OVF:
                        uart_flush_input(obj->port);
                        xQueueReset(obj->uart_queue);
                        break;

                    default:
                        break;
                }
            }
        }
        free(dtmp);
        vTaskDelete(nullptr);
    }
}