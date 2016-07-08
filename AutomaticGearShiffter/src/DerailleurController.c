#include "DerailleurController.h"

#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "TimerInterruptHandlers.h"
#include "SwitchGear.h"
#include "MagneticSensors.h"
#include "RGBIndicator.h"

uint32_t currentPwmTicksInUs = 0;
uint16_t currentComfortModeTimer = 0;
//minimum value to get angle 0 - 11;
//maximu value to get angle 180 - 48
// firt gear - 17
uint32_t gearPositions[8] = { 17, 19, 21, 23, 25, 27, 29, 32 };
int8_t currentGear = 0;
enum GearMode currentMode = remote;

void initializeGearController(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
	IntMasterEnable();
	initializeTimer(SYSCTL_PERIPH_TIMER2, TIMER2_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC, TIMER_A,
	TIMER_TIMA_TIMEOUT, 20000);
	IntEnable(INT_TIMER2A);
	TimerEnable(TIMER2_BASE, TIMER_A);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);

}

void changeCurrentGearMode(void) {
	switch (currentMode) {
	case remote:
		setCurrentRgbState(green);
		currentMode = comfort;
		disableSwitches();
		turnOnComfortModeTimer();
		break;
	case comfort:
		setCurrentRgbState(blue);;
		currentMode = active;
		enableSwitches();
		turnOffComfortModeTimer();
		break;
	case active:
		setCurrentRgbState(pink);
		currentMode = sport;
		break;
	case sport:
		setCurrentRgbState(yellow);
		currentMode = remote;
		break;
	default:
		break;
	}
}

void increaseGear(void) {
	if (currentGear < 7) {
		currentGear++;
	}
}

void reduceGear(void) {
	if (currentGear > 0) {
		currentGear--;
	}
}

void comfortModeHandler() {
	if (cadenceInRPM < 50) {
		reduceGear();
	} else if (cadenceInRPM > 70) {
		increaseGear();
	}
}
