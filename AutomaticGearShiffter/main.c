#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "SwitchGear.h"
#include "MagneticSensors.h"
#include "UartLogger.h"
#include "Imu.h"
#include "DerailleurController.h"
#include "TimerInterruptHandlers.h"
#include "RGBIndicator.h"

int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_16MHZ);
	initialzeComfModeAndContChangeTimers();
	initializeActiveAndSportModeTimers();
	initializeSportModeAccelReadings();

	initializeMagneticSensors();
	initializeImu();
	initializeGearController();
	initializeSwitches();
	initializeUart(115200);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
	GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, yellow);

	while (true) {
		if (sportModeTimerDone && currentMode == sport) {
			sportModeTimerDone = false;
			computeSteepness();

		}


	}
	return 0;
}
