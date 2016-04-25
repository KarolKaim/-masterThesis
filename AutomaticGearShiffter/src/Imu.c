#include "Imu.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "sensorlib/hw_mpu9150.h"
#include "sensorlib/hw_ak8975.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/ak8975.h"
#include "sensorlib/mpu9150.h"
#include "sensorlib/comp_dcm.h"

#include "sensorlib/hw_lsm303dlhc.h"
#include "sensorlib/lsm303dlhc_mag.h"
#include "sensorlib/lsm303dlhc_accel.h"
#include "sensorlib/l3gd20h.h"
#include "sensorlib/hw_l3gd20h.h"

tLSM303DLHCAccel sLSM303DLHCAccel;
tI2CMInstance g_sI2CInst;

volatile uint_fast8_t I2CDoneFlag;
volatile uint_fast8_t ErrorFlag;

void IMU10CallBack(void *pvCallbackData, uint_fast8_t ui8Status) {

	if (ui8Status == I2CM_STATUS_SUCCESS) {
		I2CDoneFlag = 1;
	}

	ErrorFlag = ui8Status;
}

void imuI2CHandler(void) {

	I2CMIntHandler(&g_sI2CInst);
}

void initializeImu(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	GPIOPinConfigure(GPIO_PD0_I2C3SCL);
	GPIOPinConfigure(GPIO_PD1_I2C3SDA);

	GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);

	I2CMInit(&g_sI2CInst, I2C3_BASE, INT_I2C3, 0xff, 0xff, SysCtlClockGet());

	/***************************	Accelerometer initialization **************************************/
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
	SysCtlDelay(10000000);
	LSM303DLHCAccelReadModifyWrite(&sLSM303DLHCAccel, LSM303DLHC_O_CTRL2, 0x00,
			0x00, IMU10CallBack, 0);
	while (!I2CDoneFlag) {
	}
	I2CDoneFlag = 0;
	SysCtlDelay(10000000);

}

void readAccelMeasurements(float resultsDestination[3]) {
	LSM303DLHCAccelDataRead(&sLSM303DLHCAccel, IMU10CallBack, 0);

	while (!I2CDoneFlag) {
	}

	LSM303DLHCAccelDataAccelGetFloat(&sLSM303DLHCAccel, resultsDestination,
			resultsDestination + 1, resultsDestination + 2);
}
