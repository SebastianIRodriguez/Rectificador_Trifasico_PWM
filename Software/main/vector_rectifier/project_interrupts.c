/**
 * @file project_interrupts.c
 * @author your name (you@domain.com)
 * @brief Aqui se encuentran definidas las interrupciones utilizadas en el proyecto para tener un facil acceso
 * @version 0.1
 * @date 2025-10-30
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Renolib.h"
#include "calibration.h"
#include "control_scheme.h"
#include "project.h"
#include "project_interrupt.h"
#include "rectifier/board/board.h"
#include <math.h>

static int interrupt_initiated = 0;

static void run_interrupt();
static int run_converter_mef(SIGNAL_3Magn v, SIGNAL_3Magn i, PLL_VARS grid, float vbus);

// ********** Habilitadores ********** //
#define PI_CONTROL_ENABLED 1 // Habilita la ejecucion del control PI
#define OUTPUT_ENABLED     1 // Habilita la salida por modulacion vectorial
#define SIM_INPUT_ENABLED  0 // Habilita la asquisicion simulada de datos

#define SHOW_CURRENT_EVOLUTION 1
#define SHOW_PHASE_ERROR       0
#define SHOW_FREQ_ERROR        0

#define VBUS_LOWER 5.0 // Valor umbral que considera que el bus de CC esta descargado

// TODO: Agregar seguridad, tiempo de enganche PLL, trip-zone
// TODO: Hacer un init de todo
// TODO: contar el tiempo de la interrupci�n con el timer del PWM
// TODO: Ver los valores de w de L
// TODO: Asignar valores de Ki y Kp, tal vez iniciar Ki despues

volatile float idsp = -0.5; // Corriente en directa de set point
volatile float iqsp = 0;    // Corriente en cuadratura de set point
//static float w        = 1;     // Frecuencia angular de la red
//static const float L  = 0.001; // La inductacia del tablero

//static int control_enabled = 1;     // Indica si la estructura de control esta habilitada para funcionar

//extern encender_sistema;

extern int activo;

float Ri, Si, Ti;     // Corrientes, solo usamos RST
float Rv, Sv, Tv, Yv; // Tensiones, solo usamos RST

float sRv, sSv, sTv; // Simuladas

float modmod = 18;

// ***** Habilitacion de terminos de control
float PI_ENABLED = 1;
//float I_FF_ENABLED = 0;

float Vbus_sp = 100;     // Set point de tension (antes 60)
float tau_v   = 0.175;   // Tiempo de respuesta (65%) del control tension en segundos
float r_load  = 51.6;    // Resistencia de carga
float c_bus   = 0.00188; // Capacidad del bus en faradios

// ***** Simulacion de trifasica
extern SIGNAL_3Ph SGN_3Ph_V;        // Estructuras que contienen 3 variables, una amplitud y un ángulo
extern VCO        FIFTH_VCO;        // Guarda el angulo de la señal simulada de quinto armonico
extern VCO        SEVENTH_VCO;      // Guarda el angulo de la señal simulada de septimo armonico
extern VCO        ELEVENTH_VCO;     // Guarda el angulo de la señal simulada de onceavo armonico
extern SIGNAL_3Ph SGN_NSC_V;        // Contiene la señal de armonico principal, secuencia negativa simulada
extern SIGNAL_3Ph SGN_5th_V;        // Contiene la señal de quinto armonico simulada
extern SIGNAL_3Ph SGN_7th_V;        // Contiene la señal de septimo armonico simulada
extern SIGNAL_3Ph SGN_11th_V;       // Contiene la señal de onceavo armonico simulada
extern float      k_nsc  = 0.0;     // Porcentaje de la amplitud principal que representa la secuencia negativa
extern float      k_5th  = 0.0;     // Porcentaje de la amplitud principal que representa el quinto armonico
extern float      k_7th  = 0.0;     // Porcentaje de la amplitud principal que representa el septimo armonico
extern float      k_11th = 0.0;     // Porcentaje de la amplitud principal que representa el onceavo armonico
extern VCO        AUX_VCO;          // Guarda el ángulo de la senal simulada
extern float      time_step;        // Paso de tiempo entre cada interrupcion
PLL_VARS          tension_simulada; // Contiene la tension simulada, con todos sus armonicos
float             amplitud_v = 1;   // Amplitud de la tension simulada
ANG_DIFF          theta_error;      // Variable utilizada para representar el error de fase

// *** Para los escalones de la senal simulada
volatile float amp_aux    = 0;   // Variable auxiliar para dar un escalon en tension
volatile float dw_aux     = 0.0; // Variable auxiliar para dar un escalon de frecuencia (medio HZ = 0,0795 rad/s)
volatile float dtheta_aux = 0.0; // Variable auxiliar para dar un escalon de fase en tensión (escalon de 30 grados = 0.5235)

// ********** Parametros de salida
float theta_out  = 0;
float modulo_out = 0;

#define MAX_TRIP_EVENTS 10
char trip_events = 0;

/**
 * @brief Inicia los sistemas necesarios para la simulacion de las señales de entrada
 *
 */
