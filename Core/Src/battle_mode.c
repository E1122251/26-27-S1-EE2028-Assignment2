#include "battle_mode.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_accelero.h"

extern UART_HandleTypeDef huart1;

static int lsr_e = 0;

void BattleMode_Init(void) {
    lsr_e = 0;
}

void BattleMode_OnSinglePress(void) {
    lsr_e = lsr_e + 3;
    if (lsr_e > 10) {
        lsr_e = 10;
    }
}

bool BattleMode_Update(void) {
    if (lsr_e >= 5) {
        lsr_e -= 5;
        char msg[] = "LASER GUN FIRED!\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 0xFFFF);
    }
}
