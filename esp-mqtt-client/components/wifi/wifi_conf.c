#include "wifi_conf.h"
char* TAG_WIFI = "Wifi station";
int static s_retry_num;
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;


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
            ESP_LOGI(TAG_WIFI, "Retrying connection to AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG_WIFI, "Failed to connect to AP");
    // Will show when the DHCP client successfully gets the IPV4 address or when its changed.
    } else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        // event will point to event_data which points to ip_event_got_ip_t struct
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        // IPSTR ip address format
        // IP2STR macro that converts the ip address into a suitable format.
        ESP_LOGI(TAG_WIFI, "got ip: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } 
}

void wifi_init_sta(){
    s_wifi_event_group = xEventGroupCreate();
    if(s_wifi_event_group == NULL){
        ESP_LOGE(TAG_WIFI, "Failed to create event group");
        return;
    }

    // Will initialize the network interface management, to manage network interfaces.
    // Necessary for enabling network connectivity.
    ESP_ERROR_CHECK(esp_netif_init());

    // Sets up the event handling system.
    // Handling asynchronous events.
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG_WIFI, "wifi_init_sta finished.");

    // Checking if either the connection is established or connection failed for maximum of re-tries.
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                            pdFALSE,
                            pdFALSE,
                            portMAX_DELAY);

    // xEventGroupWaitBits() returns bits before call returned.
    // bitwise AND operation - checks if its set (e.g if its 1)
    if(bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG_WIFI, "connected to ap SSID:%s", WIFI_SSID);     
    // bitwise AND operation - checks if its set (e.g if its 1)
    } else if (bits & WIFI_FAIL_BIT){
        ESP_LOGI(TAG_WIFI, "Failed to connect to SSID:%s", WIFI_SSID);
    } else {
        ESP_LOGE(TAG_WIFI, "Unexpected event");
    }
}

void wifi_main(){
    // Used to store data that must persist across device resets.
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG_WIFI, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
}