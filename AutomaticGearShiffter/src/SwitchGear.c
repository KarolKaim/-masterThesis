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
#include "GpioInterruptHandlers.h"

void initializeSwitches(void) {
	initializeGpioAsSwitch(SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE,
	GPIO_PIN_0 | GPIO_PIN_1, GPIO_FALLING_EDGE, INT_GPIOB);

}

bool isGearUpSwitchPressed(void) {
	return (!GPIOPinRead(GPIO_PORTB_BASE,
	GPIO_PIN_0)) && GPIO_PIN_0;
}

bool isGearDownSwitchPressed(void) {
	return (!GPIOPinRead(GPIO_PORTB_BASE,
	GPIO_PIN_1)) && GPIO_PIN_1;
}

void disableSwitches(void) {
	GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void enableSwitches(void) {
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

