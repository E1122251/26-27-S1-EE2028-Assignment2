#include <modeSwitch.h>
#include <stdint.h>

#include <stm32l4xx_hal.h>

#include "led.h"

#include "modeBattle.h"
#include "modeLast.h"

extern UART_HandleTypeDef huart1;

static OperatingMode operating_mode = DEFAULT_MODE;

OperatingMode M_getOperatingMode(void) {
	return operating_mode;
}

static void M_enterOperatingMode(OperatingMode mode) {
	const char *message_transmit;

	operating_mode = mode;

	switch (mode) {
		case BATTLE_MODE:
			setLedMode(LED_BLINK_1HZ);
			message_transmit = "Entering BATTLE_MODE\r\n";
			HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
			MB_Init();
			break;

		case STANDBY_MODE:
			setLedMode(LED_ALWAYS_ON);
			message_transmit = "Entering STANDBY_MODE\r\n";
			HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
			break;

		case TERMINATED:
			setLedMode(LED_ALWAYS_OFF);
			message_transmit = "Termination sequence complete. Goodbye cruel world.\r\n";
			HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
			break;

		case THE_LAST:
			setLedMode(LED_BLINK_2HZ);
			message_transmit = "Entering THE_LAST\r\n";
			HAL_UART_Transmit(&huart1, (uint8_t*)message_transmit, strlen(message_transmit),0xFFFF);
			ML_resetCountdown();
			break;
	}

}

void M_enterBattleMode(void) {
	M_enterOperatingMode(BATTLE_MODE);
}

void M_enterStandbyMode(void) {
	M_enterOperatingMode(STANDBY_MODE);
}

void M_enterTerminated(void) {
	M_enterOperatingMode(TERMINATED);
}

void M_enterTheLast(void) {
	M_enterOperatingMode(THE_LAST);
}
