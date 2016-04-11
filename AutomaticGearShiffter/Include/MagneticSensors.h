#ifndef _MAGNETICSENSORS_H_
#define _MAGNETICSENSORS_H_

#include <stdint.h>
#include <stdbool.h>

extern  uint32_t bicycleVelocity;
extern  uint32_t wheelMagnetBouncingDelayInMs;
extern  uint32_t wheelMagnetIntervalInMs;
extern  bool wheelMagnetBouncingTimerActivated;

void initializeMagneticSensors(void);
void handleWheelMagnetInt(void);
void computeBicycleVelocityInMetersPerSecond();

#endif
