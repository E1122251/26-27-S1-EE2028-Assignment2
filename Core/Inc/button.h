#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

#include <stdint.h>
#include <stdbool.h>

void B_update(uint32_t runtime);
bool B_getSinglePressFlag(void);
bool B_getDoublePressFlag(void);
void B_consumeSinglePressFlag(void);
void B_consumeSinglePressFlag(void);

#endif
