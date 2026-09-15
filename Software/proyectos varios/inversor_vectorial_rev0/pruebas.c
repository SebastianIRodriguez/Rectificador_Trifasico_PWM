/**
 * @file pruebas.c
 * @author Luciano Raffagnini (lraffagnini21@gmail.com)
 * @brief En este archivo se encuentran las implementaciones de varias funciones que prueban diversas utilidades
 * del sistema.
 * @version 0.1
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "pruebas.h"
#include "project.h"
#include "PWM_config.h"

/**
 * @brief Inicia los ePWM1, 2 y 3 con un DC fijo del 40, 50 y 60% en cada uno 
 * F: 10 kHz
 * El ADC no se inicia en esta prueba
 * No se generan pedidos de interrupcion en ningun PWM
 */
void pwm_1_2_3_static_test()
{
    InitEPwm123_static();
}

/**
 * @brief Inicia los ePWM1, 2 y 3 y generan una modulación vectorial
 * de una señal trifásica de 50 Hz. 
 * Para ello, toma al ePWM1 como fuente de interrupcion cuando este es cero
 */
void pwm_1_2_3_sim_vectorial_test()
{
    // Configura los ePWM1, 2 y 3 de forma sincronica y genera pedido de interrupcion 
    // cuando ePWM1 es cero
    InitEPwm123_withInterrupt();
}


void pwm_1_2_3_pll_test()
{
    // Iniciar el ADC
    InitAdc(6);

    // Inciar el PWM
    InitEPwm123_withInterrupt();
    //InitEPwm123_withInterrrupt_pwm4_debug();
}
