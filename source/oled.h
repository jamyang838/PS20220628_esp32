#ifndef OLED_H
#define OLED_H

#include "ssd1306.h"
#include "ssd1306_fonts.h"



typedef struct oled_queue_message
{	
    char content[64];
} oled_queue_message_t;
void oled_event(char *content);
void ssd1306_app( void );

#endif