void sim_init()
{
    // Senales simuladas
    SIGNAL_3Ph_init(&SGN_3Ph_V, amplitud_v);
    SIGNAL_3Ph_init(&SGN_NSC_V, amplitud_v * k_nsc * -1); // Le damos secuencia negativa
    SIGNAL_3Ph_init(&SGN_5th_V, amplitud_v * k_5th * -1); // Le damos secuencia negativa
    SIGNAL_3Ph_init(&SGN_7th_V, amplitud_v * k_7th);
    SIGNAL_3Ph_init(&SGN_11th_V, amplitud_v * k_11th);

    // Angulos
    VCO_init(&AUX_VCO, TWO_PI * F_NOM, time_step);
    VCO_init(&FIFTH_VCO, TWO_PI * F_NOM * 5, time_step);
    VCO_init(&SEVENTH_VCO, TWO_PI * F_NOM * 7, time_step);
    VCO_init(&ELEVENTH_VCO, TWO_PI * F_NOM * 11, time_step);

    // Estructura para mostrar la fase
    ANG_DIFF_init(&theta_error);
}

/**
 * @brief Esta funcion devuelve una trifasica simulada
 * Tambien permite generar escalones o rampas (segun el caso)
 * en amplitud, fase y frecuencia
 */
PLL_VARS sim_get()
{
    PLL_VARS salida;

    static float previus_dw    = 0;
    static float rampita       = 0;
    static float escalon_rampa = 0.00125;

    if (dw_aux != previus_dw) {
        if (dw_aux > previus_dw) {
            rampita = rampita + escalon_rampa;
            if (rampita >= dw_aux)
                previus_dw = dw_aux;
        }
        else if (dw_aux < previus_dw) {
            rampita = rampita - escalon_rampa;
            if (rampita <= dw_aux)
                previus_dw = dw_aux;
        }
    }

    AUX_VCO.in      = rampita;
    FIFTH_VCO.in    = rampita;
    SEVENTH_VCO.in  = rampita;
    ELEVENTH_VCO.in = rampita;

    // Se prepara el escalon de amplitud
    SGN_3Ph_V.K += amp_aux;
    SGN_NSC_V.K += amp_aux * k_nsc;
    SGN_5th_V.K += amp_aux * k_5th;
    SGN_7th_V.K += amp_aux * k_7th;
    SGN_11th_V.K += amp_aux * k_11th;

    // Se le da la progresion a los angulos
    VCO_MACRO(AUX_VCO);
    VCO_MACRO(FIFTH_VCO);
    VCO_MACRO(SEVENTH_VCO);
    VCO_MACRO(ELEVENTH_VCO);

    // Tension
    SGN_3Ph_V.theta  = AUX_VCO.out + dtheta_aux; // dtheta_aux: variable auxiliar para escalon de fase en tension
    SGN_NSC_V.theta  = AUX_VCO.out + dtheta_aux;
    SGN_5th_V.theta  = FIFTH_VCO.out + dtheta_aux;
    SGN_7th_V.theta  = SEVENTH_VCO.out + dtheta_aux;
    SGN_11th_V.theta = ELEVENTH_VCO.out + dtheta_aux;

    SIGNAL_3Ph_MACRO(SGN_3Ph_V);
    SIGNAL_3Ph_MACRO(SGN_NSC_V);
    SIGNAL_3Ph_MACRO(SGN_5th_V);
    SIGNAL_3Ph_MACRO(SGN_7th_V);
    SIGNAL_3Ph_MACRO(SGN_11th_V);

    salida.va    = SGN_3Ph_V.a + SGN_NSC_V.a + SGN_5th_V.a + SGN_7th_V.a + SGN_11th_V.a;
    salida.vb    = SGN_3Ph_V.b + SGN_NSC_V.b + SGN_5th_V.b + SGN_7th_V.b + SGN_11th_V.b;
    salida.vc    = SGN_3Ph_V.c + SGN_NSC_V.c + SGN_5th_V.c + SGN_7th_V.c + SGN_11th_V.c;
    salida.theta = SGN_3Ph_V.theta;
    salida.f     = ((AUX_VCO.wn + rampita) / TWO_PI);

    return salida;
}

