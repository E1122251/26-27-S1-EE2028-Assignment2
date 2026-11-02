#include "operatingMode.h"

#include <stdint.h>

#include <stm32l4xx_hal.h>

#include "led.h"

extern UART_HandleTypeDef huart1;

void enterStandbyMode(OperatingMode *operatingMode) {
	*operatingMode = STANDBY_MODE;
	setLedMode(LED_ALWAYS_ON);

	const char message_transmit[] = "Entering STANDBY_MODE\r\n";
	HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
}

void enterBattleMode(OperatingMode *operatingMode) {
	*operatingMode = BATTLE_MODE;
	setLedMode(LED_BLINK_1HZ);

	const char message_transmit[] = "Entering BATTLE_MODE\r\n";
	HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
}

void enterTheLast(OperatingMode *operatingMode) {
	*operatingMode = THE_LAST;
	setLedMode(LED_BLINK_2HZ);

	const char message_transmit[] = "Entering THE_LAST\r\n";
	HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
}

void enterTerminated(OperatingMode *operatingMode) {
	*operatingMode = TERMINATED;
	setLedMode(LED_ALWAYS_OFF);

	const char message_transmit[] = "Executing termination sequence. Goodbye cruel world.\r\n";
	HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
}
