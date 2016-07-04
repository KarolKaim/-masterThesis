#include "TimerInterruptHandlers.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#include "MagneticSensors.h"
#include "DerailleurController.h"
#include "SwitchGear.h"

void wheelMagnetBouncingTimerHander(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
	wheelMagnetBouncingDelayInMs += 1;
	if (wheelMagnetBouncingDelayInMs == 20) {
		TimerDisable(TIMER1_BASE, TIMER_B);
		wheelMagnetBouncingDelayInMs = 0;
		wheelMagnetBouncingTimerActivated = false;
	}

}

void switchBouncingTimerHandler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	switchBouncingDelayInMs += 1;
	if (switchBouncingDelayInMs == 20) {
		TimerDisable(TIMER0_BASE, TIMER_B);
		switchBouncingDelayInMs = 0;
		switchBouncingTimerNotActivated = true;
	}

}

void wheelMagnetIntervalsTimerHander(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	timeSinceLastInterrupt += 1;

}

void derailleurControlGeneratorTimerHandler(void) {
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	currentPwmTicksInUs += 1;

	if (currentPwmTicksInUs == gearPositions[currentGear]) {
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);

	} else if (currentPwmTicksInUs == PWM_PERIOD_IN_HUND_OF_US) {
		currentPwmTicksInUs = 0;
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);
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
