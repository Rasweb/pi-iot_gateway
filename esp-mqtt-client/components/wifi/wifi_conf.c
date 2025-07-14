#include "wifi_conf.h"


void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    // Will initialize the LwIP network interface (netif)
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        // Called to connect to the configured AP(Access point)
        esp_wifi_connect();
        
    /* Will happen if:
        - Fails to find the target AP
        - Authenticaiton times out
        - More then one AP with the same SSID
    */
    } else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        if(s_retry_num < ESP_MAXIMUM_RETRY){
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOG(WIfI_TAG, "Retrying connection to AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(WIfI_TAG, "Failed to connect to AP");
    // Will show when the DHCP client successfully gets the IPV4 address or when its changed.
    } else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        // event will point to event_data which points to ip_event_got_ip_t struct
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        // IPSTR ip address format
        // IP2STR macro that converts the ip address into a suitable format.
        ESP_LOGI(WIfI_TAG, "got ip: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } 
}

void wifi_init_sta(){
    s_wifi_event_group = xEventGroupCreate();
}