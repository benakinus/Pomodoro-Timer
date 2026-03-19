#ifndef SENSEHAT_H
#define SENSEHAT_H

#include <stdint.h>

#define LED_COUNT 64

// RGB565 colors
#define RED   0xF800
#define GREEN 0x07E0
#define BLACK 0x0000

int sense_init(const char *fb_device);
void sense_clear();
void sense_light_led(int index, uint16_t color);
void sense_close();

#endif