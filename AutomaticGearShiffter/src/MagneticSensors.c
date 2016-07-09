#include "MagneticSensors.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "TimerInterruptHandlers.h"

#define PI 3,14159
#define WHEELRADIUS 0.335

float bicycleVelocityInMetersPerSeconds = 0;
float cadenceInRPM = 0;

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

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 || GPIO_PIN_3);
	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2 || GPIO_PIN_3, GPIO_RISING_EDGE);

	initializeTimer(SYSCTL_PERIPH_TIMER1, TIMER1_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC, TIMER_A,
	TIMER_TIMA_TIMEOUT, 1000);

	initializeTimer(SYSCTL_PERIPH_TIMER1, TIMER1_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC, TIMER_B,
	TIMER_TIMB_TIMEOUT, 1000);

	IntMasterEnable();
	IntEnable(INT_TIMER1A);
	IntEnable(INT_TIMER1B);
	IntEnable(INT_GPIOA);

	GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_2 || GPIO_PIN_3);
	TimerEnable(TIMER1_BASE, TIMER_A);
	TimerEnable(TIMER1_BASE, TIMER_B);
}

void computeVelocityInMetersPerSecond() {
	if (timeSinceLastWheelMagnetInt > 0) {
		bicycleVelocityInMetersPerSeconds = 2 * PI * WHEELRADIUS
				/ (0.001 * timeSinceLastWheelMagnetInt);
	} else {
		bicycleVelocityInMetersPerSeconds = 0;
	}
}

void computeCadence() {
	if (timeSinceLastCrankMagnetInt > 0) {
		cadenceInRPM = 60 / timeSinceLastCrankMagnetInt;
	} else {
		cadenceInRPM = 0;
	}

}
