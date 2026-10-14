/**********************************************************************
 * File:        calibration.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Este archivo contiene las constantes de calibracion del
 *              ADC para la lectura de las variables analogicas del
 *              rectificador trifasico
 * 
 * History:
 *   24/01/2026 - original
 *   19/09/2026 - emprolijamiento del archivo
 **********************************************************************/

#ifndef CALIBRATION_H
#define CALIBRATION_H

// Tensiones
#define OFFSET_UV   2059
#define OFFSET_VV   2068
#define OFFSET_WV   2062
#define OFFSET_B1V  15
#define OFFSET_YV   19 // Usado en modo AC es 2060
#define OFFSET_ZV   2060
#define OFFSET_B2V  19
#define OFFSET_RV   2051
#define OFFSET_TV   2053
#define OFFSET_SV   2058

#define KRV (1 / 25.48836)
#define KSV (1 / 25.62630)
#define KTV (1 / 25.70917)
#define KUV (1 / 15.30328)
#define KVV (1 / 15.08250)
#define KWV (1 / 15.07191)
#define KB1 (1 / 25.47792)
#define KB2 (1 / 15.20952)
#define KYV (1 / 25.04)

// Corrientes
#define OFFSET_RI 2055
#define OFFSET_SI 2055
#define OFFSET_TI 2065
#define OFFSET_UI 2065
#define OFFSET_VI 2054
#define OFFSET_WI 2065

#define KRI (1/301.49)
#define KSI (1/300.27)
#define KTI (1/299.95)
#define KUI (1/301.93)
#define KVI (1/301.11)
#define KWI (1/299.93)

#endif
