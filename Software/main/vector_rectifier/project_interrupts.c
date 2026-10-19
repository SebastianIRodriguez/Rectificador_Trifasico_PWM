/***********************************************************************
 * File:        aproject_interrupts.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se definen las funciones para manejar las interrupciones
 *              utilizadas en el proyecto
 *
 * History:
 *   13/05/2026 - original
 *   19/09/2026 - emprolijamiento del archivo
 **********************************************************************/

//#include "control_scheme.h"
#include "project.h"
#include "rectifier/board/board.h"
#include <Renolib.h>

#include <math.h>

typedef struct {
    SIGNAL_3Magn v;
    SIGNAL_3Magn i;
    PLL_VARS     pll;
} AC_MAGN;

// Variables provenientes de main.c
extern volatile int main_encender_sistema;
extern volatile int main_stop_without_discharge;
extern volatile int main_stop_with_discharge;

static int _interrupt_initiated = 0; // Evita que se inicialice dos veces la interrupcion y me rompa todo

static void run_interrupt();
static int  run_converter_mef(AC_MAGN ac_magn, float vbus);

// ********** Habilitadores ********** //
#define SIM_INPUT_ENABLED 0 // Habilita la asquisicion simulada de datos

#define SHOW_CURRENT_EVOLUTION 1
#define SHOW_PHASE_ERROR       0
#define SHOW_FREQ_ERROR        0

#define VBUS_LOWER 5.0 // Valor umbral que considera que el bus de CC esta descargado

// TODO: Agregar seguridad, tiempo de enganche PLL, trip-zone
// TODO: Ver los valores de w de L

float Vbus_sp = 100; // Set point de tension (antes 60)

#define MAX_TRIP_EVENTS 10
char trip_events = 0;

/***********************************************************************
 * project_interrupt_init()
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
 *              - SOCA desde EPWM1, canal B
 **********************************************************************/

