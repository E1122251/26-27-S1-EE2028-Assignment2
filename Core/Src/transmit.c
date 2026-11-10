#include "transmit.h"

#include <stdio.h>
#include <string.h>

#include <stm32l4xx_hal.h>

extern UART_HandleTypeDef huart1;

void transmit(const char *message) {
	HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), 0xFFFF);
}
