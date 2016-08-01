#ifndef INCLUDE_GPIOINTERRUPTHANDLERS_H_
#define INCLUDE_GPIOINTERRUPTHANDLERS_H_

#include <stdint.h>
#include <stdbool.h>

uint32_t clearGpioInt(uint32_t intPort);
void initializeGpioAsSwitch(uint32_t portClk, uint32_t gpioPort, uint8_t pins,
		uint32_t intType, uint32_t portInt);
void turnOnGPIO(uint32_t gpioPort, uint8_t pins);
void turnOffGPIO(uint32_t gpioPort, uint8_t pins);
#endif
