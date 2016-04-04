#include "MagneticSensors.h"

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

void wheelMagnetHandler(void) {
	GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_7);
	int a;
	a = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) >> 2 ^ 1) << 2;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, a);
}
void initializeMagneticSensors(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7);
	GPIOIntRegister(GPIO_PORTC_BASE, wheelMagnetHandler);
	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_RISING_EDGE);
	GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_7);

	IntEnable(INT_GPIOC);
}
