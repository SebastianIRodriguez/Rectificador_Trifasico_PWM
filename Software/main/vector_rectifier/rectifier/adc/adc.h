/**********************************************************************
 * File:        adc.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: en este archivo se encuentra declarada la api para
 *              el manejo del conversor analogico-digital (ADC)
 *
 * History:
 *   13/05/2026 - original
 *   14/09/2026 - emprolijamiento del archivo
 **********************************************************************/

#ifndef ADC_H
#define ADC_H

#define ADC_cal_func_ptr (void (*)(void))0x380080

typedef enum {
    ADC_CH0_SEC_INT_CODE = 0,   // Canal 0, muestreo secuencial con SOC desde ePWM, interrupcion habilitada
    ADC_CH0_SIM_INT_CODE,       // Canal 0, muestreo simultaneo con SOC desde ePWM, interrupcion habilitada
    ADC_ALL_SIM_INT_CODE,       // Todos los canales, muestreo simultaneo con SOC desde ePWM, interrupcion habilitada
    ADC_ALL_SIM_CODE,           // Todos los canales, muestreo simultaneo con SOC desde ePWM, interrupcion deshabilitada
    ADC_LAST_CODE               // Utilizado con fines de control, no es un codigo valido de uso
} ADC_MODE;

int adc_init(int config_code);

#endif
