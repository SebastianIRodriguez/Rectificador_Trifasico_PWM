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

#include "project.h"
#include "calibration.h"
#include <CLARKE.h>
#include <PARK.h>
#include <PI_CONTROLLER.h>
#include <math.h>
#include "Renolib.h"

#define CONTROL_PI_ENABLE 0
#define TENSION_SIMULADA 1

#define SHOW_CURRENT_EVOLUTION 0
#define SHOW_PHASE_ERROR 1

// TODO: Agregar seguridad, tiempo de enganche PLL, trip-zone
// TODO: Hacer un init de todo
// TODO: contar el tiempo de la interrupci�n con el timer del PWM
// TODO: Ver los valores de w de L
// TODO: Asignar valores de Ki y Kp, tal vez iniciar Ki despues

volatile float idsp = 0; // Corriente en directa de set point
volatile float iqsp = 0; // Corriente en cuadratura de set point
volatile float w = 1;
volatile const float L = 0.001; // La inductacia del tablero

static CLARKE i_clarke;
static CLARKE v_clarke, v_clark_out;
static PARK v_park, v_park_out;
static PARK i_park;
static PLL_VARS sec_positiva;
static PI_CONTROLLER vpi_d, vpi_q;
extern int activo;
float kadj = 1;

float Ri, Si, Ti;     // Corrientes, solo usamos RST
float Rv, Sv, Tv, Yv; // Tensiones, solo usamos RST

float sRv, sSv, sTv; // Simuladas

float modmod = 18;

// ***** Habilitacion de terminos de control
float PI_ENABLED = 1;
float I_FF_ENABLED = 0;

float kp_control_i = 0;
float ki_control_i = 0;

// ***** Simulacion de trifasica
extern SIGNAL_3Ph SGN_3Ph_V;  // Estructuras que contienen 3 variables, una amplitud y un ángulo
extern VCO FIFTH_VCO;         // Guarda el angulo de la señal simulada de quinto armonico
extern VCO SEVENTH_VCO;       // Guarda el angulo de la señal simulada de septimo armonico
extern VCO ELEVENTH_VCO;      // Guarda el angulo de la señal simulada de onceavo armonico
extern SIGNAL_3Ph SGN_NSC_V;  // Contiene la señal de armonico principal, secuencia negativa simulada
extern SIGNAL_3Ph SGN_5th_V;  // Contiene la señal de quinto armonico simulada
extern SIGNAL_3Ph SGN_7th_V;  // Contiene la señal de septimo armonico simulada
extern SIGNAL_3Ph SGN_11th_V; // Contiene la señal de onceavo armonico simulada
extern float k_nsc = 0.0;     // Porcentaje de la amplitud principal que representa la secuencia negativa
extern float k_5th = 0.0;     // Porcentaje de la amplitud principal que representa el quinto armonico
extern float k_7th = 0.0;     // Porcentaje de la amplitud principal que representa el septimo armonico
extern float k_11th = 0.0;    // Porcentaje de la amplitud principal que representa el onceavo armonico
extern VCO AUX_VCO;           // Guarda el ángulo de la señal simulada
extern float dw_aux;          // Variable auxiliar para dar un escalon de frecuencia
extern float time_step;       // Paso de tiempo entre cada interrupcion
SIGNAL_3Ph tension_simulada;  // Contiene la tension simulada, con todos sus armonicos
float amplitud_v = 1;         // Amplitud de la tension simulada
ANG_DIFF theta_error;         // Variable utilizada para representar el error de fase

// *** Para los escalones de la senal simulada
float amp_aux = 0;      // Variable auxiliar para dar un escalon en tension
float dw_aux = 0.0;     // Variable auxiliar para dar un escalon de frecuencia
float dtheta_aux = 0.0; // Variable auxiliar para dar un escalon de fase en tensión

// ********** Parametros de salida
float theta_out = 0;
float modulo_out = 0;

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

