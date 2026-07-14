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
// Include Standard C Language Header Files
//
#include <string.h>

//---------------------------------------------------------------------------
// Include any other Header Files
//
#include "DSP2833x_Device.h" // DSP2833x header file peripheral address definitions

// Librerias propias
#include "Interrupciones.h" // Definiciones de las rutinas de interrupción (ISR)
#include "PWM_config.h"     // Configuracion de los ePWM
#include "Gpio.h"           // Configuracion de los pines digitales
#include "tripzone.h"       // Manejo del modulo trip-zone
#include "protections.h"    // Protecciones por sw

//---------------------------------------------------------------------------
// Constant Definitions
//
// ***** Constantes operativas
#define F_PWM 10000.0          // Frecuencia de conmutación del PWM en hertz
#define F_NOM 50.0             // Frecuencia de la señal a generar en hertz
#define ADC_BUF_LEN 48         // ADC buffer length
#define ADC_SAMPLE_PERIOD 3124 // 3124 = (3125-1) = 48 KHz sampling w/ 150 MHz SYSCLKOUT
#define PWM_HALF_PERIOD 37500  // period/2 for 2 kHz symmetric PWM
#define PWM_DUTY_CYCLE 28125   // 25% duty cycle
#define FILTER_LEN 5           // filter length
#define PROJECT_PERIOD 7500.0  // Semi-periodo en cuentas necesario para generar una señal de 10 KHz con 150 MHz de clock base
#define VIN_MAX 3.0            // Tensi�n de entrada m�xima
#define VOUT_MAX 3.3           // Tensi�n de salida m�xima

// ***** Constantes matematicas
#define PI 3.1415926535              // Famoso numero irracional con multiples aplicaciones
#define TWO_PI 6.283185307           // El doble de un famoso numero irracional con multiples aplicaciones
#define TWO_THIRDS_PI 2.09439510233  // Dos tercios de un famoso numero irracional
#define FOUR_THIRDS_PI 4.18879020466 // Cuatro tercios de un famoso numero irracional
#define ONE_SIXTH_PI 0.523598775     // Un sexto de un famoso numero irracional
#define ISQRT2 1.4142135623          // La raiz cuadrada del primer numero natural par

#if MATH_TYPE == IQ_MATH
#define ADC_FS_VOLTAGE _IQ(3.0)          // ADC full scale voltage
#else                                    // MATH_TYPE is FLOAT_MATH
#define ADC_FS_VOLTAGE _IQ(3.0 / 4096.0) // ADC full scale voltage, scaled for 12 bit ADC
#endif

typedef struct
{
    float f;     // Frecuencia calculada
    float theta; // Angulo de la fase a / vector rotante
    float va;    // Tension de sec principal en fase a
    float vb;    // Tension de sec principal en fase b
    float vc;    // Tensión de sec principal en dase c
    float mod;   // Modulo del vector rotante de primer armonico (pico)
} PLL_VARS;

//---------------------------------------------------------------------------
// Function Prototypes
//
extern void DelayUs(Uint16);
extern void Delayms(Uint32 tiempo);
extern void InitAdc(int test_num);
extern void InitECap(void);
extern void InitPieCtrl(void);
extern void InitSysCtrl(void);
extern void InitWatchdog(void);
extern void SetDBGIER(Uint16);

// ****************** Funciones creadas por nosotros
int imprimir_vector(float u, float theta);
int imprimir_vector_vbus(float u, float theta, float Vbus);
PLL_VARS run_pll(float Vr, float Vs, float Vt);
void run_interrupt();
void init_interrupts_macros();

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
extern Uint32 PwmDuty;
extern Uint32 PwmPeriod;
extern Uint16 AdcBuf[ADC_BUF_LEN]; // used in Labs 6 and 7 - ONLY
extern Uint16 AdcBufRaw[2 * ADC_BUF_LEN];
extern Uint16 DEBUG_TOGGLE;
extern Uint16 DEBUG_FILTER;
extern const struct PIE_VECT_TABLE PieVectTableInit; // PieVectTableInit is always extern

//---------------------------------------------------------------------------
// Macros
//
#define ADC_cal_func_ptr (void (*)(void))0x380080

typedef enum
{
    CONVERTER_IDLE = 0,
    CONVERTER_PRECHARGING, // Estado en el que la precarga está activa
    CONVERTER_STARTING_KP, // Se habilita la senal a los IGBTs y el control tension proporcional
    CONVERTER_STARTING_KI,
    CONVERTER_RISING_VBUS,
    CONVERTER_WORKING,
    CONVERTER_EXIT,
    
} CONVERTER_STATE;

//---------------------------------------------------------------------------

#endif

//--- end of file -----------------------------------------------------
