/**********************************************************************
 * File: project.h
 * Device: TMS320F2833x
 * Author: Technical Training Organization (TTO), Texas Instruments
 * Description: Include file for C28x workshop labs.  Include this
 *   file in all C-source files.
 * History:
 *   07/15/08 - original
 **********************************************************************/
//----------------------------------------------------------------------

#ifndef PROJECT_H
#define PROJECT_H

//----------------------------------------------------------------------
// Librerias estandar de C
//
#include <string.h>

//----------------------------------------------------------------------
// Librerias de texas
//
#include "texas/texas.h"
#include <DSP2833x_Device.h> // DSP2833x header file peripheral address definitions

//----------------------------------------------------------------------
// Librerias propias
//
#include "rectifier/rectifier.h"

//----------------------------------------------------------------------
// Constantes operativas
//
#define PROJECT_F_PWM       10000.0 // Frecuencia de conmutacion del PWM en hertz
#define PROJECT_F_INTERRUPT 10000.0 // Frecuencia a la cual se ejecuta la interrupcion principal (coincide con la de PWM)
#define PROJECT_F_NOM       50.0    // Frecuencia nominal de la red electrica
#define ADC_BUF_LEN         48      // ADC buffer length
#define VIN_MAX             3.0     // Tension de entrada analogica medible maxima
#define VOUT_MAX            3.3     // Tension de salida digital maxima

//----------------------------------------------------------------------
// Constantes matematicas
//
#define PI             3.1415926535  // Famoso numero irracional con multiples aplicaciones
#define TWO_PI         6.283185307   // El doble de un famoso numero irracional
#define TWO_THIRDS_PI  2.09439510233 // Dos tercios de un famoso numero irracional
#define FOUR_THIRDS_PI 4.18879020466 // Cuatro tercios de un famoso numero irracional
#define ONE_SIXTH_PI   0.523598775   // Un sexto de un famoso numero irracional
#define ISQRT2         1.4142135623  // La raiz cuadrada del primer numero natural par

#define MAX_ERROR_COUNT 10

/***********************************************************************
 * PLL_VARS
 *
 * Descripcion: Estructura que almacena todas las magnitudes relevantes
 *              obtenidas del PLL.
 **********************************************************************/
typedef struct
{
    float f;     // Frecuencia calculada
    float theta; // Angulo de la fase a / vector rotante
    float va;    // Tension de sec principal en fase a
    float vb;    // Tension de sec principal en fase b
    float vc;    // Tension de sec principal en dase c
    float mod;   // Modulo del vector rotante de primer armonico (pico)
} PLL_VARS;

/***********************************************************************
 * SIGNAL_3Magn
 *
 * Descripcion: Estructura que permite almacenar 3 magnitudes
 *              relacionadas entre si.
 *              Podria ser util para determinados sistemas de
 *              distribucion electrica que usan 3 fases, no?
 *              Debe ser casualidad...
 *              Difiere de SIGNAL_3Ph porque no contiene datos de
 *              angulo ni K
 **********************************************************************/
typedef struct {
    float a;
    float b;
    float c;
} SIGNAL_3Magn;

/***********************************************************************
 * ROTATING_VECTOR
 *
 * Descripcion: Permite representar un vector rotante por su magnitud
 *              y modulo (pico)
 **********************************************************************/
typedef struct {
    float mod;
    float theta;
} ROTATING_VECTOR;

/***********************************************************************
 * CONVERTER_STATE
 *
 * Descripcion: Estado del convertidor
 **********************************************************************/
typedef enum {
    CONVERTER_IDLE = 0,
    CONVERTER_PRECHARGING_PASSIVE,        // Precarga utilizando unicamente los diodos en antiparalelo de los IGBT
    CONVERTER_PRECHARGING_ACTIVE,         // Se lleva la salida al sp en vacio
    CONVERTER_WORKING,                    // Funcionamiento normal del convertidor
    CONVERTER_STOPPING_WITH_DISCHARGE,    // Se apaga al convertidor utilizando la carga para descargar el bus de CC
    CONVERTER_STOPPING_WITHOUT_DISCHARGE, // Se apaga al convertidor desconectando la carga y dejando el bus cargado a la tension de rectificador a diodo (minima posible)
    CONVERTER_FINAL                       // Estado no alcanzable, utilizado con fines de control
} CONVERTER_STATE;

/***********************************************************************
 * GLOBAL_ERROR
 *
 * Descripcion: Codigos de errores 
 **********************************************************************/
typedef enum {
    NO_ERROR = 0,
    ERROR_PRECHARGE_TIMEOUT, // La precarga llevo demasiado tiempo
    ERROR_QAC_STATE,         // La termica de AC se encuentra en un estado erroneo
    ERROR_QDC_STATE          // La termica de DC se encuentra en un estado erroneo
} GLOBAL_ERROR;

//----------------------------------------------------------------------
// Funciones prototipo
//
PLL_VARS project_sim_input_get();
int      project_pll_init();
PLL_VARS project_pll_run(float Vr, float Vs, float Vt);
int      project_sim_input_init();
void     project_control_init();
void     project_control_run(SIGNAL_3Magn v, SIGNAL_3Magn i, PLL_VARS red_trifasica, float vbus, float vsp, ROTATING_VECTOR *out);
int      project_interrupt_init();
int      project_modulate_vector(float u, float theta, float Vbus);

//----------------------------------------------------------------------
// Gestion de los errores globales
//
extern volatile int          global_error_count;
extern volatile GLOBAL_ERROR global_error_array[];

#define PROJECT_ADD_ERROR(err)                \
    if (global_error_count < MAX_ERROR_COUNT) \
        global_error_array[global_error_count++] = err;

#endif

//--- end of file -----------------------------------------------------
