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

uint32_t bicycleVelocity = 0;

uint32_t wheelMagnetBouncingDelayInMs = 0;
uint32_t timeSinceLastInterrupt = 0;
bool wheelMagnetBouncingTimerActivated = false;

void handleWheelMagnetInt(void) {
	if (!wheelMagnetBouncingTimerActivated) {
		TimerEnable(TIMER1_BASE, TIMER_A);
		TimerEnable(TIMER1_BASE, TIMER_B);
		wheelMagnetBouncingTimerActivated = true;
		computeBicycleVelocityInMetersPerSecond();
		timeSinceLastInterrupt = 0;
	}
}

void handleCrankMagnetInt(void){
	//TODO - check magnetic filters and decide whether delay timers are required or not

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
	IntEnable(INT_TIMER1B);
	IntEnable(INT_GPIOA);

	GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_2 || GPIO_PIN_3);
}

void computeBicycleVelocityInMetersPerSecond() {
	bicycleVelocity = 2 * PI * WHEELRADIUS / (0.001 * timeSinceLastInterrupt);
}
