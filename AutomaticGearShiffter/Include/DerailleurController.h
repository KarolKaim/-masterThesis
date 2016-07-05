#ifndef INCLUDE_DERAILLEURCONTROLLER_H_
#define INCLUDE_DERAILLEURCONTROLLER_H_

#include "stdint.h"

#define PWM_PERIOD_IN_HUND_OF_US 400

enum GearMode {
	remote, comfort, active, sport
};

enum RgbColours {
	red, green, yellow, blue, pink, cyan, white
};

extern uint32_t currentPwmTicksInUs;
extern uint32_t gearPositions[8];
extern int8_t currentGear;

void initializeGearController(void);
void changeCurrentGearMode(void);
void reduceGear(void);
void increaseGear(void);

#endif
