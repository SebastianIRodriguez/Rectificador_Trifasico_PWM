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
#include "Lab.h"

/**
 * @brief Esta funcion configura el ADC para tener de SOC al ePWM2 cuando TBCTR = TBPRD.
 * Es decir, lanza una conversion cuando el ePWM2 llega a la mitad de su periodo
 *
 */
void adc_interrupt_test()
{
    InitAdc(1);
    InitEPwm(1);
}

/**
 * @brief Esta funcion configura el ADC para tener de SOC al ePWM4 cuando TBCTR = TBPRD.
 * Es decir, lanza una conversion cuando el ePWM4 llega a la mitad de su período. A su vez,
 * el ePWM4 está generando una señal de salida.
 *
 */
void adc_interrupt_test_2()
{
    InitAdc(2);
    InitEPwm(2);
}

/**
 * @brief Esta funcion configura el ADC para tener de SOC al ePWM4 cuando TBCTR = CMPB.
 * Es decir, lanza una conversion cuando el ePWM4 llega a un valor de cuenta establecido por SW
 *
 */
void adc_interrupt_test_3()
{
    InitAdc(2);
    InitEPwm(3);
}

/**
 * @brief Esta funcion configura el ADC para tener de SOC al ePWM4 cuando TBCTR = CMPB.
 * Además, se encuentra en modo de muestreo simultáneo, así que debería tomar la lectura del A0 y B0.
 *
 */
void adc_interrupt_test_4()
{
    InitAdc(4);
    InitEPwm(3);
}

/**
 * @brief Esta funcion configura el ADC para tener de SOC al ePWM4 cuando TBCTR = CMPB.
 * Además, se encuentra en modo de muestreo simultáneo con los canales 0, 1, 2 y 3
 *
 */
void adc_interrupt_test_5()
{
    InitAdc(5);
    InitEPwm(3);
}

/**
 * @brief Esta funcion configura el ADC para tener de SOC al ePWM4 cuando TBCTR = CMPB.
 * Además, se encuentra en modo de muestreo simultáneo con los canales 0, 1, 2 y 3.
 * Además, implementa las transformadas de Clarke y Park a las señales medidas
 *
 */
void adc_interrupt_test_6()
{
    InitAdc(5);
    InitEPwm(3);
}

/**
 * @brief Esta función configura al ADC para tener de SOC al ePWM4 cuando TBCTR = CMPB.
 * Además, se encuentra en modo de muestreo simultáneo con los canales 0, 1, 2 y 3.
 * Además, configura al EPWM4 como fuente de interrupción
 */
void pwm4_interrupt_test()
{
    InitAdc(6); // OJO QUE FALTA
    InitEPwm(4);
}

/**
 * @brief Permite la prueba del PLL
 *
 */
void pll_test()
{
    InitAdc(6);
    InitEPwm(5);
}

/**
 * @brief Inicializa todos los canales de conversion analogica en modo simultáneo
 * TODO: CAMBIAR EL INITADC(7)
 */
void adc_test()
{
    InitAdc(6);
    InitEPwm(2);
}

void pwm_test()
{
    InitAdc(2);
    InitEPwm(6);
}

/**
 * @brief Inicia los ePWM1, 2 y 3 con un DC fijo del 40, 50 y 60% en cada uno 
 * F: 10 kHz
 * El ADC no se inicia en esta prueba
 * 
 */
void pwm_1_2_3_static_test()
{
    InitEPwm(6);
}

/**
 * @brief Inicia los ePWM1, 2 y 3 y generan una modulación vectorial
 * de una señal trifásica de 50 Hz. 
 * 
 */
void pwm_1_2_3_sim_vectorial_test()
{

}

/**
 * @brief Habilita la inteerrupcion en el EPWM1
 * 
 */
void pwm1_interrupt_test()
{
    InitEPwm(7);
}
