#include "UartLogger.h"

#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "driverlib/uart.h"
#include "utils/uartstdio.h"

void initializeUart(uint32_t speed) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, speed, 16000000);
}

void printFloat(float * Data)
{
	int_fast32_t i32IPart, i32FPart;
	float pfData;

	pfData=*Data;


	  i32IPart = (int32_t) pfData;
    i32FPart = (int32_t) (pfData * 1000.0f);
    i32FPart = i32FPart -(i32IPart * 1000);

    if(i32FPart < 0)
    {
      i32FPart*= -1;
		}

		UARTprintf("%3d.%3d ", i32IPart, i32FPart);
	}
