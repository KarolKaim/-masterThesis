#ifndef INCLUDE_IMU_H_
#define INCLUDE_IMU_H_

#define LSM303DLHCMag_I2C_ADDRESS       0x1E
#define LSM303DLHCAccel_I2C_ADDRESS     0x19
#define L3GD20H_I2C_ADDRESS             0x6B

extern float surfaceAngle;
extern float accelerometerReadings[3];
extern float gyroReadings[3];
extern float previousGyroReadings[3];
extern float accelOffset[3];

void imuI2CHandler(void);
void initializeImu(void);
void readAccelMeasurements(float *resultsDestination);
void readRawAccelMeasurements(float *resultsDestination);
void readRawGyroMeasurements(float *resultsDestination);
void computeSteepness(void);
void copyReadings(float source[3], float destination[3]);
void calibrateAccel();

#endif
