/*
 * buzzer.h
 *
 * Created on: Nov 10, 2026
 * Author: Aryan Goswami
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>

#define BUZZER_PORT GPIOD
#define BUZZER_PIN  GPIO_PIN_14

typedef enum {
    BUZZ_PATTERN_NONE = 0,
    BUZZ_PATTERN_SHORT_CHIRP,
    BUZZ_PATTERN_MODE_CHANGE,
    BUZZ_PATTERN_EVAC_PULSE,
    BUZZ_PATTERN_ALARM
} BuzzPattern;

void BUZZ_Init(void);
void BUZZ_Play(BuzzPattern pattern);
void BUZZ_Update(void);

#endif /* BUZZER_H */
