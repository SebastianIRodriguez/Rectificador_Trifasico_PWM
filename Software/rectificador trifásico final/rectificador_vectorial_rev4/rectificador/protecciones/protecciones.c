/**********************************************************************
 * File:        tripzone.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: se declaran las variables necesarias para las
 *              protecciones
 *
 * History:
 *   13/05/2026 - original
 *   14/09/2026 - emprolijamiento del archivo
 **********************************************************************/

#include "protecciones.h"

Protections_flags   PROTECTION_FLAGS;
Fail_counter        fail_counter = {0, 0, 0, 0, 0, 0, 0};
