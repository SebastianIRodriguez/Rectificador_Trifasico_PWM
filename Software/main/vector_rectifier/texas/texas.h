/**********************************************************************
 * File:        texas.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: En este archivo se definen las funciones extraidaas de
 *              ejemplos de texas instruments.
 *
 * History:
 *   13/10/2026 - original
 **********************************************************************/

#ifndef TEXAS_TEXAS_H_
#define TEXAS_TEXAS_H_

#include "Interrupciones.h"

//extern void DelayUs(Uint16);
void InitPieCtrl(void);
void InitSysCtrl(void);
void InitWatchdog(void);

#endif /* TEXAS_TEXAS_H_ */
