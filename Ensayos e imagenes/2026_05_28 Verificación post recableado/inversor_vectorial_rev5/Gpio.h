/**
 *
 */

#ifndef GPIO_H
#define GPIO_H
#include "project.h"

// **** GRUPO A
#define PRECHARGE_CLOSE GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; // Cierra el rele de IPM
#define PRECHARGE_OPEN GpioDataRegs.GPASET.bit.GPIO10 = 1;    // Abre el rele de IPM

#define GET_CAC1_STATE (GpioDataRegs.GPADAT.bit.GPIO22) // Estado del contactor de AC

#define GET_QAC1_STATE (GpioDataRegs.GPADAT.bit.GPIO16) // Estado de la termica de AC
#define GET_QDC1_STATE (GpioDataRegs.GPADAT.bit.GPIO18) // Estado de la termica de CC

// **** GRUPO B
#define CDC1_CLOSE GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1; // Cierra el contactor de CC
#define CDC1_OPEN GpioDataRegs.GPBSET.bit.GPIO32 = 1;    // Abre el contactor de CC

#define IPM_CLOSE GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1; // Cierra el rele de IPM
#define IPM_OPEN GpioDataRegs.GPBSET.bit.GPIO34 = 1;    // Abre el rele de IPM

// **** GRUPO C
#define CAC1_CLOSE GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1; // Cierra el contactor de CA
#define CAC1_OPEN GpioDataRegs.GPCSET.bit.GPIO87 = 1;    // Abre el contactor de CA

#define GET_CDC1_STATE (GpioDataRegs.GPCDAT.bit.GPIO86) // Estado del contactor de CC

extern void InitGpio(void);

#endif
