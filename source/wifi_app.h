#ifndef WIFI_APP_H
#define WIFI_APP_H

typedef enum wifi_app_message
{
	STA_SSID = 0,
	STA_PASSWORD,
	STA_RECONNECT,
} wifi_app_message_e;

typedef struct wifi_app_queue_message
{
	wifi_app_message_e msgID;
    char content[64];
} wifi_app_queue_message_t;

void wifi_connect_event(wifi_app_message_e event_type, char* content);
void wifi_app(void);

#endif