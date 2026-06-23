/**
 *
 */

#ifndef SW_PROTECTIONS_H
#define SW_PROTECTIONS_H

#include "tripzone.h"

#define MIN_FASE_V -50.0 // Tension minima (o maxima negativa) admisible en cualquier fase
#define MAX_FASE_V 50.0  // Tension maxima admisible en cualqueir fase
#define MAX_BUS_V 100     // Tension maxima admisible en el bus de CC
#define MIN_FASE_I -3    // Corriente minima (o maxima negativa) admisible en cualquier fase
#define MAX_FASE_I 3     // Corriente maxima admisible en cualqueir fase

#define MAX_FAILS 10

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

typedef struct
{
    int vr_fail;
    int vs_fail;
    int vt_fail;
    int vb_fail;
    int ir_fail;
    int is_fail;
    int it_fail;
} Fail_counter;

extern Protections_flags PROTECTION_FLAGS;
extern Fail_counter fail_counter;

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
        fail_counter.vr_fail++;                      \
        if (fail_counter.vr_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.vr_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.vr_fail = 0;                    \
    }                                                \
    if (vs < MIN_FASE_V || MAX_FASE_V < vs)          \
    {                                                \
        fail_counter.vs_fail++;                      \
        if (fail_counter.vs_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.vs_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.vs_fail = 0;                    \
    }                                                \
    if (vt < MIN_FASE_V || MAX_FASE_V < vt)          \
    {                                                \
        fail_counter.vt_fail++;                      \
        if (fail_counter.vt_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.vt_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.vt_fail = 0;                    \
    }                                                \
    if (MAX_BUS_V < vb)                              \
    {                                                \
        fail_counter.vb_fail++;                      \
        if (fail_counter.vb_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.vb_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.vb_fail = 0;                    \
    }                                                \
    if (ir < MIN_FASE_I || MAX_FASE_I < ir)          \
    {                                                \
        fail_counter.ir_fail++;                      \
        if (fail_counter.ir_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.ir_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.ir_fail = 0;                    \
    }                                                \
    if (is < MIN_FASE_I || MAX_FASE_I < is)          \
    {                                                \
        fail_counter.is_fail++;                      \
        if (fail_counter.is_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.is_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.is_fail = 0;                    \
    }                                                \
    if (it < MIN_FASE_I || MAX_FASE_I < it)          \
    {                                                \
        fail_counter.it_fail++;                      \
        if (fail_counter.it_fail > MAX_FAILS)        \
        {                                            \
            PROTECTION_FLAGS.flags.it_flag = 1;      \
            FORCE_TRIPZONE_INTERRUPT;                \
        }                                            \
    }                                                \
    else                                             \
    {                                                \
        fail_counter.it_fail = 0;                    \
    }

#endif
