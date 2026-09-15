/**
 * @file utils.c
 * @author Luciano Raffagnini (lraffagnini21@gmail.com)
 * @brief En este archivo se encuentran las definiciones de varias funciones utiles del proyecto
 * @version 0.1
 * @date 2026-01-24
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "project.h"
#include "DSP2833x_Device.h"

void Delayms(Uint32 tiempo)
{
    Uint32 i = 0;
    for (i = 0; i < tiempo; i++)
        DelayUs(1000);
}
