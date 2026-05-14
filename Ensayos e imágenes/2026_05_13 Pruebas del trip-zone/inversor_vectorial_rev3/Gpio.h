/**
 *
 */

#ifndef GPIO_H
#define GPIO_H
#include "project.h"

#define CAC1_CLOSE GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1; // Cierra el contactor de CA
#define CAC1_OPEN  GpioDataRegs.GPCSET.bit.GPIO87 = 1;    // Abre el contactor de CA

#define CDC1_CLOSE GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1; // Cierra el contactor de CC
#define CDC1_OPEN GpioDataRegs.GPBSET.bit.GPIO32 = 1; // Abre el contactor de CC



extern void InitGpio(void);

#endif
