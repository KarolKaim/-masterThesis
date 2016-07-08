#ifndef _SWITCHGEAR_H_
#define _SWITCHGEAR_H_

#include <stdbool.h>
#include <stdint.h>

void initializeSwitches(void);
bool isGearUpSwitchPressed(void);
bool isGearDownSwitchPressed(void);
void disableSwitches(void);
void enableSwitches(void);

#endif
