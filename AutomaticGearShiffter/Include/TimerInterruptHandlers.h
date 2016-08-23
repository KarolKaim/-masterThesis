#ifndef INCLUDE_TIMERINTERRUPTHANDLERS_H_
#define INCLUDE_TIMERINTERRUPTHANDLERS_H_

#include <stdint.h>
#include <stdbool.h>

extern bool sportModeTimerDone;
extern uint16_t sportTimerTicks;

void initializeTimer(uint32_t timerPeripheral, int32_t timerBase,
		uint32_t timerConfig, uint32_t timer, uint32_t intFlags,
		uint32_t secondsDivider);
void initialzeComfModeAndContChangeTimers(void);
void initialzieCurrentDerailleurPosChangeTimer(void);
void initializeMagneticSensorsTimers(void);
void initializeActiveAndSportModeTimers(void);
void initializeSportModeAccelReadingsAndPosChange(void);
void initialzeRgbAndBatteryLevelCheckTimers(void);

void turnOnDerailleurChangeTimer(void);
void turnOffDerailleurChangeTimer(void);
void turnOnConitnousChangeTimer(void);
void turnOffConitnousChangeTimer(void);
void turnOnComfortModeTimer(void);
void turnOffComfortModeTimer(void);
void turnOnActiveModeTimer(void);
void turnOffActiveModeTimer(void);
void turnOnSportModeTimer(void);
void turnOffSportModeTimer(void);
void turnOnMagneticSensorsTimers(void);
void turnOffMagneticSensorsTimers(void);
void turnOnAccelReadingsTimer(void);
void turnOffAccelReadingsTimer(void);
void turnOnRgbTimer(void);
void turnOffRgbTimer(void);
void turnOnBatteryTimer(void);
void turnOffBatteryTimer(void);
#endif