int project_interrupt_init()
{
    if (_interrupt_initiated)
        return -1;

    // Se inicializan las protecciones de tripzone
    PROTECTION_INIT;

    // Dejamos deshabilitada la salida de los PWM
    gpio_disable_epwm();

    pwm_init123_withInterrupt();

    project_pll_init();

    _interrupt_initiated = 1;
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
    board_digitalWrite(BOARD_GPIO08_COMMAND_PIN, 1); // Para medir la duracion de la interrupcion
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP3;         // Must acknowledge the PIE group
    EPwm1Regs.ETCLR.bit.INT = 1;                     // limpiar bandera

    run_interrupt();
    board_digitalWrite(BOARD_GPIO08_COMMAND_PIN, 0); // Para medir la duracion de la interrupcion
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
static void run_interrupt()
{
    AC_MAGN  ac_magn;
    PLL_VARS pll_vars;
    float    vbus;

    // ******************** ADQUISICION DE VALORES ANALOGICOS ******************** //

    // Tensiones de red
    ac_magn.v.a = ((int)(AdcRegs.ADCRESULT13 >> 4) - OFFSET_RV) * KRV; // R
    ac_magn.v.b = ((int)(AdcRegs.ADCRESULT15 >> 4) - OFFSET_SV) * KSV; // S
    ac_magn.v.c = ((int)(AdcRegs.ADCRESULT14 >> 4) - OFFSET_TV) * KTV; // T

    // Tension de bus
    vbus = ((int)(AdcRegs.ADCRESULT9 >> 4) - OFFSET_YV) * KYV; // Yv

    // Corrientes de red
    ac_magn.i.a = ((int)(AdcRegs.ADCRESULT10 >> 4) - OFFSET_RI) * KRI; // R
    ac_magn.i.b = ((int)(AdcRegs.ADCRESULT8 >> 4) - OFFSET_SI) * KSI;  // S
    ac_magn.i.c = ((int)(AdcRegs.ADCRESULT6 >> 4) - OFFSET_TI) * KTI;  // T

// Adquisicion de senales simuladas
#if SIM_INPUT_ENABLED == 1
    tension_simulada = sim_get();
    ac_magn.v.a      = tension_simulada.va;
    ac_magn.v.b      = tension_simulada.vb;
    ac_magn.v.c      = tension_simulada.vc;
#endif

    // ******************** PROTECCIONES  ******************** //
    PROTECTION_MACRO(ac_magn.v.a, ac_magn.v.b, ac_magn.v.c, vbus, ac_magn.i.a, ac_magn.i.b, ac_magn.i.c);

    // ******************** TRIP ZONE  ******************** //
    if (GpioDataRegs.GPADAT.bit.GPIO12 == 0)
        trip_events++;

    if (GpioDataRegs.GPADAT.bit.GPIO12 == 1)
        trip_events = 0;

    if (trip_events > MAX_TRIP_EVENTS)
        FORCE_TRIPZONE_INTERRUPT;

    // ******************** EJECUCION DEL PLL  ******************** //
    pll_vars    = project_pll_run(ac_magn.v.a, ac_magn.v.b, ac_magn.v.c);
    ac_magn.pll = pll_vars;

    // ******************** EJECUCION DE LA MEF  ******************** //
    run_converter_mef(ac_magn, vbus);
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

static int run_converter_mef(AC_MAGN ac_magn, float vbus)
{
    static CONVERTER_STATE estado           = CONVERTER_IDLE;
    static unsigned int    cuenta_precarga  = 0;
    static unsigned int    cuenta_aux       = 0; // Cuenta auxiliar
    static unsigned int    cuenta_aux2      = 0; // Cuenta auxiliar 2
    static char            f_output_enabled = 0; // Flag output enabled (el PWM llega a los pines)
    static char            f_idle           = 0; // Flag idle, utilizada para retirar el pwm de los pines una sola vez
    static char            f_stopping       = 0; // Flag stopping, utilizada para retirar el pwm de los pines una sola vez
    ROTATING_VECTOR        out;                  // Vector rotante de salida

    // Variables externas a la funcion y al archivo
    int encender_sistema       = main_encender_sistema;
    int stop_with_discharge    = main_stop_with_discharge;
    int stop_without_discharge = main_stop_without_discharge;

    switch (estado) {
        case CONVERTER_IDLE:
            // Dejamos abiertos todos los contactos, reescribir la orden no afecta
            board_openContact(BOARD_IPM_COMMAND_PIN);
            board_openContact(BOARD_CAC1_COMMAND_PIN);
            board_openContact(BOARD_CDC1_COMMAND_PIN);
            board_openContact(BOARD_PRECHARGE_RELAY_COMMAND_PIN);
            if (f_idle == 0) {
                f_idle = 1;
                gpio_disable_epwm();
            }

            if (encender_sistema) {
                int error = 0;

                if (board_isOpened(BOARD_QAC1_STATE_PIN) == 1) {
                    PROJECT_ADD_ERROR(ERROR_QAC_STATE);
                    error = 1;
                }

                if (board_isOpened(BOARD_QDC1_STATE_PIN) == 1) {
                    PROJECT_ADD_ERROR(ERROR_QDC_STATE);
                    error = 1;
                }

                if (!error) {
                    board_closeContact(BOARD_IPM_COMMAND_PIN);             // Le damos tension a la placa de los IGBT
                    board_closeContact(BOARD_PRECHARGE_RELAY_COMMAND_PIN); // Activamos el rele de precarga
                    cuenta_precarga  = 0;
                    cuenta_aux       = 0;
                    cuenta_aux2      = 0;
                    f_output_enabled = 0;
                    f_stopping       = 0;
                    f_idle           = 0;
                    estado           = CONVERTER_PRECHARGING_PASSIVE;
                }
            }
            break;

        case CONVERTER_PRECHARGING_PASSIVE: {

            // El valor minimo modulable es la tension de linea de pico, pll.mod mide la tension pico de fase, asi que el valor es ese multiplicado por raiz de 3 (1.7)
            if (vbus > 1.4 * ac_magn.pll.mod)
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
                    cuenta_aux  = 0;
                    cuenta_aux2 = 0;
                    project_control_init();
                    estado = CONVERTER_PRECHARGING_ACTIVE;
                }
            }

            // Si tarda demasiado, reportamos error
            cuenta_aux2++;
            if (cuenta_aux2 > 100000) {
                PROJECT_ADD_ERROR(ERROR_PRECHARGE_TIMEOUT);
                estado = CONVERTER_IDLE;
            }
        } break;

        case CONVERTER_PRECHARGING_ACTIVE: {

            cuenta_aux++;

            // Ejecucion del control tension
            project_control_run(ac_magn.v, ac_magn.i, ac_magn.pll, vbus, Vbus_sp, &out);
            project_modulate_vector(out.mod, out.theta, vbus);

            // Luego de 1 ms le doy señal a las llaves
            if (cuenta_aux > 10 && f_output_enabled == 0) {
                gpio_enable_epwm();
                f_output_enabled = 1;
            }

            if ((Vbus_sp * 0.9 < vbus && vbus < Vbus_sp * 1.1) && f_output_enabled == 1)
                cuenta_aux2++;
            else
                cuenta_aux2 = 0;

            if (cuenta_aux2 > 10000) {
                cuenta_aux2--;
                // Transicionamos
                cuenta_aux = 0;
                board_closeContact(BOARD_CDC1_COMMAND_PIN);
                f_output_enabled = 0;
                estado           = CONVERTER_WORKING;
            }

        } break;

        case CONVERTER_WORKING: {

            project_control_run(ac_magn.v, ac_magn.i, ac_magn.pll, vbus, Vbus_sp, &out);
            project_modulate_vector(out.mod, out.theta, vbus);

            // Transiciones
            if (stop_with_discharge) {
                stop_with_discharge = 0;
                f_stopping          = 1;
                estado              = CONVERTER_STOPPING_WITH_DISCHARGE;
            }

            if (stop_without_discharge) {
                stop_without_discharge = 0;
                f_stopping             = 1;
                estado                 = CONVERTER_STOPPING_WITHOUT_DISCHARGE;
            }
        } break;

        case CONVERTER_STOPPING_WITH_DISCHARGE: {
            if (f_stopping == 0) {
                cuenta_aux = 0;
                f_stopping = 1;
                gpio_disable_epwm(); // Apagamos el modulo ePWM y ponemos todas las salidas a 0
            }

            // Espero un milisegundo y me desconecto de la alterna
            if (cuenta_aux > 10) {

                board_openContact(BOARD_CAC1_COMMAND_PIN); // Desconectamos la carga

                if (cuenta_aux > 50000) {
                    board_openContact(BOARD_CAC1_COMMAND_PIN);
                    board_openContact(BOARD_IPM_COMMAND_PIN);
                    encender_sistema = 0;
                    f_stopping       = 0;
                    estado           = CONVERTER_IDLE;
                }
            }

            cuenta_aux++;
        } break;

        case CONVERTER_STOPPING_WITHOUT_DISCHARGE: {
            if (f_stopping == 0) {
                cuenta_aux = 0;
                f_stopping = 1;
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

        default:
            break;
    }

    return 0;
}

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
