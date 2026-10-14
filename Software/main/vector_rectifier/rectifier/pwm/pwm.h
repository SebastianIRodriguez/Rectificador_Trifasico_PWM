/***********************************************************************
 * File:        pwm.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se declaran las funciones y macros para el manejo
 *              del PWM
 *
 * History:
 *   19/09/2026 - original
 **********************************************************************/

#ifndef PWM_H
#define PWM_H

#define PWM_COUNT_PERIOD 7500.0 // Semi-periodo en cuentas necesario para generar una senal de 10 KHz con 150 MHz de clock base

void pwm_init123_withInterrupt();
void pwm_init123_withInterrupt_pwm4_debug();

#endif
