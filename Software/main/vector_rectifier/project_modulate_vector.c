/***********************************************************************
 * File:        project_vector_modulation.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se definen las funciones para la reproduccion del vec-
 *              tor rotante mediante modulacion vectorial. 
 *
 * History:
 *   10/01/2026 - original
 *   16/10/2026 - cambio de nombre de las funciones por coherencia
 **********************************************************************/

#include <math.h>
#include "project.h"

/***********************************************************************
 * project_modulate_vector()
 *
 * Entrada:     u       Valor de pico del vector a reproducir
 *              theta   Angulo del vector a reproducir
 *              Vbus    Tension del bus de CC
 *
 * Salida:      No posee
 *
 * Retorno:     Siempre cero
 *
 * Descripcion: Reproduce en la salida PWM la posicion de un vector ro-
 *              tante en base a su valor de pico, angulo y la tension
 *              de bus de CC disponible. Utiliza los ePWM 1, 2 y 3
 **********************************************************************/
int project_modulate_vector(float u, float theta, float Vbus)
{
    float E = Vbus;               // Tension del bus de continua
    float fase;                   // Variable auxiliar para calcular la fase del vector rotante
    float Ts = 1 / PROJECT_F_PWM; // Siempre que Fs sea 10 kHz
    float fase_rel;               // Fase relativa del vector rotante dentro del sextante
    int   sext;                   // Sextante en el cual se encuentra el vector rotante
    float ta, tb, t0;             // Tiempos utilizados en la modulación vectorial
    float t_pwm1, t_pwm2, t_pwm3; // Tiempos que deben permanecer en alto cada uno de los PWMs para la reproducción del vector

    // Ajustamos la fase a un numero comprendido entre 0 y 2pi
    fase = theta;
    fase = fmod(fase, 2 * M_PI);

    // Calculamos el sextante en el que se encuentra el vector junto con su fase relativa dentro del mismo
    fase_rel = fmod(fase, M_PI / 3);
    sext     = (int)ceil(fase / (M_PI / 3));

    // Calculamos los tiempos de la modulacion vectorial
    float imod = (sqrt(3) * u) / E;
    imod       = imod > 1 ? 1 : imod;

    ta = imod * Ts * sin(M_PI / 3 - fase_rel);
    tb = imod * Ts * sin(fase_rel);
    t0 = Ts - ta - tb;

    // Calculamos el tiempo en alto de cada PWM
    switch (sext) {
        case 1:
            t_pwm1 = ta + tb + t0 / 2;
            t_pwm2 = tb + t0 / 2;
            t_pwm3 = t0 / 2;
            break;

        case 2:
            t_pwm1 = ta + t0 / 2;
            t_pwm2 = ta + tb + t0 / 2;
            t_pwm3 = t0 / 2;
            break;

        case 3:
            t_pwm1 = t0 / 2;
            t_pwm2 = ta + tb + t0 / 2;
            t_pwm3 = tb + t0 / 2;
            break;

        case 4:
            t_pwm1 = t0 / 2;
            t_pwm2 = ta + t0 / 2;
            t_pwm3 = ta + tb + t0 / 2;
            break;

        case 5:
            t_pwm1 = tb + t0 / 2;
            t_pwm2 = t0 / 2;
            t_pwm3 = ta + tb + t0 / 2;
            break;

        case 6:
            t_pwm1 = ta + tb + t0 / 2;
            t_pwm2 = t0 / 2;
            t_pwm3 = ta + t0 / 2;
            break;

        default:
            break;
    }

    // Reproducimos los PWM con el DC correspondiente
    EPwm1Regs.CMPA.half.CMPA = PWM_COUNT_PERIOD * (1 - t_pwm1 / Ts);
    EPwm2Regs.CMPA.half.CMPA = PWM_COUNT_PERIOD * (1 - t_pwm2 / Ts);
    EPwm3Regs.CMPA.half.CMPA = PWM_COUNT_PERIOD * (1 - t_pwm3 / Ts);

    return 0;
}
