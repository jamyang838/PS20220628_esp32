#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_event.h"
#include "nvs_flash.h"
#include "../source/tasks_common.h"
#include "../source/wifi_app.h"
#include "ble.h"

#include "trail_element.h"
#include "oled.h"

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Start Wifi
	// app_start();
	wifi_app();
	ble_app();
	ssd1306_app();
}