/***********************************************************************
 * interrupt_init()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     0 si hubo exito, -1 si ya se inicializo
 *
 * Descripcion: Inicializa los registros para la correcta ejecucion de
 *              la interrupcion. Esto implica:
 *              - Frecuencia de 10 kHz
 *              - EPWM 1, 2 y 3 en modo sincronico up/down
 *              - Tripzone en todos los PWM, solo activado por sw
 *              - SOCA desde EPWM1
 **********************************************************************/

int interrupt_init()
{
    if (interrupt_initiated)
        return -1;

    // Se inicializan las protecciones de tripzone
    PROTECTION_INIT;

    // Dejamos deshabilitada la salida de los PWM
    gpio_disable_epwm();

    // Se inicializan los ePWM
    pwm_init123_withInterrupt();

    interrupt_initiated = 1;
    return 0;
}

/***********************************************************************
 * EPWM1_INT_ISR()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Interrupcion que se ejecuta a una frecuencia de 10 kHz.
 *              Es generada por el EPWM1
 **********************************************************************/

interrupt void EPWM1_INT_ISR(void) // PIE3.1 @ 0x000D60  EPWM1_INT (EPWM1)
{
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP3; // Must acknowledge the PIE group
    EPwm1Regs.ETCLR.bit.INT = 1;             // limpiar bandera

    // Toggle al pin de control
    // GpioDataRegs.GPATOGGLE.bit.GPIO11 = 1;

    run_interrupt();
    // secuencia_simulada();
    // run_pll ();
    // interrupcion();
}

/***********************************************************************
 * run_interrupt()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Funcion que se ejecuta en la interrupcion de ePWM1
 **********************************************************************/
void run_interrupt()
{
    board_digitalWrite(BOARD_GPIO08_COMMAND_PIN, 1); // Para medir la duracion de la interrupcion

    SIGNAL_3Magn    v;
    SIGNAL_3Magn    i;
    PLL_VARS        red_trifasica;

    // ******************** ADQUISICION DE VALORES ANALOGICOS ******************** //

    // Tensiones trifasicas
    v.a = ((int)(AdcRegs.ADCRESULT13 >> 4) - OFFSET_RV) * KRV; // R
    v.b = ((int)(AdcRegs.ADCRESULT15 >> 4) - OFFSET_SV) * KSV; // S
    v.c = ((int)(AdcRegs.ADCRESULT14 >> 4) - OFFSET_TV) * KTV; // T

    // Tension de bus
    Yv = ((int)(AdcRegs.ADCRESULT9 >> 4) - OFFSET_YV) * KYV;

    // Corrientes
    i.a = ((int)(AdcRegs.ADCRESULT10 >> 4) - OFFSET_RI) * KRI; // R
    i.b = ((int)(AdcRegs.ADCRESULT8 >> 4) - OFFSET_SI) * KSI;  // S
    i.c = ((int)(AdcRegs.ADCRESULT6 >> 4) - OFFSET_TI) * KTI;  // T

// Adquisicion de senales simuladas
#if SIM_INPUT_ENABLED == 1
    tension_simulada = sim_get();
    Rv               = tension_simulada.va;
    Sv               = tension_simulada.vb;
    Tv               = tension_simulada.vc;
#endif

    // ******************** PROTECCIONES  ******************** //
    PROTECTION_MACRO(Rv, Sv, Tv, Yv, Ri, Si, Ti);

    // **** Trip zone simulado
    if (GpioDataRegs.GPADAT.bit.GPIO12 == 0)
        trip_events++;

    if (GpioDataRegs.GPADAT.bit.GPIO12 == 1)
        trip_events = 0;

    if (trip_events > MAX_TRIP_EVENTS)
        FORCE_TRIPZONE_INTERRUPT;

    // Ejecucion del PLL
    red_trifasica = run_pll(Rv, Sv, Tv);

    // Ejecucion de la MEF del sistema
    run_converter_mef(v, i, red_trifasica, Yv);

    // ******************** Salida ******************** //
    // Aca se reproduce la salida en el modulador vectorial
    imprimir_vector_vbus(modulo_out, theta_out, Yv);

    board_digitalWrite(BOARD_GPIO08_COMMAND_PIN, 0); // Para medir la duracion de la interrupcion
}