SIGNAL_3Ph sim_get()
{
    SIGNAL_3Ph salida;

    static float previus_dw = 0;
    static float rampita = 0;
    static float escalon_rampa = 0.00125;

    if (dw_aux != previus_dw)
    {
        if (dw_aux > previus_dw)
        {
            rampita = rampita + escalon_rampa;
            if (rampita >= dw_aux)
                previus_dw = dw_aux;
        }
        else if (dw_aux < previus_dw)
        {
            rampita = rampita - escalon_rampa;
            if (rampita <= dw_aux)
                previus_dw = dw_aux;
        }
    }

    AUX_VCO.in = rampita;
    FIFTH_VCO.in = rampita;
    SEVENTH_VCO.in = rampita;
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

    // Tensión
    SGN_3Ph_V.theta = AUX_VCO.out + dtheta_aux; // dtheta_aux: variable auxiliar para escalon de fase en tension
    SGN_NSC_V.theta = AUX_VCO.out + dtheta_aux;
    SGN_5th_V.theta = FIFTH_VCO.out + dtheta_aux;
    SGN_7th_V.theta = SEVENTH_VCO.out + dtheta_aux;
    SGN_11th_V.theta = ELEVENTH_VCO.out + dtheta_aux;

    SIGNAL_3Ph_MACRO(SGN_3Ph_V);
    SIGNAL_3Ph_MACRO(SGN_NSC_V);
    SIGNAL_3Ph_MACRO(SGN_5th_V);
    SIGNAL_3Ph_MACRO(SGN_7th_V);
    SIGNAL_3Ph_MACRO(SGN_11th_V);

    salida.a = SGN_3Ph_V.a + SGN_NSC_V.a + SGN_5th_V.a + SGN_7th_V.a + SGN_11th_V.a;
    salida.b = SGN_3Ph_V.b + SGN_NSC_V.b + SGN_5th_V.b + SGN_7th_V.b + SGN_11th_V.b;
    salida.c = SGN_3Ph_V.c + SGN_NSC_V.c + SGN_5th_V.c + SGN_7th_V.c + SGN_11th_V.c;
    salida.theta = SGN_3Ph_V.theta;

    return salida;
}

/**
 * @brief Esta funcion se llama dentro de la interrupcion por EPWM1
 *
 */
