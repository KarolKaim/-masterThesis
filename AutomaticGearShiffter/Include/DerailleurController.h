#ifndef INCLUDE_DERAILLEURCONTROLLER_H_
#define INCLUDE_DERAILLEURCONTROLLER_H_

#include "stdint.h"

#define PWM_PERIOD_IN_HUND_OF_US 400

enum GearMode {
	remote, comfort, active, sport
};

extern uint32_t currentPwmTicksInUs;
extern uint32_t derailleurPositions[8];
extern uint32_t derailleurUp[8];
extern uint32_t derailleurDown[8];
extern volatile uint32_t * currentDerailleurPosition;
extern int8_t currentGear;
extern enum GearMode currentMode;

void initializeGearController(void);
void changeCurrentGearMode(void);
void reduceGear(void);
void increaseGear(void);
void comfortModeHandler();
void activeModeHandler();
void sportModeHandler();

#endif
