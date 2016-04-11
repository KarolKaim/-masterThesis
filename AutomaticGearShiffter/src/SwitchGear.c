#include "SwitchGear.h"

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

volatile uint32_t DelayConuter = 0;

void startAntiBouncingTimer() {

	IntDisable(INT_GPIOA);
	IntDisable(INT_GPIOB);
	TimerEnable(TIMER0_BASE, TIMER_B);
}


void switchBouncingTimerHandler(void) {

	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	DelayConuter += 1;
	if (DelayConuter == 50) {
		IntEnable(INT_GPIOA);
		IntEnable(INT_GPIOB);
		TimerDisable(TIMER0_BASE, TIMER_B);
		DelayConuter = 0;
	}
}

void initializeSwitches(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6);

	//GPIOIntRegister(GPIO_PORTA_BASE, gearUpHandler);
	//GPIOIntRegister(GPIO_PORTB_BASE, gearDownHandler);

	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_RISING_EDGE);

	GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_4);
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_6);

	IntEnable(INT_GPIOA);
	IntEnable(INT_GPIOB);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_B, SysCtlClockGet() / 1000);
	IntMasterEnable();
	TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	IntEnable(INT_TIMER0B);
}