/***********************************************************************
 * run_converter_mef()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     Siempre 0
 *
 * Descripcion: Funcion encargada de la evolucion del comportamiento
 *              del sistema.
 **********************************************************************/

static int run_converter_mef(SIGNAL_3Magn v, SIGNAL_3Magn i, PLL_VARS grid, float vbus)
{
    static CONVERTER_STATE estado = CONVERTER_IDLE;
    static unsigned int cuenta_precarga  = 0;
    static unsigned int cuenta_aux       = 0;
    static unsigned int cuenta_aux2      = 0;
    static char         f_kp             = 0;
    static char         f_stopping       = 0; // Flag stopping
    int                 encender_sistema = 0; // TODO: borrar
    int stop_with_discharge = 0; // TODO: borrar

    switch (estado) {
        case CONVERTER_IDLE:

            // Dejamos abiertos todos los contactos, reescribir la orden no afecta
            board_openContact(BOARD_IPM_COMMAND_PIN);
            board_openContact(BOARD_CAC1_COMMAND_PIN);
            board_openContact(BOARD_CDC1_COMMAND_PIN);
            board_openContact(BOARD_PRECHARGE_RELAY_COMMAND_PIN);

            if (encender_sistema) {
                int error = 0;

                if (board_isOpened(BOARD_QAC1_STATE_PIN) == 1) {
                    ADD_ERROR(ERROR_QAC_STATE);
                    error = 1;
                }

                if (board_isOpened(BOARD_QDC1_STATE_PIN) == 1) {
                    ADD_ERROR(ERROR_QDC_STATE);
                    error = 1;
                }

                if (!error) {
                    board_closeContact(BOARD_IPM_COMMAND_PIN);             // Le damos tension a la placa de los IGBT
                    board_closeContact(BOARD_PRECHARGE_RELAY_COMMAND_PIN); // Activamos el rele de precarga
                    cuenta_precarga = 0;
                    cuenta_aux      = 0;
                    cuenta_aux2     = 0;
                    estado          = CONVERTER_PRECHARGING_PASSIVE;
                }
            }
            break;

        case CONVERTER_PRECHARGING_PASSIVE: {
            cuenta_aux2++;
            
            // El valor minimo modulable es la tension de linea de pico, grid.mod mide la tension pico de fase, asi que el valor es ese multiplicado por raiz de 3 (1.7)
            if (vbus > 1.4 * grid.mod)
                cuenta_precarga++;

            // El bus esta cargado a la tension de diodo
            if (cuenta_precarga > 2) {
                cuenta_aux++;

                if (cuenta_aux > 5000) {
                    // Cerramos el contactor
                    board_closeContact(BOARD_CAC1_COMMAND_PIN);
                }

                // Luego de 0.5 segundos abro el rele de precarga y transicionamos
                if (cuenta_aux > 10000) {
                    board_openContact(BOARD_PRECHARGE_RELAY_COMMAND_PIN);

                    // Transicionamos
                    cuenta_aux = 0;
                    // Vbus_sp = 40;
                    //pi_id.x = 0; // TODO: Hay que ponerlo 06/10/2026
                    //pi_iq.x = 0; // TODO: Hay que ponerlo 06/10/2026
                    // pi_vbus.x = 0.94;
                    estado = CONVERTER_PRECHARGING_ACTIVE;
                }

                // Si tarda demasiado, reportamos error
                if (cuenta_aux > 100000) {
                    ADD_ERROR(ERROR_PRECHARGE_TIMEOUT);
                    estado = CONVERTER_IDLE;
                }
            }
        } break;

        case CONVERTER_PRECHARGING_ACTIVE: {
            static int cuenta_transicion = 0;
            cuenta_aux++;
            ROTATING_VECTOR out;

            control_run(v, i, grid, vbus, Vbus_sp, &out);

            // Luego de 1 ms le doy señal a las llaves
            if (cuenta_aux > 10 && f_kp == 0) {
                gpio_enable_epwm();
                f_kp = 1;
            }

            if ((Vbus_sp * 0.9 < vbus && vbus < Vbus_sp * 1.1) && f_kp == 1) {
                cuenta_transicion++;
            }
            else {
                cuenta_transicion = 0;
            }

            // Esperamos 5 segundos
            // if (cuenta_aux > 14000)
            if (cuenta_aux > 2500) {
                cuenta_aux--;
                // Transicionamos
                cuenta_aux = 0;
                // pi_vbus.x = 0.94;
                // pi_id.x = -1.4;
                //pi_vbus.x = 0; // TODO: importante incluir
                // pi_id.x = 0;
                board_closeContact(BOARD_CDC1_COMMAND_PIN);
                f_kp   = 0;
                estado = CONVERTER_WORKING;
            }
        } break;

        case CONVERTER_WORKING: {
            ROTATING_VECTOR out;
            float           vsp = 100;

            control_run(v, i, grid, vbus, vsp, &out);

            // Transiciones
            if (stop_with_discharge) {
                stop_with_discharge = 0;
                estado              = CONVERTER_STOPPING_WITH_DISCHARGE;
            }
        } break;

        case CONVERTER_STOPPING_WITH_DISCHARGE: {
            if (f_stopping == 0) {
                cuenta_aux = 0;
                f_stopping = 1;
                //control_enabled = 0;    // Le quito la potestad al control automatico
                gpio_disable_epwm(); // Apagamos el modulo ePWM y ponemos todas las salidas a 0
            }

            // Espero un milisegundo y me desconecto de la alterna
            if (cuenta_aux > 10) {
                board_openContact(BOARD_CAC1_COMMAND_PIN); // Abro el contactor de AC
                if (vbus < VBUS_LOWER || cuenta_aux > 100000) {
                    board_openContact(BOARD_CDC1_COMMAND_PIN);
                    board_openContact(BOARD_IPM_COMMAND_PIN);
                    encender_sistema = 0;
                    f_stopping       = 0;
                    estado           = CONVERTER_IDLE;
                }
            }

            cuenta_aux++;
        } break;

        case CONVERTER_STOPPING_WITHOUT_DISCHARGE: {
            // No hacemos nada
        } break;

        default:
            break;
    }

    return 0;
}

