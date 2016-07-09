#include "RGBIndicator.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

/*
 *      GPIO_PIN_5 - red
 *      GPIO_PIN_6 - green
 *      GPIO_PIN_7 - blue
 */
void initializeRGBIndicator(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	setCurrentRgbState(yellow);
}

void setCurrentRgbState(enum RgbColours currentColor) {
	switch (currentColor) {
	case red:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, red);
		break;
	case green:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, green);
		break;
	case yellow:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, yellow);
		break;
	case blue:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, blue);
		break;
	case pink:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, pink);
		break;
	case cyan:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, cyan);
		break;
	case white:
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, white);
		break;
	default:
		break;
	}
}
