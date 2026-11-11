#include "modeBattle.h"
#include "main.h"
#include "sensorData.h"
#include "stdio.h"
#include "string.h"

#include <stdbool.h>

#include "transmit.h"

#include "../../Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_accelero.h"
#include "modeOled.h"

static int lsr_e = 0;

static int drone_health = 100;
static bool destroyed = false;

// Variables to track OLED state and prevent redundant I2C updates
static int last_health = -1;
static int last_lsr = -1;
static bool last_warning = false;

static void Print_Health_Bar(int health, bool is_healing);

void MB_Init(void) {
    lsr_e = 0;
    drone_health = 100;
    destroyed = false;

    // Reset cache so screen force-draws on entry
    last_health = -1;
    last_lsr = -1;
    last_warning = false;

    MOL_PowerOn();
    MOL_Clear();
    MOL_UpdateBattleScreen(drone_health, lsr_e, false);

    char init_msg[] = VT_B_CYAN "=== Battle Mode Active ===" VT_RESET "\r\n"
                      VT_GREEN "Drone online | Hull: [##########] 100%" VT_RESET "\r\n";
    transmit(init_msg);
}

void MB_OnSinglePress(SensorData sensorData) {
    if (sensorData.temp.warning) {
        char warn[] = VT_BG_RED VT_WHITE VT_BOLD "[OVERHEAT] High temp detected! Charging locked out." VT_RESET "\r\n";
        transmit(warn);
        return;
    }

    lsr_e = lsr_e + 3;
    if (lsr_e > 10) {
        lsr_e = 10;
    }

    char msg[128];
    snprintf(msg, sizeof(msg), VT_B_YELLOW "[CHARGE]" VT_RESET " Laser energy: %d/10\r\n", lsr_e);
    transmit(msg);
}

void MB_Update(SensorData sensorData) {
    if (sensorData.noWarnings) {
    	if (lsr_e >= 5) {
			lsr_e -= 5;
			char msg[140];
			snprintf(msg, sizeof(msg), VT_B_GREEN ">>> Laser fired! (-5 energy, %d/10 left) <<<\r\n" VT_RESET, lsr_e, sensorData.temp.reading);
			transmit(msg);
		} else {
			if (drone_health > 0 && drone_health < 100) {
				drone_health += 10;
				if (drone_health > 100) {
					drone_health = 100;
				}

				Print_Health_Bar(drone_health, true);
			}
		}
    } else if (sensorData.accel.warning) {
    	drone_health -= 25;
		if (drone_health < 0) {
			drone_health = 0;
		}

		Print_Health_Bar(drone_health, false);

		if (drone_health == 0) {
			destroyed = true;
			char term_msg[] = VT_BG_RED VT_WHITE VT_BOLD "[CRITICAL] Drone destroyed! Initiating countdown. " VT_RESET "\r\n";
			transmit(term_msg);
		}
    } else {
    	char msg[160];

    	snprintf(msg, sizeof(msg),
			VT_BG_RED VT_WHITE VT_BOLD "[WARNING]" VT_RESET " " VT_B_YELLOW "Drone malfunctioning! " VT_RESET);
    	transmit(msg);

    	if (sensorData.temp.warning) {
    		snprintf(msg, sizeof(msg), VT_B_YELLOW "T" VT_RESET);
    		transmit(msg);
    	}

    	if (sensorData.pressure.warning) {
			snprintf(msg, sizeof(msg), VT_B_YELLOW "P" VT_RESET);
			transmit(msg);
		}

    	if (sensorData.humidity.warning) {
			snprintf(msg, sizeof(msg), VT_B_YELLOW "H" VT_RESET);
			transmit(msg);
		}

    	if (sensorData.gyro.warning) {
			snprintf(msg, sizeof(msg), VT_B_YELLOW "G" VT_RESET);
			transmit(msg);
		}

    	if (sensorData.magneto.warning) {
			snprintf(msg, sizeof(msg), VT_B_YELLOW "M" VT_RESET);
			transmit(msg);
		}

    	snprintf(msg, sizeof(msg), "\r\n");
    	transmit(msg);
    }

    // Only update OLED if data has actually changed to prevent CPU blocking
    if (destroyed) {
        MOL_PowerOff();
    } else if (drone_health != last_health || lsr_e != last_lsr || sensorData.temp.warning != last_warning) {
        MOL_UpdateBattleScreen(drone_health, lsr_e, sensorData.temp.warning);
        last_health = drone_health;
        last_lsr = lsr_e;
        last_warning = sensorData.temp.warning;
    }
}

bool MB_GetDestroyed(void) {
	return destroyed;
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

    transmit(msg);
}
