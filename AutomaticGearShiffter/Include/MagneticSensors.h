#ifndef _MAGNETICSENSORS_H_
#define _MAGNETICSENSORS_H_

#include <stdint.h>
#include <stdbool.h>

extern  float bicycleVelocityInMetersPerSeconds;
extern float cadenceInRPM;
extern  uint32_t timeSinceLastWheelMagnetInt;
extern  uint32_t timeSinceLastCrankMagnetInt;

void initializeMagneticSensors(void);
void handleWheelMagnetInt(void);
void handleCrankMagnetInt(void);
void computeVelocityInMetersPerSecond();
uint8_t getBikeVelocityInKmPerH();
void computeCadence();

#endif
