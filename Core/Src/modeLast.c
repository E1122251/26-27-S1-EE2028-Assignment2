#include "modeLast.h"

#include <stdbool.h>
#include <stdio.h>

#include "transmit.h"

static int countdown = 10;

void ML_transmitRescueRequest(void) {
	char message_transmit[128];

	if (countdown != 0) {
		sprintf(message_transmit, "Drone Was Attacked! Beginning termination sequence in %i\r\n", countdown);
	} else {
		sprintf(message_transmit, "Drone Was Attacked! Beginning termination sequence\r\n");
	}

	transmit(message_transmit);
}

void ML_transmitRescueAcknowledgement(void) {
	char message_transmit[128];
	sprintf(message_transmit, "Drone Was Rescued!\r\n", countdown);
	transmit(message_transmit);
}

int ML_getCountdown(void) {
	return countdown;
}

void ML_decrementCountdown(void) {
	countdown -= 1;
}

void ML_resetCountdown(void) {
	countdown = 10;
}

