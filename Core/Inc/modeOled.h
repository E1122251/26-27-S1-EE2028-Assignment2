/*
 * modeOled.h
 *
 * Created on: Nov 10, 2026
 * Author: Aryan Goswami
 */

#ifndef MODEOLED_H
#define MODEOLED_H

#include <stdint.h>
#include <stdbool.h>

#define OLED_I2C_ADDR 0x78

void MOL_Init(void);
void MOL_Clear(void);
void MOL_PowerOff(void);
void MOL_PowerOn(void);
void MOL_UpdateBattleScreen(int health, int energy, bool temp_warning);

#endif /* MODEOLED_H */
