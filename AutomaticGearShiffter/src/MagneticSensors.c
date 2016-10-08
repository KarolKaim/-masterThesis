#include "MagneticSensors.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "TimerInterruptHandlers.h"

#define PI 3.14159
#define WHEELRADIUS 0.335

float bicycleVelocityInMetersPerSeconds = 0;
float cadenceInRPM = 0;
float oldCadence = 0;
float derivateOfCaddence = 0;

uint32_t timeSinceLastWheelMagnetInt = 0;
uint32_t timeSinceLastCrankMagnetInt = 0;

void handleWheelMagnetInt(void) {
	computeVelocityInMetersPerSecond();
	timeSinceLastWheelMagnetInt = 0;
}

void handleCrankMagnetInt(void) {
	computeCadence();
	timeSinceLastCrankMagnetInt = 0;
}

void initializeMagneticSensors(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4);
	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4, GPIO_FALLING_EDGE);
	IntMasterEnable();
	IntEnable(INT_GPIOA);
	GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4);
	initializeMagneticSensorsTimers();
	TimerEnable(TIMER1_BASE, TIMER_A);
	TimerEnable(TIMER1_BASE, TIMER_B);
}

void computeVelocityInMetersPerSecond() {
	if (timeSinceLastWheelMagnetInt > 20) {
		bicycleVelocityInMetersPerSeconds = 2 * PI * WHEELRADIUS
				/ (0.001 * timeSinceLastWheelMagnetInt);
	}
}

float getBikeVelocityInKmPerH() {
	return 3.6 * bicycleVelocityInMetersPerSeconds;
}

void computeCadence() {
	oldCadence = cadenceInRPM;
	float tmpCadence = 0;
	if (timeSinceLastCrankMagnetInt > 0) {
		tmpCadence = 60 / (0.001 * timeSinceLastCrankMagnetInt);
		derivateOfCaddence = (tmpCadence - oldCadence)
				/ timeSinceLastCrankMagnetInt;
		if (derivateOfCaddence < 1) {
			cadenceInRPM = tmpCadence;
		}
	}
}
