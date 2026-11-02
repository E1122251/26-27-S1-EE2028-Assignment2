#include "led.h"

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01.h"

static LedMode current_mode = LED_DEFAULT;
static uint32_t prev_toggle_time = 0;
static uint32_t toggle_interval = 0;

void setLedMode(LedMode mode) {
	if (mode == current_mode) return;

	current_mode = mode;

	switch (current_mode) {
		case LED_ALWAYS_ON:
			BSP_LED_On(LED2);
			toggle_interval = 0;
			break;

		case LED_ALWAYS_OFF:
			BSP_LED_Off(LED2);
			toggle_interval = 0;
			break;

		case LED_BLINK_1HZ:
			BSP_LED_On(LED2);
			toggle_interval = 500;
			break;

		case LED_BLINK_2HZ:
			BSP_LED_On(LED2);
			toggle_interval = 250;
			break;
	}
}

void updateLed(uint32_t runtime) {
    if (runtime == prev_toggle_time) return;

    prev_toggle_time = runtime;

    if (toggle_interval > 0 && (runtime % toggle_interval == 0)) {
        BSP_LED_Toggle(LED2);
        prev_toggle_time = runtime;
    }
}
