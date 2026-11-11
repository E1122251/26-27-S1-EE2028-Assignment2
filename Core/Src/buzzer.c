/*
 * buzzer.c
 *
 * Created on: Nov 10, 2026
 * Author: Aryan Goswami
 */

#include "buzzer.h"
#include "main.h"

static uint32_t step_start_time = 0;
static uint8_t current_step = 0;
static BuzzPattern active_pattern = BUZZ_PATTERN_NONE;

void BUZZ_Init(void) {
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BUZZER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void BUZZ_Play(BuzzPattern pattern) {
    active_pattern = pattern;
    current_step = 0;
    step_start_time = HAL_GetTick();

    if (pattern != BUZZ_PATTERN_NONE) {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
    }
}

void BUZZ_Update(void) {
    if (active_pattern == BUZZ_PATTERN_NONE) {
        return;
    }

    uint32_t elapsed = HAL_GetTick() - step_start_time;

    switch (active_pattern) {
        case BUZZ_PATTERN_SHORT_CHIRP:
            if (current_step == 0 && elapsed >= 10) { // Snappy 10ms chirp
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                active_pattern = BUZZ_PATTERN_NONE;
            }
            break;

        case BUZZ_PATTERN_MODE_CHANGE:
            if (current_step == 0 && elapsed >= 15) { // 15ms ON
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                current_step = 1;
                step_start_time = HAL_GetTick();
            } else if (current_step == 1 && elapsed >= 15) { // 15ms OFF
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
                current_step = 2;
                step_start_time = HAL_GetTick();
            } else if (current_step == 2 && elapsed >= 15) { // 15ms ON
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                active_pattern = BUZZ_PATTERN_NONE;
            }
            break;

        case BUZZ_PATTERN_EVAC_PULSE:
            if (current_step == 0 && elapsed >= 30) { // Quick 30ms pulse
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                active_pattern = BUZZ_PATTERN_NONE;
            }
            break;

        case BUZZ_PATTERN_ALARM:
            if (current_step == 0 && elapsed >= 30) {
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                current_step = 1;
                step_start_time = HAL_GetTick();
            } else if (current_step == 1 && elapsed >= 20) {
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
                current_step = 2;
                step_start_time = HAL_GetTick();
            } else if (current_step == 2 && elapsed >= 30) {
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                current_step = 3;
                step_start_time = HAL_GetTick();
            } else if (current_step == 3 && elapsed >= 20) {
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
                current_step = 4;
                step_start_time = HAL_GetTick();
            } else if (current_step == 4 && elapsed >= 30) {
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                active_pattern = BUZZ_PATTERN_NONE;
            }
            break;

        default:
            HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
            active_pattern = BUZZ_PATTERN_NONE;
            break;
    }
}
