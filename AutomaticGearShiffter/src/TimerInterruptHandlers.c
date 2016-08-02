#include "TimerInterruptHandlers.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "MagneticSensors.h"
#include "DerailleurController.h"
#include "SwitchGear.h"
#include "GpioInterruptHandlers.h"
#include "Imu.h"

/*
 *_____Timers used in application______
 *
 *  Timer0A - sport mode accel readings
 *
 *	Timer1A - wheel rotational speed
 *	Timer1B - crank rotational speed
 *
 *	Timer2A - PWM signal generation
 *
 *	Timer3A - continous gear change
 *	Timer3B - comfort mode
 *
 *	Timer4A - change current derailleurs position
 *
 *  Timer5A - active mode
 *  Timer5B - sport mode
 */

uint16_t continuousChangeTimerMs = 0;
uint16_t derailleurChangePosTimer = 0;
uint16_t currentAutomaticChangeTimer = 0;
uint16_t accelReadingsTimer = 0;

bool sportModeTimerDone = false;
uint16_t sportTimerTicks = 10;

void wheelMagnetTimerHander(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	timeSinceLastWheelMagnetInt += 1;

}

void crankMagnetTimerHander(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
	timeSinceLastCrankMagnetInt += 1;
}

void continuousChangeTimerHandler(void) {
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	continuousChangeTimerMs += 1;
	if (continuousChangeTimerMs == 1000) {

		if (isGearDownSwitchPressed() && isGearUpSwitchPressed()) {
			changeCurrentGearMode();
		}
		if (isGearUpSwitchPressed()) {
			reduceGear();
		} else if (isGearDownSwitchPressed()) {
			increaseGear();
		} else {
			turnOffConitnousChangeTimer();
		}
		continuousChangeTimerMs = 0;
		enableSwitches();
	}
}

void derailleurControlGeneratorTimerHandler(void) {
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	currentPwmTicksInUs += 1;

	if (currentPwmTicksInUs == *(currentDerailleurPosition + currentGear)) {
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);

	} else if (currentPwmTicksInUs == PWM_PERIOD_IN_HUND_OF_US) {
		currentPwmTicksInUs = 0;
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);
	}
}

void derailleurChangePositionTimerHandler(void) {
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	derailleurChangePosTimer += 1;
	if (derailleurChangePosTimer == 500) {
		currentDerailleurPosition = derailleurPositions;
		turnOffDerailleurChangeTimer();
		derailleurChangePosTimer = 0;

	}
}

void comfortModeTimerHandler(void) {
	TimerIntClear(TIMER3_BASE, TIMER_TIMB_TIMEOUT);
	currentAutomaticChangeTimer += 1;
	if (currentAutomaticChangeTimer > 2500) {
		currentAutomaticChangeTimer = 0;
		comfortModeHandler();

	}
}

void activeModeTimerHandler(void) {
	TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
	currentAutomaticChangeTimer += 1;
	if (currentAutomaticChangeTimer > 1500) {
		currentAutomaticChangeTimer = 0;
		activeModeHandler();

	}
}
void sportModeTimerHandler(void) {
	TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);
	currentAutomaticChangeTimer += 1;
	if (currentAutomaticChangeTimer > 1000) {
		currentAutomaticChangeTimer = 0;
		sportModeHandler();

	}
}

void sportModeAccelReadings(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	accelReadingsTimer += 1;
	if (accelReadingsTimer == sportTimerTicks) {
		accelReadingsTimer = 0;
		sportModeTimerDone = true;
	}

}
void initializeTimer(uint32_t timerPeripheral, int32_t timerBase,
		uint32_t timerConfig, uint32_t timer, uint32_t intFlags,
		uint32_t secondsDivider) {
	SysCtlPeripheralEnable(timerPeripheral);
	TimerConfigure(timerBase, timerConfig);
	TimerLoadSet(timerBase, timer, SysCtlClockGet() / secondsDivider);
	TimerIntEnable(timerBase, intFlags);
}

void initialzeComfModeAndContChangeTimers(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER3, TIMER3_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC,
	TIMER_A | TIMER_B,
	TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT, 1000);
	IntEnable(INT_TIMER3A);
	IntEnable(INT_TIMER3B);
}

void initializeMagneticSensorsTimers(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER1, TIMER1_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC,
	TIMER_A | TIMER_B,
	TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT, 1000);
	IntEnable(INT_TIMER1A);
	IntEnable(INT_TIMER1B);
}

void initialzieCurrentDerailleurPosChangeTimer(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER4, TIMER4_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC, TIMER_A,
	TIMER_TIMA_TIMEOUT, 1000);
	IntEnable(INT_TIMER4A);
}

void initializeActiveAndSportModeTimers(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER5, TIMER5_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC,
	TIMER_A | TIMER_B,
	TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT, 1000);
	IntEnable(INT_TIMER5A);
	IntEnable(INT_TIMER5B);
}

void initializeSportModeAccelReadings(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER0, TIMER0_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC,
	TIMER_A,
	TIMER_TIMA_TIMEOUT, 1000);
	IntEnable(INT_TIMER0A);
}

void turnOnAccelReadingsTimer(void) {
	TimerEnable(TIMER0_BASE, TIMER_A);
}

void turnOffAccelReadingsTimer(void) {
	TimerDisable(TIMER0_BASE, TIMER_A);
}

void turnOnDerailleurChangeTimer(void) {
	TimerEnable(TIMER4_BASE, TIMER_A);
}

void turnOffDerailleurChangeTimer(void) {
	TimerDisable(TIMER4_BASE, TIMER_A);
}

void turnOnComfortModeTimer(void) {
	TimerEnable(TIMER3_BASE, TIMER_B);
}

void turnOffComfortModeTimer(void) {
	TimerDisable(TIMER3_BASE, TIMER_B);
}

void turnOnConitnousChangeTimer(void) {
	TimerEnable(TIMER3_BASE, TIMER_A);
}

void turnOffConitnousChangeTimer(void) {
	TimerDisable(TIMER3_BASE, TIMER_A);
}

void turnOnActiveModeTimer(void) {
	TimerEnable(TIMER5_BASE, TIMER_A);
}

void turnOffActiveModeTimer(void) {
	TimerDisable(TIMER5_BASE, TIMER_A);
}

void turnOnSportModeTimer(void) {
	TimerEnable(TIMER5_BASE, TIMER_B);
}

void turnOffSportModeTimer(void) {
	TimerDisable(TIMER5_BASE, TIMER_B);
}

void turnOnMagneticSensorsTimers(void) {
	TimerEnable(TIMER1_BASE, TIMER_A | TIMER_B);
}

void turnOffMagneticSensorsTimers(void) {
	TimerDisable(TIMER1_BASE, TIMER_A | TIMER_B);
}
