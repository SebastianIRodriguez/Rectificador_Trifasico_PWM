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

#include "project.h"

/**
 * @brief Esta funcion se llama dentro de la interrupcion por EPWM1
 *
 */
void run_interrupt()
{
    // ***** ADQUISICION DE VALORES ANALOGICOS
    // (MMMM ACA SE ADQUIERE O SE SIMULAN LAS VARIABLES ANALOGICAS)

    // ***** Ejecucion del PLL
    PLL_VARS sec_positiva;
    sec_positiva = run_pll();

    // ***** Transformadas
    // Aca tengo que usar las macros de clark y park con las tensiones de sec positiva y las corrientes

    // ***** Control
    // Aca ejecuto la secuencia de control para obtener los valores de salida
    // Por consejo de pablo, el control debe arrancar como P y luego de 0.5 segundos como P+I

    // ***** Salida
    // Aca se reproduce la salida en el modulador vectorial

    // TODO: que run_pll retorne una estructura que tenga las 3 tensiones de sec positiva, ademas de la fase, frecuencia y magnitud del vector rotante.
}
