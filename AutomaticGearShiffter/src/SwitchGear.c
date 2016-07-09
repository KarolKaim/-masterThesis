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

void initializeSwitches(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	IntEnable(INT_GPIOB);
	initialzeContinuousGearChangeTimer();
}

bool isGearUpSwitchPressed(void) {
	return (GPIOPinRead(GPIO_PORTB_BASE,
	GPIO_PIN_6) && GPIO_PIN_6) == GPIO_PIN_6;
}

bool isGearDownSwitchPressed(void) {
	return (GPIOPinRead(GPIO_PORTB_BASE,
	GPIO_PIN_6) && GPIO_PIN_6) == GPIO_PIN_6;
}

void disableSwitches(void) {
	GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
}

void enableSwitches(void) {
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
}

