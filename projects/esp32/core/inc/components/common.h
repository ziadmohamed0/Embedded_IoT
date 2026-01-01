#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include <iostream>
#include <map>
#include <functional>
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <cmath>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/uart.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
#include "driver/i2c_master.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
extern "C" {
    #include "mqtt_client.h"
}

#endif