// #include <string.h>
// #include <stdlib.h>
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

static EventGroupHandle_t wifi_event_group;
static void event_handler( void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data )
{

}
void wifi_app_start( void )
{
    ESP_ERROR_CHECK( esp_netif_init() );
    wifi_event_group = xEventGroupCreate();
    
    ESP_ERROR_CHECK( esp_event_loop_create_default() );    
    esp_netif_t* sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    //******************************************    
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    // esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    //******************************************

    ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}