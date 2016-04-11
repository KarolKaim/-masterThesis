#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "SwitchGear.h"
#include "MagneticSensors.h"

int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);

	//initializeSwitches();
	initializeMagneticSensors();

	while (true) {
	}
	return 0;
}
