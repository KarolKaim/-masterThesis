#include "TimerInterruptHandlers.h"

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#include "MagneticSensors.h"

void wheelMagnetBouncingTimerHander(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
	wheelMagnetBouncingDelayInMs += 1;
	if (wheelMagnetBouncingDelayInMs == 20) {
		TimerDisable(TIMER1_BASE, TIMER_B);
		wheelMagnetBouncingDelayInMs = 0;
		wheelMagnetBouncingTimerActivated = false;
	}

}

void wheelMagnetIntervalsTimerHander(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	wheelMagnetIntervalInMs += 1;

}

void initializeTimer(uint32_t timerPeripheral, int32_t timerBase,
		uint32_t timerConfig, uint32_t timer, uint32_t intFlags,
		uint32_t secondsDivider) {
	SysCtlPeripheralEnable(timerPeripheral);
	TimerConfigure(timerBase, timerConfig);
	TimerLoadSet(timerBase, timer, SysCtlClockGet() / secondsDivider);
	TimerIntEnable(timerBase, intFlags);
}
