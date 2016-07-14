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

/*
 *_____Timers used in application______
 *
 *	Timer1A - wheel rotational speed
 *	Timer1B - crank rotational speed
 *
 *	Timer2A - PWM signal generation
 *
 *	Timer3A - continous gear change
 *	Timer3B - periodic timer for comfort mode
 *
 *	Timer4A - change current derailleurs position
 *
 */

uint16_t continuousChangeTimerMs = 0;
uint16_t derailleurChangePosTimer = 0;

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
	if (continuousChangeTimerMs == 200) {
		if (isGearUpSwitchPressed()) {
			reduceGear();
		} else if (isGearDownSwitchPressed()) {
			increaseGear();
		} else {
			turnOffConitnousChangeTimer();
		}
		continuousChangeTimerMs = 0;

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
	TimerIntClear(TIMER4_BASE, TIMER_TIMB_TIMEOUT);
	currentComfortModeTimer += 1;
	if (currentComfortModeTimer == 3000) {
		currentComfortModeTimer = 0;
		comfortModeHandler();

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

void turnOnConitnousChangeTimer(void) {
	TimerEnable(TIMER3_BASE, TIMER_A);
}

void turnOffConitnousChangeTimer(void) {
	TimerDisable(TIMER3_BASE, TIMER_A);
}

void initialzeComfModeAndContChangeTimers(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER3, TIMER3_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC,
			TIMER_A | TIMER_B,
			TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT, 1000);
	IntEnable(INT_TIMER3A);
}

void initialzieCurrentDerailleurPosChangeTimer(void) {
	initializeTimer(SYSCTL_PERIPH_TIMER4, TIMER4_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC, TIMER_A,
	TIMER_TIMA_TIMEOUT, 1000);
	IntEnable(INT_TIMER4A);
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
