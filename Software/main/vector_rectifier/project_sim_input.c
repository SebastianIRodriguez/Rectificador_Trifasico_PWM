/***********************************************************************
 * File:        project_sim_input.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se definen las funciones para la adquisicion simulada
 *              de tension
 *
 * History:
 *   15/10/2026 - original
 **********************************************************************/

#include "project.h"
#include <renolib.h>

static float amplitud_v = 1;   // Amplitud de la tension simulada
static float k_nsc      = 0.0; // Porcentaje de la amplitud principal que representa la secuencia negativa
static float k_5th      = 0.0; // Porcentaje de la amplitud principal que representa el quinto armonico
static float k_7th      = 0.0; // Porcentaje de la amplitud principal que representa el septimo armonico
static float k_11th     = 0.0; // Porcentaje de la amplitud principal que representa el onceavo armonico

static float time_step = 1.0 / PROJECT_F_INTERRUPT; // Paso de tiempo entre cada interrupcion

static VCO MAIN_VCO;     // Guarda el angulo de la senal simulada de armonico principal
static VCO FIFTH_VCO;    // Guarda el angulo de la senal simulada de quinto armonico
static VCO SEVENTH_VCO;  // Guarda el angulo de la senal simulada de septimo armonico
static VCO ELEVENTH_VCO; // Guarda el angulo de la senal simulada de onceavo armonico

static SIGNAL_3Ph SGN_3Ph_V;  // Contine la senal de secuencia principal positiva
static SIGNAL_3Ph SGN_NSC_V;  // Contiene la senal de armonico principal, secuencia negativa simulada
static SIGNAL_3Ph SGN_5th_V;  // Contiene la senal de quinto armonico simulada
static SIGNAL_3Ph SGN_7th_V;  // Contiene la senal de septimo armonico simulada
static SIGNAL_3Ph SGN_11th_V; // Contiene la senal de onceavo armonico simulada

// *** Para los escalones de la senal simulada
volatile float amp_aux    = 0;   // Variable auxiliar para dar un escalon en tension
volatile float dw_aux     = 0.0; // Variable auxiliar para dar un escalon de frecuencia (medio HZ = 0,0795 rad/s)
volatile float dtheta_aux = 0.0; // Variable auxiliar para dar un escalon de fase en tensión (escalon de 30 grados = 0.5235)

/***********************************************************************
 * project_sim_input_init()
 * 
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     Siempre 0
 *
 * Descripcion: Inicia los sistemas necesarios para la simulacion de 
 *              las señales de entrada
 **********************************************************************/
int project_sim_input_init()
{
    // Senales simuladas
    SIGNAL_3Ph_init(&SGN_3Ph_V, amplitud_v);
    SIGNAL_3Ph_init(&SGN_NSC_V, amplitud_v * k_nsc * -1); // Le damos secuencia negativa
    SIGNAL_3Ph_init(&SGN_5th_V, amplitud_v * k_5th * -1); // Le damos secuencia negativa
    SIGNAL_3Ph_init(&SGN_7th_V, amplitud_v * k_7th);
    SIGNAL_3Ph_init(&SGN_11th_V, amplitud_v * k_11th);

    // Angulos
    VCO_init(&MAIN_VCO, TWO_PI * PROJECT_F_NOM, time_step);
    VCO_init(&FIFTH_VCO, TWO_PI * PROJECT_F_NOM * 5, time_step);
    VCO_init(&SEVENTH_VCO, TWO_PI * PROJECT_F_NOM * 7, time_step);
    VCO_init(&ELEVENTH_VCO, TWO_PI * PROJECT_F_NOM * 11, time_step);

    return 0;
}

/***********************************************************************
 * project_sim_input_init()
 * 
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     Siempre 0
 *
 * Descripcion: Esta funcion devuelve una trifasica simulada con 
 *              diversas componentes armonicas.
 *              Tambien permite generar escalones o rampas (segun el 
 *              caso) en amplitud, fase y frecuencia mediante las va-
 *              riables globales disponibles
 **********************************************************************/
PLL_VARS project_sim_input_get()
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

    MAIN_VCO.in     = rampita;
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
    VCO_MACRO(MAIN_VCO);
    VCO_MACRO(FIFTH_VCO);
    VCO_MACRO(SEVENTH_VCO);
    VCO_MACRO(ELEVENTH_VCO);

    // Tension
    SGN_3Ph_V.theta  = MAIN_VCO.out + dtheta_aux; // dtheta_aux: variable auxiliar para escalon de fase en tension
    SGN_NSC_V.theta  = MAIN_VCO.out + dtheta_aux;
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
    salida.f     = ((MAIN_VCO.wn + rampita) / TWO_PI);

    return salida;
}
