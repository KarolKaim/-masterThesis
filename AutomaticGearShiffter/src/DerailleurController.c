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
uint32_t gearPositions[8] = { 18, 25, 29, 32, 35, 36, 37, 38};
int8_t currentGear = 0;

void initializeGearController(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	IntMasterEnable();
	initializeTimer(SYSCTL_PERIPH_TIMER2, TIMER2_BASE,
	TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC, TIMER_A,
	TIMER_TIMA_TIMEOUT, 20000);
	IntEnable(INT_TIMER2A);
	TimerEnable(TIMER2_BASE, TIMER_A);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);

}

