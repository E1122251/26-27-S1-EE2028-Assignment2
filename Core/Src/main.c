/******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * (c) EE2028 Teaching Team
  ******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <stdbool.h>
#include <stdint.h>

#include "customInit.h"

#include "button.h"
#include "led.h"
#include "sensors.h"

#include "modeSwitch.h"
#include "modeStandby.h"
#include "modeBattle.h"
#include "modeLast.h"
#include "modeTerminated.h"

UART_HandleTypeDef huart1;

// ========== Handle mode ==========
void handleModeSwitch(bool double_press_flag, SensorData sensor_data) {
	switch(M_getOperatingMode()) {
		case STANDBY_MODE:
			if (double_press_flag) {
				M_enterBattleMode();
			}
			break;

		case BATTLE_MODE:
			if (MB_GetDestroyed()) {
				M_enterTheLast();
			} else if (double_press_flag) {
				M_enterStandbyMode();
			}
			break;

		case THE_LAST:
			if (MT_getTerminated()) {
				M_enterTerminated();
			} else if (double_press_flag) {
				ML_transmitRescueAcknowledgement();
				M_enterBattleMode();
			}
			break;

		case TERMINATED:
			break;

		default:
			break;
	}

	B_consumeDoublePressFlag();
}

void handleModeButtonActions(bool single_press_flag, uint32_t runtime, SensorData sensorData) {
	if (single_press_flag) {
		switch (M_getOperatingMode()) {
			case STANDBY_MODE:
				MS_transmitLifetime(runtime);
				break;

			case BATTLE_MODE:
				MB_OnSinglePress(sensorData);
				break;

			default:
				break;
		}

		B_consumeSinglePressFlag();
	}
}

void handleModeRecurringActions(SensorData sensorData) {
	switch(M_getOperatingMode()) {
		case STANDBY_MODE:
			MS_transmitTelemetry(sensorData);
			break;

		case BATTLE_MODE:
			MB_Update(sensorData);
			break;

		case THE_LAST:
			if (ML_getCountdown() == 0) MT_setTerminated();
			ML_transmitRescueRequest();
			ML_decrementCountdown();
			break;

		case TERMINATED:
			while (1) {}
			break;

		default:
			break;
	}
}

// ========== Main loop ==========

int main(void)
{

	customInit();

	uint32_t runtime = -1;
	uint32_t prev_runtime = -1;

	bool clock_1Hz = false;
	bool clock_2Hz = false;
	bool clock_4Hz = false;

	SensorData sensorData = { 0 };

	M_enterStandbyMode();

	while (1) {

		// Update runtime, skip if loop has already been run for this tick
		runtime = HAL_GetTick();
		if (runtime == prev_runtime) continue;

		// Update clock bools
		clock_1Hz = (runtime % 1000 == 0);
		clock_2Hz = (runtime % 500 == 0);
		clock_4Hz = (runtime % 250 == 0);

		// Update button flags
		B_update(runtime);

		if (clock_1Hz) {
			updateSensorData();
			sensorData = getSensorData();
		}

		if (clock_1Hz) handleModeSwitch(B_getDoublePressFlag(), sensorData);
		if (clock_2Hz) handleModeButtonActions(B_getSinglePressFlag(), runtime, sensorData);
		if (clock_1Hz) handleModeRecurringActions(sensorData);

		if (clock_4Hz) updateLed(runtime);

		// Update prev_runtime
		prev_runtime = runtime;

	}
}
