#include "battle_mode.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_accelero.h"

extern UART_HandleTypeDef huart1;

static int lsr_e = 0;
static uint32_t led_lst = 0;

// FORWARD DECLARATION (MUST be above BattleMode_Update)
static bool Internal_Check_Upside_Down(void);

void BattleMode_Init(void) {
    lsr_e = 0;
    led_lst = HAL_GetTick();
}

void BattleMode_OnSinglePress(void) {
    lsr_e = lsr_e + 3;
    if (lsr_e > 10) {
        lsr_e = 10;
    }
}

bool BattleMode_Update(uint32_t current_time) {
    if (current_time - led_lst >= 500) {
        led_lst = current_time;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }

    if (lsr_e >= 5) {
        lsr_e -= 5;
        char msg[] = "LASER GUN FIRED!\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 0xFFFF);
    }

    if (Internal_Check_Upside_Down()) {
        return true;
    }

    return false;
}

static bool Internal_Check_Upside_Down(void)
{
    int16_t accel_raw[3] = {0};

    BSP_ACCELERO_AccGetXYZ(accel_raw);

    if (accel_raw[2] < -500) {
        return true;
    } else {
        return false;
    }
}
