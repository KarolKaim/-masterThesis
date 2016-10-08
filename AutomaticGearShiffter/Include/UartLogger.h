#ifndef INCLUDE_UARTLOGGER_H_
#define INCLUDE_UARTLOGGER_H_

#include <stdint.h>

void initializeUart(uint32_t speed);
void uartPrintFloat(float floatNumber);
void distributeFloat(int *intPart, int *fracPart, float *floatNumber);
void sendSystemData(void);

#endif
