#ifndef INCLUDE_TIMERINTERRUPTHANDLERS_H_
#define INCLUDE_TIMERINTERRUPTHANDLERS_H_

#include <stdint.h>
#include <stdbool.h>

void initializeTimer(uint32_t timerPeripheral, int32_t timerBase,
		uint32_t timerConfig, uint32_t timer, uint32_t intFlags,
		uint32_t secondsDivider);
void turnOnConitnousChangeTimer(void);
void turnOffConitnousChangeTimer(void);
void initialzeContinuousGearChangeTimer(void);
void initializeComfortModePeriodicTimer(void);
void turnOnComfortModeTimer(void);
void turnOffComfortModeTimer(void);
#endif
