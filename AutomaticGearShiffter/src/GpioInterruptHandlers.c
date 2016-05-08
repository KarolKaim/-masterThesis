#include "GpioInterruptHandlers.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "MagneticSensors.h"
#include "SwitchGear.h"

void gpioPortA(void) {
	uint32_t status = clearGpioInt(GPIO_PORTA_BASE);
	if ((status & GPIO_PIN_4) == GPIO_PIN_4) {
		handleGearUp();
	}
}

void gpioPortB(void) {
	uint32_t status = clearGpioInt(GPIO_PORTB_BASE);
	if ((status & GPIO_PIN_6) == GPIO_PIN_6) {
		handleGearDown();
	}
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

