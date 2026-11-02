#include "theLast.h"

#include <stdbool.h>

#include <stm32l4xx_hal.h>

extern UART_HandleTypeDef huart1;

static bool terminated = false;
static int countdown = 10;

void transmitRescueRequest(void) {
	char message_transmit[128];

	sprintf(message_transmit, "Drone Was Attacked! Terminating in %i\r\n", countdown);
	HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
}

void decrementCountdown(void) {
	countdown -= 1;
	if (countdown == 0) terminated = true;
}

void resetCountdown(void) {
	countdown = 10;
}

bool getTerminated(void) {
	return terminated;
}
