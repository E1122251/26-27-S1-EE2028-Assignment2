#ifndef OPERATINGMODE_H
#define OPERATINGMODE_H

typedef enum {
	STANDBY_MODE,
	BATTLE_MODE,
	THE_LAST,
	TERMINATED,
	DEFAULT_MODE
} OperatingMode;

void enterStandbyMode(OperatingMode *operatingMode);
void enterBattleMode(OperatingMode *operatingMode);
void enterTheLast(OperatingMode *operatingMode);
void enterTerminated(OperatingMode *operatingMode);

#endif
