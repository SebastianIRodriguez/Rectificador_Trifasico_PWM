/***********************************************************************
 * File:        control_scheme.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se declaran los elementos del esquema de control del
 *              rectificador
 *
 * History:
 *   06/10/2026 - original
 **********************************************************************/

#ifndef CONTROL_SCHEME_H
#define CONTROL_SCHEME_H

#include <Renolib.h>
#include "project.h"

#define I_FF_ENABLED 0

/***********************************************************************
 * SIGNAL_3Magn
 *
 * Descripcion: Estructura que permite almacenar 3 magnitudes
 *              relacionadas entre si.
 *              Podria ser util para determinados sistemas de
 *              distribucion electrica que usan 3 fases, ¿no?
 *              Debe ser casualidad...
 *              Difiere de SIGNAL_3Ph porque no contiene datos de
 *              angulo ni K
 **********************************************************************/
typedef struct{
    float a;
    float b;
    float c;
} SIGNAL_3Magn;

/***********************************************************************
 * ROTATING_VECTOR
 *
 * Descripcion: Permite representar un vector rotante por su magnitud
 *              y modulo (pico)
 **********************************************************************/
typedef struct{
    float mod;
    float theta;
} ROTATING_VECTOR;

void control_init();
void control_run(SIGNAL_3Magn v, SIGNAL_3Magn i, PLL_VARS red_trifasica, float vbus, float vsp, ROTATING_VECTOR *out);

#endif /* CONTROL_SCHEME_H */
