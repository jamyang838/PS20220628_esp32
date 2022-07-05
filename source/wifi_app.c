#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "wifi_app.h"
// #include "esp_wpa2.h"
// #include "esp_system.h"
// #include "esp_event.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_netif.h"

static char *TAG = "Wife";
static EventGroupHandle_t wifi_event_group;
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_ERROR_CHECK(esp_wifi_connect());
        ESP_LOGI(TAG, "WIFI_EVENT_STA_START");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}
void wifi_reconnect(char *ssid, char *password)
{
    // esp_wifi_disconnect();
    wifi_config_t wifi_cfg_ = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK}};
            memset(wifi_cfg_.sta.ssid,0,32);
            memset(wifi_cfg_.sta.password,0,64);
    strcpy((char *)wifi_cfg_.sta.ssid, ssid);
    strcpy((char *)wifi_cfg_.sta.password, password);
    // ESP_LOGI(TAG, "%s", wifi_cfg_.sta.ssid);
    // ESP_LOGI(TAG, "%s",wifi_cfg_.sta.password);
    ESP_LOGI(TAG, "%s", ssid);
    ESP_LOGI(TAG, "%s",password);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg_));
    // esp_wifi_connect();
    ESP_ERROR_CHECK(esp_wifi_disconnect() );
    ESP_ERROR_CHECK(esp_wifi_stop() );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static char ssid[32];
static char password[64];
static QueueHandle_t wifi_app_queue_handle;
static wifi_app_queue_message_t m;
void wifi_connect_event(wifi_app_message_e event_type, char *content)
{
    m.msgID = event_type;
    strcpy(m.content, content);
    ESP_LOGI("yang", "m.content: %s", m.content);
    xQueueSend(wifi_app_queue_handle, &m, 10 / portTICK_PERIOD_MS);
}
void wifi_app_start(void *arg)
{
    wifi_app_queue_message_t msg;

    ESP_ERROR_CHECK(esp_netif_init());
    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // strcpy(ssid, "dlink-m200");
    // strcpy(password, "itriitri");
    // wifi_sta_config_t sta;
    wifi_config_t wifi_cfg =
        {
            .sta = {
                .threshold.authmode = WIFI_AUTH_WPA2_PSK}};
    strcpy((char *)wifi_cfg.sta.ssid, "dlink-m200");
    strcpy((char *)wifi_cfg.sta.password, "itriitri");
    //******************************************
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    //******************************************

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
    for (;;)
    {
        if (xQueueReceive(wifi_app_queue_handle, (void *)&msg, (portTickType)portMAX_DELAY))
        {
            ESP_LOGI(TAG, "%d", msg.msgID);
            ESP_LOGI(TAG, "%d", STA_SSID);

            switch (msg.msgID)
            {
            case STA_SSID:
                ESP_LOGI("yang", "test4");
                memset(ssid, 0, 32);
                strcpy(ssid, msg.content);
                ESP_LOGI(TAG, "ssid:%s", ssid);
                break;
            case STA_PASSWORD:
                memset(password, 0, 64);
                strcpy(password, msg.content);
                ESP_LOGI(TAG, "password:%s", password);
                break;
            case STA_RECONNECT:
                ESP_LOGI(TAG, "yes, connect");
                wifi_reconnect(ssid, password);
                break;
            default:
                break;
            }
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

void wifi_app(void)
{
    wifi_app_queue_handle = xQueueCreate(3, sizeof(wifi_app_queue_message_t));
    xTaskCreate(&wifi_app_start, "wifiapp", 8192, NULL, 6, NULL);
}