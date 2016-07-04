#ifndef _MAGNETICSENSORS_H_
#define _MAGNETICSENSORS_H_

#include <stdint.h>
#include <stdbool.h>

extern  float bicycleVelocityInMetersPerSeconds;
extern  uint32_t timeSinceLastWheelMagnetInt;

void initializeMagneticSensors(void);
void handleWheelMagnetInt(void);
void handleCrankMagnetInt(void);
void computeVelocityInMetersPerSecond();
void computeCadence();

#endif
