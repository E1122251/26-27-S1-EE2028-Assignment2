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
#include "operatingMode.h"

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

	int the_last_countdown = 10;

	SensorData sensors = { 0 };

	bool double_press_flag = false;
	bool terminated = false;

	OperatingMode operating_mode = DEFAULT_MODE;

	while (1) {

		if (operating_mode == DEFAULT_MODE) enterStandbyMode(&operating_mode);

		// Update runtime, skip if loop has already been run for this tick
		runtime = HAL_GetTick();
		if (runtime == prev_runtime) continue;

		// Update sensor data
		updateSensorData(&sensors, runtime);

		// Update double_press_flag
		if (button_pressed_flag) double_press_flag = computeDoublePress(runtime, &prev_button_press_time);

		// Update LED
		updateLed(runtime);

		// Mode switching
		if (runtime % 1000 == 0) {
			switch(operating_mode) {
				case STANDBY_MODE:
					if (double_press_flag) enterBattleMode(&operating_mode);
					break;

				case BATTLE_MODE:
					if (sensors.accel.warning) enterTheLast(&operating_mode);
					break;

				case THE_LAST:
					if (double_press_flag) {
						enterStandbyMode(&operating_mode);
						resetCountdown();
					}
					break;

				case TERMINATED:
					break;
			}

			double_press_flag = false;
		}

		// Mode handling
		if (runtime % 1000 == 0) {
			switch(operating_mode) {
				case STANDBY_MODE:
					transmitTelemetry(sensors, runtime);
					break;

				case BATTLE_MODE:
					break;

				case THE_LAST:
					transmitRescueRequest();
					terminated = getTerminated();
					if (terminated) enterTerminated(&operating_mode);
					decrementCountdown();
					break;

				case TERMINATED:
					while (1) {}
			}
		}

		// Update prev_runtime
		prev_runtime = runtime;

		// Clear button_pressed_flag;
		button_pressed_flag = false;

	}
}
