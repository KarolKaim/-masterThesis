#include "Imu.h"

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/hw_lsm303dlhc.h"
#include "sensorlib/lsm303dlhc_accel.h"
#include "sensorlib/l3gd20h.h"
#include "sensorlib/hw_l3gd20h.h"

#include "TimerInterruptHandlers.h"

tLSM303DLHCAccel sLSM303DLHCAccel;
tI2CMInstance g_sI2CInst;
tL3GD20H sl3gd20h;

volatile uint_fast8_t I2CDoneFlag;
volatile uint_fast8_t ErrorFlag;

float gyroReadings[3] = { 0.0, 0.0, 0.0 };
float previousGyroReadings[3] = { 0.0, 0.0, 0.0 };
float accelOffset[3] = { 0.0, 0.0, 0.0 };
float surfaceAngle = 0;
float tmpSurfaceAngle = 0;
float gravityAcceleration = 9.8102;
float accelerometerReadings[3] = { 0.0, 0.0, 0.0 };

void IMU10CallBack(void *pvCallbackData, uint_fast8_t ui8Status) {

	if (ui8Status == I2CM_STATUS_SUCCESS) {
		I2CDoneFlag = 1;
	}

	ErrorFlag = ui8Status;
}

void imuI2CHandler(void) {

	I2CMIntHandler(&g_sI2CInst);
}

void calibrateAccel() {
	float sum[3] = { 0.0, 0.0, 0.0 };
	int i = 0;
	for (i = 0; i < 30; i++) {
		readRawAccelMeasurements(accelOffset);
		sum[0] += accelOffset[0];
		sum[1] += accelOffset[1];
		sum[2] += accelOffset[2];
	}
	sum[0] = sum[0] / 30;
	sum[1] = sum[1] / 30;
	sum[1] = sum[1] - gravityAcceleration;
	sum[2] = sum[2] / 30;

	accelOffset[0] = sum[0];
	accelOffset[1] = sum[1];
	accelOffset[2] = sum[2];

}

void initializeAccel() {
	LSM303DLHCAccelInit(&sLSM303DLHCAccel, &g_sI2CInst,
	LSM303DLHCAccel_I2C_ADDRESS, IMU10CallBack, &sLSM303DLHCAccel);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;

	LSM303DLHCAccelReadModifyWrite(&sLSM303DLHCAccel, LSM303DLHC_O_CTRL1, 0x00,
			LSM303DLHC_CTRL1_ODR_100HZ | LSM303DLHC_CTRL1_AXIS_X_EN
					| LSM303DLHC_CTRL1_AXIS_Y_EN | LSM303DLHC_CTRL1_AXIS_Z_EN,
			IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
	SysCtlDelay(10000);
	LSM303DLHCAccelReadModifyWrite(&sLSM303DLHCAccel, LSM303DLHC_O_CTRL2, 0x00,
			0x00, IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
	SysCtlDelay(100000);

}

void initializeGyro(void) {
	uint8_t pui8Data[5];
	L3GD20HInit(&sl3gd20h, &g_sI2CInst, L3GD20H_I2C_ADDRESS, IMU10CallBack,
			&sl3gd20h);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
	SysCtlDelay(10000);
	L3GD20HReadModifyWrite(&sl3gd20h, L3GD20H_O_CTRL1, 0xF0, 0x0F,
			IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
	SysCtlDelay(10000);

	L3GD20HReadModifyWrite(&sl3gd20h, L3GD20H_O_CTRL4, ~L3GD20H_CTRL4_FS_M,
	L3GD20H_CTRL4_FS_245DPS, IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
	SysCtlDelay(10000);

	L3GD20HRead(&sl3gd20h, L3GD20H_O_CTRL1, pui8Data, 1, IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
}

void initializeImu(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	GPIOPinConfigure(GPIO_PD0_I2C3SCL);
	GPIOPinConfigure(GPIO_PD1_I2C3SDA);

	GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);

	I2CMInit(&g_sI2CInst, I2C3_BASE, INT_I2C3, 0xff, 0xff, SysCtlClockGet());
	initializeAccel();
	initializeGyro();
	calibrateAccel();
	calibrateAccel();

}

void readAccelMeasurements(float *resultsDestination) {
	readRawAccelMeasurements(resultsDestination);
	resultsDestination[0] -= accelOffset[0];
	resultsDestination[1] -= accelOffset[1];
	resultsDestination[2] -= accelOffset[2];
}

void readRawAccelMeasurements(float *resultsDestination) {
	LSM303DLHCAccelDataRead(&sLSM303DLHCAccel, IMU10CallBack, 0);

	while (!I2CDoneFlag) {
	}
	LSM303DLHCAccelDataAccelGetFloat(&sLSM303DLHCAccel, resultsDestination,
			resultsDestination + 1, resultsDestination + 2);
}

void readRawGyroMeasurements(float * resultsDestination) {
	copyReadings(resultsDestination, previousGyroReadings);
	L3GD20HDataRead(&sl3gd20h, IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}

	L3GD20HDataGyroGetFloat(&sl3gd20h, resultsDestination,
			resultsDestination + 1, resultsDestination + 2);
}

void computeSteepness(void) {
	float accelAngle = 0;
	readAccelMeasurements(accelerometerReadings);
	readRawGyroMeasurements(gyroReadings);
	surfaceAngle += gyroReadings[2] * (sportTimerTicks / 1000);
	accelAngle = (180
			* atan2f(accelerometerReadings[0], accelerometerReadings[1]))
			/ 3.14159;
	surfaceAngle = 0.99 * surfaceAngle + 0.01* accelAngle;
}

void copyReadings(float *source, float *destination) {
	int i = 0;
	for (; i < 3; i++) {
		destination[i] = source[i];

	}
}
