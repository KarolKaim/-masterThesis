#include "RGBIndicator.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "TimerInterruptHandlers.h"

/*
 *      GPIO_PIN_1 - red
 *      GPIO_PIN_2 - blue
 *      GPIO_PIN_3 - green
 *
 */

enum RgbColours currentRgbColor = yellow;
bool blinkingModeTurnedOn = false;

void initializeRGBIndicator(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
	GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	setCurrentRgbState(yellow);
}

void setCurrentRgbState(enum RgbColours currentColor) {
	if(blinkingModeTurnedOn)
	{
		turnOffRgbTimer();
		blinkingModeTurnedOn = false;
	}
	currentRgbColor = currentColor;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
			currentRgbColor);

}

void setBlinkingState(enum RgbColours currentColor) {
	turnOnRgbTimer();
	currentRgbColor = currentColor;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
			currentRgbColor);
	blinkingModeTurnedOn = true;

}
