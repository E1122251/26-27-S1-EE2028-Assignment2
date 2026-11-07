#ifndef MODELAST_H
#define MODELAST_H

#include <stdbool.h>

void ML_transmitRescueRequest(void);
void ML_decrementCountdown(void);
void ML_resetCountdown(void);
bool ML_getTerminated(void);

#endif
