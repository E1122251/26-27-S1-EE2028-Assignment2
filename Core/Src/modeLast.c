#include "modeLast.h"
#include <stdbool.h>

#include <stm32l4xx_hal.h>

extern UART_HandleTypeDef huart1;

static int countdown = 10;

void ML_transmitRescueRequest(void) {
	char message_transmit[128];

	if (countdown != 0) {
		sprintf(message_transmit, "Drone Was Attacked! Beginning termination sequence in %i\r\n", countdown);
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
	} else {
		sprintf(message_transmit, "Drone Was Attacked! Beginning termination sequence\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
	}
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