void init_interrupts_macros()
{

    PROTECTION_INIT;

    // No se pq no arranca asi!!!!!
    //pi_id.x = 0;

#if SIM_INPUT_ENABLED == 1
    sim_init();
#endif
}

//extern int encender_sistema;
extern int stop_without_discharge;
extern int stop_with_discharge;

CONVERTER_STATE estado = CONVERTER_IDLE;

/*
 *
 *
 *  // ************ Representacion de las variables internas ************ //

#if SHOW_CURRENT_EVOLUTION == 1
  // ******************** Medición de respuesta de control corriente
  // La resolucion son 3 amperes cada 3 V.
  float amplitud_max_i = 3;
  float idmedida;

  if (i_park.d < 0.01)
      idmedida = 0;
  else
      idmedida = i_park.d;

  int vsp = PWM_COUNT_PERIOD * (1 - (idsp / amplitud_max_i));
  int vmed = PWM_COUNT_PERIOD * (1 - (idmedida / amplitud_max_i));

  EPwm4Regs.CMPA.half.CMPA = vsp;
  EPwm4Regs.CMPB = vmed;

#elif SHOW_PHASE_ERROR == 1

  // Evolucion de la tension del PLL ante un escalon en la fase
  theta_error.a = tension_simulada.theta; // Tension simulada
  theta_error.b = red_trifasica.theta;    // Tension del PLL
  ANG_DIFF_MACRO(theta_error);

  // Error en fase, centrado en 1.5V (ESCALA: 1v = 30 GRADOS)
  EPwm4Regs.CMPA.half.CMPA = PWM_COUNT_PERIOD * (1 - ((theta_error.out / ONE_SIXTH_PI) + 1.5) / 3.3);

  // Fase A, centrada en 1.5V
  EPwm4Regs.CMPB = PWM_COUNT_PERIOD * (1 - ((tension_simulada.a) / amplitud_v + 1.5) / 3.3);

#elif SHOW_FREQ_ERROR == 1

  // Frecuencia digital
  EPwm4Regs.CMPA.half.CMPA = PWM_COUNT_PERIOD * (1 - (tension_simulada.f - 49.75) * 2 / 3.3); // Exterioriza la variable

  // Frecuencia estimada por el PLL
  EPwm4Regs.CMPB = PWM_COUNT_PERIOD * (1 - (red_trifasica.f - 49.75) * 2 / 3.3); // Exterioriza la variable (49.5Hz = 0.5V, 50 Hz = 1.5V) (ESCALA: 1Hz=2V)
#endif
 *
 *
 * */
