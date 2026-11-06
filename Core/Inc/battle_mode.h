/*
 * battle_mode.h
 *
 *  Created on: Nov 4, 2026
 *      Author: Aryan Goswami
 */

#ifndef BATTLE_MODE_H
#define BATTLE_MODE_H

#include <stdint.h>
#include <stdbool.h>

void BattleMode_Init(void);
void BattleMode_OnSinglePress(void);
bool BattleMode_Update(void);

#endif /* BATTLE_MODE_H */
