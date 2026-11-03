#ifndef OPERATINGMODE_H
#define OPERATINGMODE_H

typedef enum {
	STANDBY_MODE,
	BATTLE_MODE,
	THE_LAST,
	TERMINATED,
	DEFAULT_MODE
} OperatingMode;

OperatingMode getOperatingMode(void);

void enterBattleMode(void);
void enterStandbyMode(void);
void enterTerminated(void);
void enterTheLast(void);

#endif
