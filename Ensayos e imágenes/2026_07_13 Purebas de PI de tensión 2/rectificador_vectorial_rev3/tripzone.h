#ifndef TRIPZONE_H
#define TRIPZONE_H

/**
 * Fuerza la activacion del modulo trip-zone en los ePWM 1, 2 y 3
 */
#define FORCE_TRIPZONE_INTERRUPT                                               \
    asm(" EALLOW");              /* Enable EALLOW protected register access */ \
    EPwm1Regs.TZFRC.bit.OST = 1; /* Fuerza el evento de trip - zone */         \
    EPwm2Regs.TZFRC.bit.OST = 1; /* Fuerza el evento de trip - zone */         \
    EPwm3Regs.TZFRC.bit.OST = 1; /* Fuerza el evento de trip - zone */         \
    asm(" EDIS");                /* Disable EALLOW protected register access */

#endif
