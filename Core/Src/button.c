#include "button.h"
#include "main.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	WAITING_FOR_1ST,
	WAITING_FOR_2ND
} ButtonFsmState;

static const uint32_t DOUBLE_PRESS_INTERVAL = 500;

static uint32_t interval = 0;

static uint32_t prev_button_interrupt_time = -1;
static volatile bool button_interrupt_flag = false;

static bool single_press_flag = false;
static bool double_press_flag = false;

static ButtonFsmState buttonFsmState = WAITING_FOR_1ST;

void B_update(uint32_t runtime) {
	switch (buttonFsmState) {

		case WAITING_FOR_1ST:
			if (button_interrupt_flag) {
				prev_button_interrupt_time = runtime;
				buttonFsmState = WAITING_FOR_2ND;
				button_interrupt_flag = false;
			}
			break;

		case WAITING_FOR_2ND:
			interval = runtime - prev_button_interrupt_time;

			if (interval >= DOUBLE_PRESS_INTERVAL) {
				single_press_flag = true;
				buttonFsmState = WAITING_FOR_1ST;
			} else if (button_interrupt_flag) {
				double_press_flag = true;
				buttonFsmState = WAITING_FOR_1ST;
				button_interrupt_flag = false;
			}
			break;
	}
}

bool B_getSinglePressFlag(void) {
	return single_press_flag;
}

bool B_getDoublePressFlag(void) {
	return double_press_flag;
}

void B_consumeSinglePressFlag(void) {
	single_press_flag = false;
}

void B_consumeDoublePressFlag(void) {
	double_press_flag = false;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == BUTTON_EXTI13_Pin) {
		button_interrupt_flag = true;
	}
}
