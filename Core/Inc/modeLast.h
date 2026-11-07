#ifndef MODELAST_H
#define MODELAST_H

#include <stdbool.h>

void ML_transmitRescueRequest(void);
int ML_getCountdown(void);
void ML_decrementCountdown(void);
void ML_resetCountdown(void);

#endif
