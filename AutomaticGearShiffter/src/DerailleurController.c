#include "DerailleurController.h"

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
#include "Imu.h"

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
bool gearChangedManualy = false;
uint8_t secondsCounter = 0;
bool peddalingStoped = false;
bool peddalingWasStopped = false;

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
		gearChangedManualy = false;
		//turnOnAccelReadingsTimer();
		break;
	case sport:
		turnOffSportModeTimer();
		//turnOffAccelReadingsTimer();
		turnOffMagneticSensorsTimers();
		setCurrentRgbState(yellow);
		currentMode = remote;
		peddalingStoped = false;
		peddalingWasStopped = false;
		gearChangedManualy = false;
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
	if (timeSinceLastWheelMagnetInt > 2000) {
		bicycleVelocityInMetersPerSeconds = 0;
		return;
	}
	if (timeSinceLastCrankMagnetInt > 1800) {
		cadenceInRPM = 0;
		return;
	}
	changeGearInCadenceRange(45, 60);
}

void activeModeHandler() {

	//reset velocity
	if (timeSinceLastWheelMagnetInt > 2000) {
		bicycleVelocityInMetersPerSeconds = 0;
		return;
	}
	if (gearChangedManualy) {
		secondsCounter += 1;
		if (secondsCounter == 7) {
			gearChangedManualy = false;
		} else {
			return;
		}
	}
	if (timeSinceLastCrankMagnetInt > 1800) {
		cadenceInRPM = 0;
		peddalingStoped = true;
		return;
	}
	changeGearInCadenceRange(55, 70);
}

void sportModeHandler() {
	if (timeSinceLastWheelMagnetInt > 2000) {
		bicycleVelocityInMetersPerSeconds = 0;
		return;
	}
	if (gearChangedManualy) {
		secondsCounter += 1;
		if (secondsCounter == 7) {
			gearChangedManualy = false;
		} else {
			return;
		}
	}

	if (timeSinceLastCrankMagnetInt > 1800) {
		cadenceInRPM = 0;
		peddalingStoped = true;
		peddalingWasStopped = true;
		return;
	}
    if(peddalingWasStopped)
    {
    	peddalingWasStopped = false;
    	return;
    }
	if (surfaceAngle < -3 || surfaceAngle > 3) {
		changeGearInCadenceRange(55, 70);
	} else {
		changeGearInCadenceRange(65, 80);
	}
}

void changeGearInCadenceRange(uint8_t lowerLimit, uint8_t upperLimit) {
	if (cadenceInRPM < lowerLimit) {
		reduceGear();
	} else if (cadenceInRPM > upperLimit) {
		increaseGear();
	}
}

void setGearAccordingToSpeed() {
	uint8_t bikeVeloInKmPerH = getBikeVelocityInKmPerH();
	if (bikeVeloInKmPerH < 2) {
		currentGear = 0;
	} else if (bikeVeloInKmPerH < 5) {
		currentGear = 1;
	} else if (bikeVeloInKmPerH < 10) {
		currentGear = 2;
	} else if (bikeVeloInKmPerH < 14) {
		currentGear = 3;
	} else if (bikeVeloInKmPerH < 18) {
		currentGear = 4;
	} else if (bikeVeloInKmPerH < 22) {
		currentGear = 5;
	} else if (bikeVeloInKmPerH < 26) {
		currentGear = 6;
	} else if (bikeVeloInKmPerH < 100) {
		currentGear = 7;
	}
}
