#include "DerailleurController.h"

#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "TimerInterruptHandlers.h"

uint32_t currentPwmTicksInUs = 0;
//minimum value to get position 0 - 11;
//maximu value to get position 180 - 48
// firt gear - 17
uint32_t gearPositions[8] = { 17, 19, 21, 23, 25, 27, 29, 32};
int8_t currentGear = 0;

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

