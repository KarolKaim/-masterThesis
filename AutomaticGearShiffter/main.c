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

int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_16MHZ);

	initializeSwitches();
	initializeMagneticSensors();
	//initializeUart(115200);
	//UARTprintf("Rakieta !!!!\n");
	//initializeImu();
	initializeGearController();

	while (true) {
	}
	return 0;
}
