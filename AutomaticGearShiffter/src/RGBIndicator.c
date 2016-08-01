#include "RGBIndicator.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

/*
 *      GPIO_PIN_1 - red
 *      GPIO_PIN_2 - blue
 *      GPIO_PIN_3 - green
 *
 */
void initializeRGBIndicator(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	setCurrentRgbState(yellow);
}

void setCurrentRgbState(enum RgbColours currentColor) {
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_3,
			currentColor);

}
