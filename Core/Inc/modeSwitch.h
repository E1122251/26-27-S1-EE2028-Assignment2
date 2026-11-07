#ifndef MODESWITCH_H
#define MODESWITCH_H

typedef enum {
	STANDBY_MODE,
	BATTLE_MODE,
	THE_LAST,
	TERMINATED,
	DEFAULT_MODE
} OperatingMode;

OperatingMode M_getOperatingMode(void);

void M_enterBattleMode(void);
void M_enterStandbyMode(void);
void M_enterTerminated(void);
void M_enterTheLast(void);

#endif
