/**********************************************************************
 * File: project.h
 * Device: TMS320F2833x
 * Author: Technical Training Organization (TTO), Texas Instruments
 * Description: Include file for C28x workshop labs.  Include this
 *   file in all C-source files.
 * History:
 *   07/15/08 - original
 **********************************************************************/

#ifndef PROJECT_H
#define PROJECT_H

//---------------------------------------------------------------------------
// Librerias estandar de C
//
#include <string.h>

//---------------------------------------------------------------------------
// Librerias de texas
//
#include <DSP2833x_Device.h> // DSP2833x header file peripheral address definitions
#include <rectifier/board/gpio.h>           // Configuracion de los pines digitales

//---------------------------------------------------------------------------
// Librerias propias
//
#include "rectifier/rectifier.h"
#include "Interrupciones.h"                 // Definiciones de las rutinas de interrupcion (ISR)


//---------------------------------------------------------------------------
// Constantes operativas
//
#define F_PWM               10000.0 // Frecuencia de conmutacion del PWM en hertz
#define F_NOM               50.0    // Frecuencia de la señal a generar en hertz
#define ADC_BUF_LEN         48      // ADC buffer length
#define VIN_MAX             3.0     // Tension de entrada analogica medible maxima
#define VOUT_MAX            3.3     // Tension de salida digital maxima

//---------------------------------------------------------------------------
// Constantes matematicas
//
#define PI              3.1415926535    // Famoso numero irracional con multiples aplicaciones
#define TWO_PI          6.283185307     // El doble de un famoso numero irracional
#define TWO_THIRDS_PI   2.09439510233   // Dos tercios de un famoso numero irracional
#define FOUR_THIRDS_PI  4.18879020466   // Cuatro tercios de un famoso numero irracional
#define ONE_SIXTH_PI    0.523598775     // Un sexto de un famoso numero irracional
#define ISQRT2          1.4142135623    // La raiz cuadrada del primer numero natural par

typedef struct
{
    float f;     // Frecuencia calculada
    float theta; // Angulo de la fase a / vector rotante
    float va;    // Tension de sec principal en fase a
    float vb;    // Tension de sec principal en fase b
    float vc;    // Tension de sec principal en dase c
    float mod;   // Modulo del vector rotante de primer armonico (pico)
} PLL_VARS;

//---------------------------------------------------------------------------
// Function Prototypes
//
//extern void DelayUs(Uint16);
extern void InitPieCtrl(void);
extern void InitSysCtrl(void);
extern void InitWatchdog(void);

// ****************** Funciones creadas por nosotros
int         imprimir_vector(float u, float theta);
int         imprimir_vector_vbus(float u, float theta, float Vbus);
PLL_VARS    run_pll(float Vr, float Vs, float Vt);
void        run_interrupt();
void        init_interrupts_macros();

//---------------------------------------------------------------------------
// Global symbols defined in the linker command file
//
extern Uint16 secureRamFuncs_loadstart;
extern Uint16 secureRamFuncs_loadsize;
extern Uint16 secureRamFuncs_runstart;
extern Uint16 hwi_vec_loadstart;
extern Uint16 hwi_vec_loadsize;
extern Uint16 hwi_vec_runstart;
extern Uint16 trcdata_loadstart;
extern Uint16 trcdata_loadsize;
extern Uint16 trcdata_runstart;

//---------------------------------------------------------------------------
extern Uint16 AdcBuf[ADC_BUF_LEN]; // used in Labs 6 and 7 - ONLY
extern Uint16 AdcBufRaw[2 * ADC_BUF_LEN];
extern const struct PIE_VECT_TABLE PieVectTableInit; // PieVectTableInit is always extern

//---------------------------------------------------------------------------
// Macros
//

typedef enum
{
    CONVERTER_IDLE = 0,
    CONVERTER_PRECHARGING, // Estado en el que la precarga está activa
    CONVERTER_STARTING_KP, // Se habilita la senal a los IGBTs y el control tension proporcional
    CONVERTER_WORKING,
    CONVERTER_STOPPING_WITH_DISCHARGE,      // Se apaga al convertidor utilizando la carga para descargar el bus de CC
    CONVERTER_STOPPING_WITHOUT_DISCHARGE,   // Se apaga al convertidor desconectando la carga y dejando el bus cargado a la tension de rectificador a diodo (minima posible)
    CONVERTER_FINAL                         // Estado no alcanzable, utilizado con fines de control
} CONVERTER_STATE;

//---------------------------------------------------------------------------

#endif

//--- end of file -----------------------------------------------------
