#ifndef LED_H
#define LED_H

#include <stdint.h>

typedef enum {
    LED_ALWAYS_ON,
	LED_ALWAYS_OFF,
    LED_BLINK_1HZ,
    LED_BLINK_2HZ,
	LED_DEFAULT
} LedMode;

void setLedMode (LedMode mode);
void updateLed(uint32_t runtime);

#endif
