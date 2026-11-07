/******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * (c) EE2028 Teaching Team
  ******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <stdbool.h>

#include "customInit.h"
#include "sensors.h"

#include "modeSwitch.h"
#include "modeStandby.h"
#include "modeBattle.h"
#include "modeLast.h"
#include "modeTerminated.h"

UART_HandleTypeDef huart1;

static const uint32_t DOUBLE_PRESS_INTERVAL = 500;

// ========== Push button ==========
static volatile bool button_pressed_flag = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == BUTTON_EXTI13_Pin) {
		button_pressed_flag = true;
	}
}

bool computeDoublePress(uint32_t runtime, uint32_t *prev_button_press_time) {
	if (*prev_button_press_time == -1){
		*prev_button_press_time = runtime;
		return false;
	}

	uint32_t interval = runtime - *prev_button_press_time;

	if (interval <= DOUBLE_PRESS_INTERVAL) {
		*prev_button_press_time = -1;
		return true;
	}

	*prev_button_press_time = runtime;
	return false;
}

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
			}
			break;

		case THE_LAST:
			if (MT_getTerminated()) {
				M_enterTerminated();
			} else if (double_press_flag) {
				M_enterStandbyMode();
				ML_resetCountdown();
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

	uint32_t prev_button_press_time = -1;
	uint32_t prev_led_toggle_time = -1;

	SensorData sensorData = { 0 };

	bool double_press_flag = false;

	M_enterStandbyMode();

	while (1) {

		// Update runtime, skip if loop has already been run for this tick
		runtime = HAL_GetTick();
		if (runtime == prev_runtime) continue;

		// Update double_press_flag
		if (button_pressed_flag) {
			double_press_flag = computeDoublePress(runtime, &prev_button_press_time);

			if (!double_press_flag && M_getOperatingMode() == BATTLE_MODE) {
				MB_OnSinglePress();
			}
		}

		// 4 Hz
		if (runtime % 250 == 0) {
			updateLed(runtime);
		}

		// 1 Hz
		if (runtime % 1000 == 0) {
			// Update sensorData
			updateSensorData();
			sensorData = getSensorData();

			handleModeSwitch(double_press_flag, sensorData);
			double_press_flag = false;

			handleModeActions(sensorData);
		}

		// Update prev_runtime
		prev_runtime = runtime;

		// Clear button_pressed_flag;
		button_pressed_flag = false;

	}
}
