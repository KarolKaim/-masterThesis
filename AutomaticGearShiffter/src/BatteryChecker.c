#include "BatteryChecker.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "RGBIndicator.h"
#include "TimerInterruptHandlers.h"
uint32_t batteryLevelReadings[4] = { 0, 0, 0, 0 };
uint32_t batteryLevel = 0;

void adcIntHandler(void) {
	ADCIntClear(ADC0_BASE, 1);
	ADCSequenceDataGet(ADC0_BASE, 1, batteryLevelReadings);
	batteryLevel = (batteryLevelReadings[0] + batteryLevelReadings[1] + batteryLevelReadings[2] + batteryLevelReadings[3])/4;
	if(batteryLevel<= 2980)
	{
         setCurrentRgbState(red);
	}
	else if(batteryLevel < 3230)
	{
		setBlinkingState(red);
	}

}

void initializeBatteryLevelCheck(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH9);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH9);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH9);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 3,
			ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 1);
	ADCIntClear(ADC0_BASE, 1);
	ADCIntEnable(ADC0_BASE, 1);
	IntEnable(INT_ADC0SS1);
	IntMasterEnable();
    turnOnBatteryTimer();
}

void readBatteryLevel(void) {
	ADCIntClear(ADC0_BASE, 1);
	ADCProcessorTrigger(ADC0_BASE, 1);

}

