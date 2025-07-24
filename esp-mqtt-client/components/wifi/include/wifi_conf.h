#pragma once
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "config.h"


#define WIFI_SSID WIFI_NAME
#define WIFI_PASS WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY 5
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK



/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */

// 1 << 0 which equals to 1 in decimal - binary:
#define WIFI_CONNECTED_BIT BIT0
// 1 << 1 which equals to 2 in decimal - binary: 00000010
#define WIFI_FAIL_BIT BIT1

extern char *TAG_WIFI;


void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_sta();
void wifi_main();