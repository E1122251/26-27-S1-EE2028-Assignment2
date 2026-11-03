#include "operatingMode.h"

#include <stdint.h>

#include <stm32l4xx_hal.h>

#include "led.h"

extern UART_HandleTypeDef huart1;

static OperatingMode operating_mode = DEFAULT_MODE;

OperatingMode getOperatingMode(void) {
	return operating_mode;
}

static void enterOperatingMode(OperatingMode mode) {
	const char *message_transmit;

	operating_mode = mode;

	switch (mode) {
		case BATTLE_MODE:
			setLedMode(LED_BLINK_1HZ);

			message_transmit = "Entering BATTLE_MODE\r\n";
			break;

		case STANDBY_MODE:
			setLedMode(LED_ALWAYS_ON);

			message_transmit = "Entering STANDBY_MODE\r\n";
			break;

		case TERMINATED:
			setLedMode(LED_ALWAYS_OFF);

			message_transmit = "Executing termination sequence. Goodbye cruel world.\r\n";
			break;

		case THE_LAST:
			setLedMode(LED_BLINK_2HZ);

			message_transmit = "Entering THE_LAST\r\n";
			break;
	}

	HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
}

void enterBattleMode(void) {
	enterOperatingMode(BATTLE_MODE);
}

void enterStandbyMode(void) {
	enterOperatingMode(STANDBY_MODE);
}

void enterTerminated(void) {
	enterOperatingMode(TERMINATED);
}

void enterTheLast(void) {
	enterOperatingMode(THE_LAST);
}
