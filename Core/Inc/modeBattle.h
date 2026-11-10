/*
- modeBattle.h
 *
-  Created on:
Nov 4, 2026
-      Author: Aryan Goswami
 */

#ifndef MODEBATTLE_H
#define MODEBATTLE_H

#include <stdint.h>
#include <stdbool.h>

#define VT_RESET     "\033[0m"
#define VT_BOLD      "\033[1m"
#define VT_DIM       "\033[2m"

#define VT_RED       "\033[31m"
#define VT_GREEN     "\033[32m"
#define VT_YELLOW    "\033[33m"
#define VT_CYAN      "\033[36m"
#define VT_WHITE     "\033[37m"

#define VT_B_RED     "\033[1;31m"
#define VT_B_GREEN   "\033[1;32m"
#define VT_B_YELLOW  "\033[1;33m"
#define VT_B_CYAN    "\033[1;36m"
#define VT_BG_RED    "\033[41m"

#include "sensorData.h"

void MB_Init(void);
void MB_OnSinglePress(SensorData sensorData);
void MB_Update(SensorData sensorData);
bool MB_GetDestroyed(void);

#endif /* MODEBATTLE_H */
