#ifndef INCLUDE_BATTERYCHECKER_H_
#define INCLUDE_BATTERYCHECKER_H_

#include <stdbool.h>
#include <stdint.h>

extern uint32_t batteryLevelReadings[4];

void initializeBatteryLevelCheck(void);
void readBatteryLevel(void);

#endif
