#include "GpioInterruptHandlers.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "MagneticSensors.h"

void gpioPortA(void) {
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_4);
	IntDisable(INT_GPIOA);
	IntDisable(INT_GPIOB);
	int32_t a = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) >> 2 ^ 1) << 2;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, a);
}

void gpioPortB(void) {

	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_6);
	int a;
	a = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) >> 2 ^ 1) << 2;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, a);
}

void gpioPortC(void) {

	uint32_t status = clearGpioInt(GPIO_PORTC_BASE);

	if ((status & GPIO_PIN_7) == GPIO_PIN_7) {

		handleWheelMagnetInt();
	}
}

uint32_t clearGpioInt(uint32_t intPort) {
	uint32_t status = 0;
	status = GPIOIntStatus(intPort, true);
	GPIOIntClear(intPort, status);
	return status;
}

