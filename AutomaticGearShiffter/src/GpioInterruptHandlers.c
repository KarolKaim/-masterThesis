#include "GpioInterruptHandlers.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "MagneticSensors.h"
#include "SwitchGear.h"
#include "DerailleurController.h"
#include "TimerInterruptHandlers.h"

/*
 *  GPIO Input pins:
 *
 *  GPIO_A2 - wheel magnet
 *  GPIO_A3 - crank magnet
 *  GPIO_B6 - gear Up switch
 *  GPIO_B7 - gear down switch
 *  GPIO_C4 - gear mode switch
 *
 *  GPIO Output pins:
 *  GPIO_C5 - RBG indication
 *  GPIO_C6 - RBG indication
 *  GPIO_C7 - RBG indication
 *  GPIO_D0 - I2C SCL
 *  GPIO_D1 - I2D SDA
 *  GPIO_E2 - servo control
 *
 */

void gpioPortA(void) {
	uint32_t status = clearGpioInt(GPIO_PORTA_BASE);
	if ((status & GPIO_PIN_2) == GPIO_PIN_2) {
		handleWheelMagnetInt();
	} else if((status & GPIO_PIN_3) == GPIO_PIN_3){
		handleCrankMagnetInt();

	}
}

void gpioPortB(void) {
	uint32_t status = clearGpioInt(GPIO_PORTB_BASE);
	if ((status & GPIO_PIN_6) == GPIO_PIN_6) {
		reduceGear();
	} else if ((status & GPIO_PIN_7) == GPIO_PIN_7) {
		increaseGear();
	}
	turnOnConitnousChangeTimer();
}

void gpioPortC(void) {
	uint32_t status = clearGpioInt(GPIO_PORTC_BASE);

	if ((status & GPIO_PIN_4) == GPIO_PIN_4) {
		changeCurrentGearMode();
	}
}

uint32_t clearGpioInt(uint32_t intPort) {
	uint32_t status = 0;
	status = GPIOIntStatus(intPort, true);
	GPIOIntClear(intPort, status);
	return status;
}

