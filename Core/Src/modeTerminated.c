#include "modeTerminated.h"

#include <stdbool.h>

static bool terminated = false;

bool MT_getTerminated(void) {
	return terminated;
}

void MT_setTerminated(void) {
	terminated = true;
}
