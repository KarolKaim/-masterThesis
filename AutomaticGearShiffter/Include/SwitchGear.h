#ifndef _SWITCHGEAR_H_
#define _SWITCHGEAR_H_

#include <stdbool.h>
#include <stdint.h>

extern uint32_t switchBouncingDelayInMs;
extern bool switchBouncingTimerNotActivated;

void initializeSwitches(void);
void handleGearUp(void);
void handleGearDown(void);

#endif
