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

//minimum value to get angle 0 - 11;
//maximu value to get angle 180 - 48
// firt gear - 17
uint32_t derailleurUp[8] = { 16, 18, 20, 22, 24, 26, 29, 30 };
uint32_t derailleurPositions[8] = { 17, 19, 21, 23, 25, 27, 29, 32 };
uint32_t derailleurDown[8] = { 17, 19, 21, 23, 25, 27, 29, 32 };
volatile uint32_t * currentDerailleurPosition = derailleurPositions;
int8_t currentGear = 0;
enum GearMode currentMode = remote;

void initializeGearController(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
	IntMasterEnable();
	initializeTimer(SYSCTL_PERIPH_TIMER2, TIMER2_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC, TIMER_A,
	TIMER_TIMA_TIMEOUT, 20000);
	initialzieCurrentDerailleurPosChangeTimer();
	IntEnable(INT_TIMER2A);
	TimerEnable(TIMER2_BASE, TIMER_A);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);

}

void changeCurrentGearMode(void) {
	switch (currentMode) {
	case remote:
		setCurrentRgbState(green);
		currentMode = comfort;
		turnOnMagneticSensorsTimers();
		turnOnComfortModeTimer();
		break;
	case comfort:
		turnOffComfortModeTimer();
		setCurrentRgbState(blue);
		currentMode = active;
		turnOnActiveModeTimer();
		break;
	case active:
		turnOffActiveModeTimer();
		setCurrentRgbState(pink);
		currentMode = sport;
		turnOnSportModeTimer();
		turnOnAccelReadingsTimer();
		break;
	case sport:
		turnOffSportModeTimer();
		turnOffAccelReadingsTimer();
		turnOffMagneticSensorsTimers();
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
		currentDerailleurPosition = derailleurDown;
		turnOnDerailleurChangeTimer();
	}
}

void reduceGear(void) {
	if (currentGear > 0) {
		currentGear--;
		currentDerailleurPosition = derailleurUp;
		turnOnDerailleurChangeTimer();

	}
}

void comfortModeHandler() {
	if(timeSinceLastWheelMagnetInt > 2000)
	{
		return;
	}
	if(timeSinceLastWheelMagnetInt > 2000)
	{
		return;
	}
	if (cadenceInRPM < 45) {
		reduceGear();
	} else if (cadenceInRPM > 60) {
		increaseGear();
	}
}

void activeModeHandler() {
	if(timeSinceLastWheelMagnetInt > 2000)
	{
		return;
	}
	if (cadenceInRPM < 52) {
		reduceGear();
	} else if (cadenceInRPM > 67) {
		increaseGear();
	}
}

void sportModeHandler() {
	if(timeSinceLastWheelMagnetInt > 2000)
	{
		return;
	}
	if (cadenceInRPM < 55) {
		reduceGear();
	} else if (cadenceInRPM > 74) {
		increaseGear();
	}
}
