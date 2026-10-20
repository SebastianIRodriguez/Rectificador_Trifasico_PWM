/***********************************************************************
 * File:        gpio.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se declaran las funciones para el manejo de los pines
 *              digitales y el pwm.
 *
 * History:
 *   10/01/2026 - original
 *   19/09/2026 - emprolijamiento del archivo
 **********************************************************************/

#ifndef GPIO_H
#define GPIO_H

void gpio_init();
void gpio_disable_epwm();
void gpio_enable_epwm();

#endif
