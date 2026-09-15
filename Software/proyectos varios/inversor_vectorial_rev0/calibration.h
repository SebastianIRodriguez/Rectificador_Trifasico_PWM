/**
 * @file calibration.h
 * @author Luciano Raffagnii (lraffagnini21@gmail.com)
 * @brief Este archivo contiene las constantes de calibración del ADC
 * @version 0.1
 * @date 2026-01-24
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

#define OFFSET_WI 0
#define OFFSET_UV 2059
#define OFFSET_VI 0
#define OFFSET_VV 2068
#define OFFSET_UI 0
#define OFFSET_WV 2062
#define OFFSET_TI 0
#define OFFSET_B1V 15
#define OFFSET_SI 0
#define OFFSET_YV 2060
#define OFFSET_RI 0
#define OFFSET_ZV 2060
#define OFFSET_B2V 19
#define OFFSET_RV 2051
#define OFFSET_TV 2053
#define OFFSET_SV 2058

#define KRV (1 / 25.48836)
#define KSV (1 / 25.62630)
#define KTV (1 / 25.70917)
#define KUV (1 / 15.30328)
#define KVV (1 / 15.08250)
#define KWV (1 / 15.07191)
#define KB1 (1 / 25.47792)
#define KB2 (1 / 15.20952)


#endif
