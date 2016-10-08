#include "UartLogger.h"

#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "MagneticSensors.h"
#include "Imu.h"
#include "DerailleurController.h"
#include "SwitchGear.h"

void initializeUart(uint32_t speed) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, speed, 16000000);
}

void distributeFloat(int *intPart, int *fracPart, float *floatNumber) {
	*intPart = (int32_t) *floatNumber;
	*fracPart = (int32_t) (*floatNumber * 1000.0f);
	*fracPart = *fracPart - (*intPart * 1000);
	if (*fracPart < 0) {
		*fracPart *= -1;
	}
}

void sendSystemData() {
	float velocity = getBikeVelocityInKmPerH();
	int intCadence, fracCadence, intAngle, fracAngle, intVelo, fracVelo;
	distributeFloat(&intVelo, &fracVelo, &velocity);
	distributeFloat(&intCadence, &fracCadence, &cadenceInRPM);
	distributeFloat(&intAngle, &fracAngle, &surfaceAngle);
	UARTprintf("%d;%d;%d;%d;%d.%d;%d.%d;%d.%d", currentGear, currentMode,
			isGearDownSwitchPressed(), isGearUpSwitchPressed(), intVelo, fracVelo, intAngle, fracAngle,
			intCadence, fracCadence);
}

