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
static CLARKE v_clarke, v_clarko;
static PARK v_park, v_parko;
static PARK i_park;
static PLL_VARS sec_positiva;
static PI_CONTROLLER vpi_d, vpi_q;
extern int activo;
float kadj = 1;

float Ri, Si, Ti;           // Corrientes, solo usamos RST
float Rv, Sv, Tv, Yv; // Tensiones, solo usamos RST

float sRv, sSv, sTv; // Simuladas

float modmod = 18;

//***** Habilitacion de terminos de control
float PI_ENABLED = 1;
float I_FF_ENABLED = 0;

float kp_control_i = 0;
float ki_control_i = 0;


//float misLecturas[1000];
//int mi_indice = 0;
//Ui = ((int)(AdcRegs.ADCRESULT4 >> 4) - OFFSET_UI) * KUI;
//Vi = ((int)(AdcRegs.ADCRESULT2 >> 4) - OFFSET_VI) * KVI;
//Wi = ((int)(AdcRegs.ADCRESULT0 >> 4) - OFFSET_WI) * KWI;
//Rv = ((AdcRegs.ADCRESULT13 >> 4) - OFFSET_RV) * KRV;
//Sv = ((AdcRegs.ADCRESULT15 >> 4) - OFFSET_SV) * KSV;
//Tv = ((AdcRegs.ADCRESULT14 >> 4) - OFFSET_TV) * KTV;
//B1v = ((int)(AdcRegs.ADCRESULT7 >> 4) - OFFSET_B1V) * KB1;
//B2v = ((int)(AdcRegs.ADCRESULT12 >> 4) - OFFSET_B2V) * KB2;

extern SIGNAL_3Ph SGN_3Ph_V; // Estructuras que contienen 3 variables, una amplitud y un ángulo
extern VCO FIFTH_VCO;                   // Guarda el angulo de la señal simulada de quinto armonico
extern VCO SEVENTH_VCO;                 // Guarda el angulo de la señal simulada de septimo armonico
extern VCO ELEVENTH_VCO;                // Guarda el angulo de la señal simulada de onceavo armonico
extern SIGNAL_3Ph SGN_NSC_V;            // Contiene la señal de armonico principal, secuencia negativa simulada
extern SIGNAL_3Ph SGN_5th_V;            // Contiene la señal de quinto armonico simulada
extern SIGNAL_3Ph SGN_7th_V;            // Contiene la señal de septimo armonico simulada
extern SIGNAL_3Ph SGN_11th_V;           // Contiene la señal de onceavo armonico simulada
extern float k_nsc = 0.0;              // Porcentaje de la amplitud principal que representa la secuencia negativa
extern float k_5th = 0.0;              // Porcentaje de la amplitud principal que representa el quinto armonico
extern float k_7th = 0.0;              // Porcentaje de la amplitud principal que representa el septimo armonico
extern float k_11th = 0.0;              // Porcentaje de la amplitud principal que representa el onceavo armonico
extern VCO AUX_VCO;                     // Guarda el ángulo de la señal simulada
extern float dw_aux;              // Variable auxiliar para dar un escalon de frecuencia
extern float time_step;   // Paso de tiempo entre cada interrupcion

float amplitud_v = 10;
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
}

void sim_get(float *va, float *theta)
{

    // Se le da la progresion a los angulos
    VCO_MACRO(AUX_VCO);
    VCO_MACRO(FIFTH_VCO);
    VCO_MACRO(SEVENTH_VCO);
    VCO_MACRO(ELEVENTH_VCO);

    // Tension
    SGN_3Ph_V.theta = AUX_VCO.out;
    SGN_NSC_V.theta = AUX_VCO.out;
    SGN_5th_V.theta = FIFTH_VCO.out;
    SGN_7th_V.theta = SEVENTH_VCO.out;
    SGN_11th_V.theta = ELEVENTH_VCO.out;

    SIGNAL_3Ph_MACRO(SGN_3Ph_V);
    SIGNAL_3Ph_MACRO(SGN_NSC_V)
    SIGNAL_3Ph_MACRO(SGN_5th_V);
    SIGNAL_3Ph_MACRO(SGN_7th_V);
    SIGNAL_3Ph_MACRO(SGN_11th_V);
    *va = SGN_3Ph_V.a + SGN_NSC_V.a + SGN_5th_V.a + SGN_7th_V.a + SGN_11th_V.a; // Modificacion de valores medidos por ADC, se reemplazan
    *theta = SGN_3Ph_V.theta;

}


/**
 * @brief Esta funcion se llama dentro de la interrupcion por EPWM1
 *
 */
void run_interrupt()
{
    static char onetime = 1; // Se usa si se toman datos simulados

    if(!activo)
        return;

    if(onetime)
    {
        onetime = 0;
        //sim_init();
        PI_CONTROLLER_init(&vpi_d, kp_control_i, ki_control_i, 1/F_PWM);
        PI_CONTROLLER_init(&vpi_q, kp_control_i, ki_control_i, 1/F_PWM);
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


    // TODO: modificar
    //float modsim, thetasim;
    //sim_get(&modsim, &thetasim);
    //imprimir_vector_vbus(amplitud_v, thetasim, Yv);

    // ******************** Ejecucion del PLL
    sec_positiva = run_pll(Rv, Sv, Tv);

    // ******************** Transformadas

    // ********** CLARKE

    // ***** Tension de secuancia positiva
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

    if((49 < sec_positiva.f) &&  (sec_positiva.f < 51))
    {
        w = 2 * M_PI * sec_positiva.f;

        // ***** Rama superior
        vpi_d.sp = idsp;
        vpi_d.fb = i_park.d;
        vpi_d.Kp = kp_control_i;
        vpi_d.Ki = ki_control_i;
        PI_CONTROLLER_MACRO(vpi_d);
        v_parko.d = v_park.d + vpi_d.out * PI_ENABLED - I_FF_ENABLED * i_park.q * w * L;

        // ***** Rama inferior
        vpi_q.sp = iqsp;
        vpi_q.fb = i_park.q;
        vpi_q.Kp = kp_control_i;
        vpi_q.Ki = ki_control_i;
        PI_CONTROLLER_MACRO(vpi_q);
        v_parko.q = v_park.q + vpi_q.out * PI_ENABLED + I_FF_ENABLED * i_park.d * w * L;

        //v_parko.d = v_park.d; Esto era para copiar la entrada
        //v_parko.q = v_park.q;
        //v_parko.z = v_park.z;
    }

    // ******************** Antitransformadas

    // ***** PARK
    // Pensar si incluir z / zero en antitransf
    v_parko.cos = cos(sec_positiva.theta);
    v_parko.sin = sin(sec_positiva.theta);
    float modulo_out = sqrt(v_parko.d * v_parko.d + v_parko.q * v_parko.q);
    iPARK_MACRO(v_parko);

    // ***** Clarke
    v_clarko.alpha = v_parko.alpha;
    v_clarko.beta = v_parko.beta;
    v_clarko.zero = v_parko.zero;
    iCLARKE_MACRO(v_clarko);

    float thetao = atan2(v_clarko.beta, v_clarko.alpha) +  2 * PI;

    // ******************** Salida
    // Aca se reproduce la salida en el modulador vectorial
    imprimir_vector_vbus(modulo_out, thetao, Yv);

}


void init_interrupts_macros()
{
    CLARKE_init(&i_clarke);
    CLARKE_init(&v_clarke);
    CLARKE_init(&v_clarko);

    PARK_init(&v_park);
    PARK_init(&v_parko);
    PARK_init(&i_park);
}

