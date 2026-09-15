/**
 * @file project_interrupts.c
 * @author your name (you@domain.com)
 * @brief Aqui se encuentran definidas las interrupciones utilizadas en el proyecto para tener un facil acceso
 * @version 0.1
 * @date 2025-10-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Lab.h"

/*
interrupt void EPWM1_INT_ISR(void) // PIE3.1 @ 0x000D60  EPWM1_INT (EPWM1)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; // Must acknowledge the PIE group
    EPwm1Regs.ETCLR.bit.INT = 1;            // limpiar bandera

    // Toggle al pin de control
    GpioDataRegs.GPATOGGLE.bit.GPIO11 = 1;
    //interrupcion();
}*/