void run_interrupt()
{
    static char onetime = 1; // Se usa si se toman datos simulados

    if (!activo)
        return;

    if (onetime)
    {
        onetime = 0;
        if (TENSION_SIMULADA)
            sim_init();
        PI_CONTROLLER_init(&vpi_d, kp_control_i, ki_control_i, 1 / F_PWM);
        PI_CONTROLLER_init(&vpi_q, kp_control_i, ki_control_i, 1 / F_PWM);
    }

    // ******************** ADQUISICION DE VALORES ANALOGICOS

    // Tensiones
    int mrv = (AdcRegs.ADCRESULT13 >> 4);
    int mtv = (AdcRegs.ADCRESULT14 >> 4);
    int msv = (AdcRegs.ADCRESULT15 >> 4);

    Rv = (mrv - OFFSET_RV) * KRV;
    Sv = (msv - OFFSET_SV) * KSV;
    Tv = (mtv - OFFSET_TV) * KTV;

    Yv = ((int)(AdcRegs.ADCRESULT9 >> 4) - OFFSET_YV) * KYV;

    // Corrientes
    Ri = ((int)(AdcRegs.ADCRESULT10 >> 4) - OFFSET_RI) * KRI;
    Si = ((int)(AdcRegs.ADCRESULT8 >> 4) - OFFSET_SI) * KSI;
    Ti = ((int)(AdcRegs.ADCRESULT6 >> 4) - OFFSET_TI) * KTI;

    // Adquisicion de senales simuladas
    if (TENSION_SIMULADA)
    {
        tension_simulada = sim_get();
        Rv = tension_simulada.a;
        Sv = tension_simulada.b;
        Tv = tension_simulada.c;
    }

    // ******************** Ejecucion del PLL
    sec_positiva = run_pll(Rv, Sv, Tv);

    if (CONTROL_PI_ENABLE)
    {
        // ******************** Transformadas
        // ********** CLARKE
        // ***** Tension total
        v_clarke.a = sec_positiva.va;
        v_clarke.b = sec_positiva.vb;
        v_clarke.c = sec_positiva.vc;

        // ***** Corriente total (todos los armonicos)
        i_clarke.a = Ri;
        i_clarke.b = Si;
        i_clarke.c = Ti;

        // ***** Aplicamos las transformadas de clarke
        CLARKE_MACRO(v_clarke);
        CLARKE_MACRO(i_clarke);

        // ********** PARK

        // ***** Tensiones
        v_park.alpha = v_clarke.alpha;
        v_park.beta = v_clarke.beta;
        v_park.zero = v_clarke.zero;
        v_park.cos = cos(sec_positiva.theta);
        v_park.sin = sin(sec_positiva.theta);

        // ***** Corriente
        i_park.alpha = i_clarke.alpha;
        i_park.beta = i_clarke.beta;
        i_park.zero = i_clarke.zero;
        i_park.cos = cos(sec_positiva.theta); // v_park.cos
        i_park.sin = sin(sec_positiva.theta); // v_park.sin

        // ***** Aplicamos las transformadas de park
        PARK_MACRO(v_park);
        PARK_MACRO(i_park);

        // ******************** Control
        // Por consejo de pablo, el control debe arrancar como P y luego de 0.5 segundos como P+I

        if ((49 < sec_positiva.f) && (sec_positiva.f < 51))
        {
            w = 2 * M_PI * sec_positiva.f;

            // ***** Rama superior
            vpi_d.sp = idsp;
            vpi_d.fb = i_park.d;
            vpi_d.Kp = kp_control_i;
            vpi_d.Ki = ki_control_i;
            PI_CONTROLLER_MACRO(vpi_d);
            v_park_out.d = v_park.d + vpi_d.out * PI_ENABLED - I_FF_ENABLED * i_park.q * w * L;

            // ***** Rama inferior
            vpi_q.sp = iqsp;
            vpi_q.fb = i_park.q;
            vpi_q.Kp = kp_control_i;
            vpi_q.Ki = ki_control_i;
            PI_CONTROLLER_MACRO(vpi_q);
            v_park_out.q = v_park.q + vpi_q.out * PI_ENABLED + I_FF_ENABLED * i_park.d * w * L;
        }

        // ******************** Antitransformadas

        // ***** PARK
        // Pensar si incluir z / zero en antitransf
        v_park_out.cos = cos(sec_positiva.theta);
        v_park_out.sin = sin(sec_positiva.theta);
        modulo_out = sqrt(v_park_out.d * v_park_out.d + v_park_out.q * v_park_out.q);
        iPARK_MACRO(v_park_out);

        // ***** Clarke
        v_clark_out.alpha = v_park_out.alpha;
        v_clark_out.beta = v_park_out.beta;
        v_clark_out.zero = v_park_out.zero;
        iCLARKE_MACRO(v_clark_out);

        theta_out = atan2(v_clark_out.beta, v_clark_out.alpha) + 2 * PI;
    }

    // ******************** Salida
    // Aca se reproduce la salida en el modulador vectorial
    imprimir_vector_vbus(modulo_out, theta_out, Yv);

    // ******************** Medición de respuesta de control corriente
    // La resolucion son 3 amperes cada 3 V.
    float amplitud_max_i = 3;
    float idmedida;

    if (i_park.d < 0.01)
        idmedida = 0;
    else
        idmedida = i_park.d;

    int vsp = PROJECT_PERIOD * (1 - idsp / amplitud_max_i); // 0.9090
    int vmed = PROJECT_PERIOD * (1 - idmedida / amplitud_max_i);

    // *** Representacion de las variables
    if (SHOW_CURRENT_EVOLUTION)
    {
        // Evolucion del control corriente ante un escalon en Id
        EPwm4Regs.CMPA.half.CMPA = vsp;
        EPwm4Regs.CMPB = vmed;
    }
    else if (SHOW_PHASE_ERROR)
    {
        // Evolucion de la tension del PLL ante un escalon en la fase
        theta_error.a = tension_simulada.theta; // Tension simulada
        theta_error.b = sec_positiva.theta;     // Tension del PLL
        ANG_DIFF_MACRO(theta_error);
        EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - ((theta_error.out) / ONE_SIXTH_PI + 1.5) / 3.3); // Mostramos el error en la fase (ESCALA: 1v=30 GRADOS)
        EPwm4Regs.CMPB = PROJECT_PERIOD * (1 - ((tension_simulada.a) / amplitud_v + 1.5) / 3.3);
    }
}

void init_interrupts_macros()
{
    CLARKE_init(&i_clarke);
    CLARKE_init(&v_clarke);
    CLARKE_init(&v_clark_out);

    PARK_init(&v_park);
    PARK_init(&v_park_out);
    PARK_init(&i_park);
}
