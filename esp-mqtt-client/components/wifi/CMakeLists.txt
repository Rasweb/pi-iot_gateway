idf_component_register(SRCS "wifi_conf.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_event esp_wifi nvs_flash)