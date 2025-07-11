#include <stdio.h>
#include <mqtt_client.h>
#include "config.h"
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

// https://docs.espressif.com/projects/esp-idf/en/v5.4.2/esp32c6/api-reference/protocols/mqtt.html 
const char *TAG = "MQTT5";

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id){
    // Established connection and is ready to send and receive data.
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Established connection");
        // TODO - Implement esp_mqtt5_client_set_user_property() for advanced mqtt features
        msg_id = esp_mqtt_client_publish(client, "/topic/test1", "test_1", 0, 1, 1);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        
        msg_id = esp_mqtt_client_subscribe(client, "/topic/test0",0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        
        msg_id = esp_mqtt_client_subscribe(client, "/topic/test1",2);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/test0");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    // Unable to read or write data, server is unavailable.
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Disconnected");
        break;
    // Broker has acknowledged the client's subscribe request.
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "Subscribed, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "topic/test0", "test_0", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    // Broker has acknowledged the client's unsubscribe request
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "Unsubscribed, msg_id=%d", event->msg_id);
        esp_mqtt_client_disconnect(client);
        break;
    // Broker has acknowledged the client's publish request
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "Published, msg_id=%d", event->msg_id);
        break;
    // Client has received a publish message.
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Received publish message");
        ESP_LOGI(TAG, "TOPIC=%.*s", event->topic_len, event->topic);
        ESP_LOGI(TAG, "DATA=%.*s", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "Error");
        ESP_LOGI(TAG, "MQTT5 return code is %d", event->error_handle->connect_return_code);
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt5_app_start(){
char buffer[100];
    // Strings with custom formats
    sprintf(buffer, "mqtt://%s:%s@%s", MQTT_USERNAME, MQTT_PASSWORD, BROKER_IP);
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = buffer,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = true,
        .session.last_will.topic = "/topic/will",
        .session.last_will.msg= "I will leave",
        .session.last_will.msg_len = 13,
        .session.last_will.qos = 1,
        .session.last_will.retain = true,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

}

void app_main(void)
{    
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());


    // Used to store data that must persist across device resets.
    ESP_ERROR_CHECK(nvs_flash_init());
    // Necessary for enabling network connectivity.
    ESP_ERROR_CHECK(esp_netif_init());
    // Handling asynchronous events.
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // TODO - fix, in example its from the file protocol_examples_common.h
    // ESP_ERROR_CHECK(example_connect());
    ESP_ERROR_CHECK(example_connect());

    mqtt5_app_start();

    vTaskDelay(pdMS_TO_TICKS(10));
}