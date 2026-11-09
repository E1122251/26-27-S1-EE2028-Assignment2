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
#include "sensors.h"
#include "button.h"

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
				MB_Init();
			}
			break;

		case BATTLE_MODE:
			if (sensor_data.accel.warning) {
				M_enterTheLast();
			} else if (double_press_flag) {
				M_enterStandbyMode();
			}
			break;

		case THE_LAST:
			if (MT_getTerminated()) {
				M_enterTerminated();
			} else if (double_press_flag) {
				ML_resetCountdown();
				M_enterBattleMode();
			}
			break;

		case TERMINATED:
			break;
	}
}

void handleModeActions(SensorData sensorData) {
	switch(M_getOperatingMode()) {
		case STANDBY_MODE:
			MS_transmitTelemetry(sensorData);
			break;

		case BATTLE_MODE:
			MB_Update();
			break;

		case THE_LAST:
			if (ML_getCountdown() == 0) MT_setTerminated();
			ML_transmitRescueRequest();
			ML_decrementCountdown();
			break;

		case TERMINATED:
			while (1) {}
			break;
	}
}

// ========== Main loop ==========

int main(void)
{

	// Initialisation
	HAL_Init();
	MX_GPIO_Init();
	UART1_Init();
	initSensors();
	initLed();

	uint32_t runtime = -1;
	uint32_t prev_runtime = -1;

	SensorData sensorData = { 0 };

	M_enterStandbyMode();

	while (1) {

		// Update runtime, skip if loop has already been run for this tick
		runtime = HAL_GetTick();
		if (runtime == prev_runtime) continue;

		// Update button flags
		B_update(runtime);

		// 4 Hz
		if (runtime % 250 == 0) {
			updateLed(runtime);
		}

		// 1 Hz
		if (runtime % 1000 == 0) {
			// Update sensorData
			updateSensorData();
			sensorData = getSensorData();

			handleModeSwitch(B_getDoublePressFlag(), sensorData);
			B_consumeDoublePressFlag();
		}

		// 2 Hz
		if (runtime % 500 == 0) {
			if (B_getSinglePressFlag()) {
				MB_OnSinglePress();
				B_consumeSinglePressFlag();
			}
		}

		if (runtime % 1000 == 0) {
			handleModeActions(sensorData);
		}

		// Update prev_runtime
		prev_runtime = runtime;

	}
}
