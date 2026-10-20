/**********************************************************************
 * File:        board.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: En este archivo se incluyen las bibliotecas realacioadas
 *              al manejo de la placa del proyecto, esto incluye:
 *                  ADC
 *                  PWM
 *                  GPIO
 *              Tambien se definen macros para el control de las
 *              entradas y salidas digitales
 *
 * History:
 *   13/05/2026 - original
 **********************************************************************/
#ifndef BOARD_H
#define BOARD_H


// ----------------------------- GRUPO A ---------------------------- //

#define BOARD_PRECHARGE_CLOSE             GpioDataRegs.GPACLEAR.bit.GPIO10    = 1;    // Cierra el rele de IPM
#define BOARD_PRECHARGE_OPEN              GpioDataRegs.GPASET.bit.GPIO10      = 1;    // Abre el rele de IPM

#define BOARD_GET_CAC1_STATE              (GpioDataRegs.GPADAT.bit.GPIO22)            // Estado del contactor de AC

#define BOARD_GET_QAC1_STATE              (GpioDataRegs.GPADAT.bit.GPIO16)            // Estado de la termica de AC
#define BOARD_GET_QDC1_STATE              (GpioDataRegs.GPADAT.bit.GPIO18)            // Estado de la termica de CC

#define BOARD_GPIO08_SET                  GpioDataRegs.GPASET.bit.GPIO8       = 1;
#define BOARD_GPIO08_CLEAR                GpioDataRegs.GPACLEAR.bit.GPIO8     = 1;

#define BOARD_GPIO09_SET                  GpioDataRegs.GPASET.bit.GPIO9       = 1;
#define BOARD_GPIO09_CLEAR                GpioDataRegs.GPACLEAR.bit.GPIO9     = 1;

#define BOARD_GET_PRECHARGE_RELAY_STATE   (GpioDataRegs.GPADAT.bit.GPIO15)            // Estado de los reles de precarga


// ----------------------------- GRUPO B ---------------------------- //

#define BOARD_CDC1_CLOSE                  GpioDataRegs.GPBCLEAR.bit.GPIO32    = 1;    // Cierra el contactor de CC
#define BOARD_CDC1_OPEN                   GpioDataRegs.GPBSET.bit.GPIO32      = 1;    // Abre el contactor de CC

#define BOARD_IPM_CLOSE                   GpioDataRegs.GPBCLEAR.bit.GPIO34    = 1;    // Cierra el rele de IPM
#define BOARD_IPM_OPEN                    GpioDataRegs.GPBSET.bit.GPIO34      = 1;    // Abre el rele de IPM


// ----------------------------- GRUPO C ---------------------------- //

#define BOARD_CAC1_CLOSE                  GpioDataRegs.GPCCLEAR.bit.GPIO87    = 1;    // Cierra el contactor de CA
#define BOARD_CAC1_OPEN                   GpioDataRegs.GPCSET.bit.GPIO87      = 1;    // Abre el contactor de CA

#define BOARD_GET_CDC1_STATE              (GpioDataRegs.GPCDAT.bit.GPIO86)            // Estado del contactor de CC

#endif
