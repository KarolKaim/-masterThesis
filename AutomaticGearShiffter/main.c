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
#include "BatteryChecker.h"

int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_16MHZ);
	initialzeComfModeAndContChangeTimers();
	initializeActiveAndSportModeTimers();
	initializeSportModeAccelReadingsAndPosChange();

	initializeMagneticSensors();
	//initializeImu();
	initializeGearController();
	initializeSwitches();
	initializeUart(115200);
	initialzeRgbAndBatteryLevelCheckTimers();
	initializeBatteryLevelCheck();
	initializeRGBIndicator();


	currentMode = active;
	changeCurrentGearMode();

	while (true) {
		if (sportModeTimerDone && currentMode == sport) {
//			sportModeTimerDone = false;
//			computeSteepness();
//			if (surfaceAngle < 0 && surfaceAngle > -1) {
//				UARTprintf("-");
//			}
//			printFloat(&surfaceAngle);
//			printFloat(&tmpSurfaceAngle);
//
//			UARTprintf("\n");

		}

	}
	return 0;
}
