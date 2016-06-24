#include "SwitchGear.h"

#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "TimerInterruptHandlers.h"
#include "DerailleurController.h"

uint32_t switchBouncingDelayInMs = 0;
bool switchBouncingTimerNotActivated = true;

void handleGearUp(void) {

	if (switchBouncingTimerNotActivated) {
		TimerEnable(TIMER0_BASE, TIMER_B);
		switchBouncingTimerNotActivated = false;
		if (currentGear < 7) {
			currentGear++;
		}
	}
}

void handleGearDown(void) {

	if (switchBouncingTimerNotActivated) {
		TimerEnable(TIMER0_BASE, TIMER_B);
		switchBouncingTimerNotActivated = false;
		if (currentGear > 0) {
			currentGear--;
		}
	}
}

void initializeSwitches(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6);

	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);

	GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_4);
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_6);

	IntEnable(INT_GPIOA);
	IntEnable(INT_GPIOB);

	initializeTimer(SYSCTL_PERIPH_TIMER0, TIMER0_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC, TIMER_B,
	TIMER_TIMB_TIMEOUT, 1000);
	IntEnable(INT_TIMER0B);
}
