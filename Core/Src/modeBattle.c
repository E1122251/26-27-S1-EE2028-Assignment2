#include "modeBattle.h"
#include "main.h"
#include "sensors.h"
#include "stdio.h"
#include "string.h"

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_accelero.h"

extern UART_HandleTypeDef huart1;

static int lsr_e = 0;

static int drone_health = 100;
static uint32_t health_lst = 0;

static bool Internal_Check_Upside_Down(void);
static void Print_Health_Bar(int health, bool is_healing);

void MB_Init(void) {
    lsr_e = 0;
    drone_health = 100;
    health_lst = HAL_GetTick();

    char init_msg[] = VT_B_CYAN "=== Battle Mode Active ===" VT_RESET "\r\n"
                      VT_GREEN "Drone online | Hull: [##########] 100%" VT_RESET "\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)init_msg, strlen(init_msg), 0xFFFF);
}

void MB_OnSinglePress(void) {
    SensorData current_sensors = getSensorData();

    if (current_sensors.temp.warning) {
        char warn[] = VT_BG_RED VT_WHITE VT_BOLD "[OVERHEAT] High temp detected! Charging locked out." VT_RESET "\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t*)warn, strlen(warn), 0xFFFF);
        return;
    }

    lsr_e = lsr_e + 3;
    if (lsr_e > 10) {
        lsr_e = 10;
    }

    char msg[128];
    snprintf(msg, sizeof(msg), VT_B_YELLOW "[CHARGE]" VT_RESET " Laser energy: %d/10\r\n", lsr_e);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 0xFFFF);
}

bool MB_Update(void) {
    uint32_t current_time = HAL_GetTick();
    SensorData current_sensors = getSensorData();

    if (lsr_e >= 5 && !current_sensors.temp.warning) {
        lsr_e -= 5;
        char msg[140];
        snprintf(msg, sizeof(msg), VT_B_GREEN ">>> Laser fired! (-5 energy, %d/10 left) [Temp: %.1f C] <<<\r\n" VT_RESET, lsr_e, current_sensors.temp.reading);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 0xFFFF);
    }

    if (current_time - health_lst >= 1000) {
        health_lst = current_time;

        if (Internal_Check_Upside_Down()) {
            drone_health -= 25;
            if (drone_health < 0) {
                drone_health = 0;
            }

            Print_Health_Bar(drone_health, false);

            if (drone_health == 0) {
                char term_msg[] = VT_BG_RED VT_WHITE VT_BOLD " [CRITICAL] Drone destroyed! Initiating failure sequence. " VT_RESET "\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t*)term_msg, strlen(term_msg), 0xFFFF);
                return true;
            }
        } else {
            if (drone_health > 0 && drone_health < 100) {
                drone_health += 10;
                if (drone_health > 100) {
                    drone_health = 100;
                }

                Print_Health_Bar(drone_health, true);
            }
        }
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

static void Print_Health_Bar(int health, bool is_healing) {
    char bar[11] = "----------";
    int filled = health / 10;
    if (filled < 0) filled = 0;
    if (filled > 10) filled = 10;

    for (int i = 0; i < filled; i++) {
        bar[i] = '#';
    }

    char msg[160];
    if (is_healing) {
        snprintf(msg, sizeof(msg),
            VT_B_CYAN "[REPAIR]" VT_RESET " " VT_GREEN "Recovering... Hull: [%s] %d%%" VT_RESET "\r\n",
            bar, health);
    } else {
        if (health > 60) {
            snprintf(msg, sizeof(msg),
                VT_BG_RED VT_WHITE VT_BOLD "[DAMAGE]" VT_RESET " " VT_GREEN "Hull hit! Health: [%s] %d%%" VT_RESET "\r\n",
                bar, health);
        } else if (health >= 30) {
            snprintf(msg, sizeof(msg),
                VT_BG_RED VT_WHITE VT_BOLD "[WARNING]" VT_RESET " " VT_B_YELLOW "Hull taking heavy hits! Health: [%s] %d%%" VT_RESET "\r\n",
                bar, health);
        } else {
            snprintf(msg, sizeof(msg),
                VT_BG_RED VT_WHITE VT_BOLD "[DANGER]" VT_RESET " " VT_B_RED "Hull failing! Health: [%s] %d%%" VT_RESET "\r\n",
                bar, health);
        }
    }

    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 0xFFFF);
}
