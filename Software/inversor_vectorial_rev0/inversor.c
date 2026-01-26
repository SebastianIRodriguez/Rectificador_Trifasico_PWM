/**
 * @file inversor.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-01-24
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "project.h"
#include "inversor.h"

static int inicializado = 0;
static int activo = 0;

/**
 * @brief Inicializa el inversor sin arrancarlo
 *
 */
void InitInversor()
{
    if (!inicializado)
    {
        // Iniciamos el ADC para el muestreo de las tensiones y corrientes RST, además del bus de continua
        InitAdc(6);

        // Inciar los PWM 1, 2 y 3 en formato de modulacion vectorial y habilitamos la interrupcion del ePWM1
        InitEPwm123_withInterrupt();
        inicializado = 1;
    }
}

/**
 * @brief Arranca al inversor.
 * Por defecto no toma ni entrega potencia.
 *
 */
void StartInversor()
{
    activo = 1;
}

/**
 * @brief Arranca al inversor.
 * Por defecto no toma ni entrega potencia.
 *
 */
void StopInversor()
{
    //TODO: Poner a 0 los PWM 
    activo = 0;
    EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD; // DC = 40%
	EPwm2Regs.CMPA.half.CMPA = PROJECT_PERIOD; // DC = 50%
	EPwm3Regs.CMPA.half.CMPA = PROJECT_PERIOD; // DC = 60%
    
}

/**
 * @brief Indica si el inversor esta activo
 * 
 * @return int verdadero si esta activo, falso caso contrario
 */
int IsInversorOn()
{
    return activo;
}
