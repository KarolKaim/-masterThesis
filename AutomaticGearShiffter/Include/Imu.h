#ifndef INCLUDE_IMU_H_
#define INCLUDE_IMU_H_

#define LSM303DLHCMag_I2C_ADDRESS       0x1E
#define LSM303DLHCAccel_I2C_ADDRESS     0x19
#define L3GD20H_I2C_ADDRESS             0x6B

extern float surfaceAngle;
extern float accelerometerReadings[3];

void imuI2CHandler(void);
void initializeImu(void);
void readAccelMeasurements(float resultsDestination[3]);
void computeSteepness(void);



#endif
