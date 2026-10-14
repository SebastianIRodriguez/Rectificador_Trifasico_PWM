/***********************************************************************
 * File:        project_errors.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se declaran las variables globales utilizadas para el
 *              seguimiento de los errores del sistema
 *
 * History:
 *   14/10/2026 - original
 **********************************************************************/

#include "project.h"

volatile int          global_error_count = 0;
volatile GLOBAL_ERROR global_error_array[MAX_ERROR_COUNT];

