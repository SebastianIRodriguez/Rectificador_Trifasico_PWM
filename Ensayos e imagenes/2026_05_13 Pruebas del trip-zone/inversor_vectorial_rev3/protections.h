/**
 *
 */

#ifndef SW_PROTECTIONS_H
#define SW_PROTECTIONS_H

#include "tripzone.h"

#define MIN_FASE_V -20.0 // Tension minima (o maxima negativa) admisible en cualquier fase
#define MAX_FASE_V 20.0  // Tension maxima admisible en cualqueir fase
#define MAX_BUS_V 20   // Tension maxima admisible en el bus de CC
#define MIN_FASE_I -3     // Corriente minima (o maxima negativa) admisible en cualquier fase
#define MAX_FASE_I 3      // Corriente maxima admisible en cualqueir fase

/**********************************************************************************
 * Union que permite divisar con facilidad la fuente del trip-zone
 *
 *
 **********************************************************************************/
typedef union
{
    char registro;
    struct
    {
        char vr_flag : 1;
        char vs_flag : 1;
        char vt_flag : 1;
        char vb_flag : 1;
        char ir_flag : 1;
        char is_flag : 1;
        char it_flag : 1;
        char fill : 1;
    } flags;
} Protections_flags;

extern Protections_flags PROTECTION_FLAGS;

#define PROTECTION_INIT \
    PROTECTION_FLAGS.registro = 0;

// TODO: implementar una rutina que verifique el estado correcto de los contactores
// y todo lo medible

/**********************************************************************************
 * Esta macro monitorea si alguno de los valores analogicos excede los limites    *
 * operativos.  Si lo hacen guarda la informacion de cual y lanza la interrupcion *
 * de trip-zone                                                                   *
 */
#define PROTECTION_MACRO(vr, vs, vt, vb, ir, is, it) \
    if (vr < MIN_FASE_V || MAX_FASE_V < vr)          \
    {                                                \
        PROTECTION_FLAGS.flags.vr_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }                                                \
    if (vs < MIN_FASE_V || MAX_FASE_V < vs)          \
    {                                                \
        PROTECTION_FLAGS.flags.vs_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }                                                \
    if (vt < MIN_FASE_V || MAX_FASE_V < vt)          \
    {                                                \
        PROTECTION_FLAGS.flags.vt_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }                                                \
    if (MAX_BUS_V < vb)                             \
    {                                                \
        PROTECTION_FLAGS.flags.vb_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }                                                \
    if (ir < MIN_FASE_I || MAX_FASE_I < ir)          \
    {                                                \
        PROTECTION_FLAGS.flags.ir_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }                                                \
    if (is < MIN_FASE_I || MAX_FASE_I < is)          \
    {                                                \
        PROTECTION_FLAGS.flags.is_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }                                                \
    if (it < MIN_FASE_I || MAX_FASE_I < it)          \
    {                                                \
        PROTECTION_FLAGS.flags.it_flag = 1;          \
        FORCE_TRIPZONE_INTERRUPT;                    \
    }

#endif
