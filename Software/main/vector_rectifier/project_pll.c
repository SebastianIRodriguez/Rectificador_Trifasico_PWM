/***********************************************************************
 * File:        project_pll.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se implementa el PLL
 *
 * History:
 *   19/05/2025 - original
 *   15/10/2026 - actualizacion funcional general
 **********************************************************************/

#include "project.h"
#include <Renolib.h>

// Variables para el funcionamiento del EPLL
static FE         FE_U1;    // Estimador de frecuencia
static SEQ_CORE   PSC_U1;   // Core de componente fundamental, sec. positiva
static SEQ_CORE   NSC_U1;   // Core de componente fundamental, sec. negativa
static SEQ_CORE   NSC_U3;   // Core de componente de tercer armonico
static SEQ_CORE   PSC_U3;   // Core de componente de tercer armonico
static SEQ_CORE   PSC_U5;   // Core de componente de quinto armonico positivo
static SEQ_CORE   NSC_U5;   // Core de componente de quinto armonico negativo
static SEQ_CORE   PSC_U5;   // Core de componente de quinto armonico positivo
static SEQ_CORE   PSC_U7;   // Core de componente de septimo armonico positivo
static SEQ_CORE   NSC_U7;   // Core de componente de septimo armonico negativo
static INTEGRATOR INT_CC_A; // Integrador utilizado para quitar la componente de continua del EPLL
static INTEGRATOR INT_CC_B; // Integrador utilizado para quitar la componente de continua del EPLL
static INTEGRATOR INT_CC_C; // Integrador utilizado para quitar la componente de continua del EPLL

// Habilitaciones
static float U1N_COMP_EN = 1; // Habilita la compensacion de secuencia fundamental negativa
static float U3P_COMP_EN = 0; // Habilita la compensacion de tercer armonico sec positiva
static float U3N_COMP_EN = 0; // Habilita la compensacion de tercer armonico sec negativa
static float U5P_COMP_EN = 0; // Habilita la compensacion de quinto armonico sec positiva
static float U5N_COMP_EN = 1; // Habilita la compensacion de quinto armonico sec negativa
static float U7P_COMP_EN = 1; // Habilita la compensacion de septimo armonico sec positiva
static float U7N_COMP_EN = 0; // Habilita la compensacion de septimo armonico sec negativa

static float U1N_OUT_EN = 1; // Habilita la salida de secuencia fundamental negativa
static float U3P_OUT_EN = 0; // Habilita la salida de tercer armonico sec positiva
static float U3N_OUT_EN = 0; // Habilita la salida de tercer armonico sec negativa
static float U5P_OUT_EN = 0; // Habilita la salida de quinto armonico sec positiva
static float U5N_OUT_EN = 1; // Habilita la salida de quinto armonico sec negativa
static float U7P_OUT_EN = 1; // Habilita la salida de septimo armonico sec positiva
static float U7N_OUT_EN = 0; // Habilita la salida de septimo armonico sec negativa

static float EPLL_u0 = 0;    // Variable de ajuste para la integracion de la componenete de continua
static float EPLL_u1 = 200;  // Variable de ajuste para los cores (amplitud) - 200
static float EPLL_u2 = 5000; // Ganancia del estimulador, una constante que se ajusta para el funcionamiento del estimador de frecuencia, originalmente 20, luego 12000
static float EPLL_u3 = 200;  // Variable de ajuste para los cores (fase) - 200

/***********************************************************************
 * project_pll_init()
 * 
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     Siempre 0
 *
 * Descripcion: Inicializa las variables internas del PLL
 **********************************************************************/
int project_pll_init()
{
    float time_step = 1 / PROJECT_F_INTERRUPT;

    // Cores
    SEQ_CORE_init(&PSC_U1, 1, 1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step);  // Fundamental
    SEQ_CORE_init(&NSC_U1, 1, -1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step); // Fundamental secuencia negativa
    SEQ_CORE_init(&NSC_U3, 3, -1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step); // Tercer armonico, secuencia negativa
    SEQ_CORE_init(&PSC_U3, 3, 1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step);  // Tercer armonico, secuencia positiva
    SEQ_CORE_init(&NSC_U5, 5, -1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step); // Quinto armonico, secuencia negativa
    SEQ_CORE_init(&PSC_U5, 5, 1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step);  // Quinto armonico, secuencia positiva
    SEQ_CORE_init(&PSC_U7, 7, 1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step);  // Septimo armonico, secuencia positiva
    SEQ_CORE_init(&NSC_U7, 7, -1, EPLL_u1, EPLL_u3, TWO_PI * PROJECT_F_NOM, time_step); // Septimo armonico, secuencia positiva

    // Integradores para la eliminacion de offset
    INTEGRATOR_init(&INT_CC_A, time_step);
    INTEGRATOR_init(&INT_CC_B, time_step);
    INTEGRATOR_init(&INT_CC_C, time_step);

    // Estimador de frecuencia
    FE_init(&FE_U1, EPLL_u2, TWO_PI * PROJECT_F_NOM, time_step);

    return 0;
}

/***********************************************************************
 * project_pll_run(Vr, Vs, Vt)
 * 
 * Entrada:     Vr  Tension R - Neutro
 *              Vs  Tension S - Neutro
 *              Vt  Tension T - Neutro
 *
 * Salida:      No posee
 *
 * Retorno:     Variables obtenidas del PLL
 *
 * Descripcion: Ejecuta un paso de integracion del PLL.
 *              Se utiliza la estructura de EPLL2 (Enhanced PLL 2)
 *              Basado en cores. 
 *              Los estimadores disponibles son:
 *              - Frecuencia fundamental, secuencia positiva y negativa
 *              - 3° armonico, secuencia positiva y negativa
 *              - 5° armonico, secuencia positiva y negativa
 *              - 7° armonico, secuencia positiva y negativa
 *              - Componente de continua
 **********************************************************************/
PLL_VARS project_pll_run(float Vr, float Vs, float Vt)
{
    PLL_VARS salida;
    float    VPLL[3];

    // ****************************** Se utiliza el EPLL (Enhanced PLL 2) basado en cores U
    // *** Estimacion de frecuencia
    FE_U1.in = PSC_U1.fe_out; // Se da como entrada la frecuencia calculada en la secuencia positiva pasada (salida del propio PSC_CORE)
    FE_MACRO(FE_U1);          // Se corre el estimador de frecuencia
    PSC_U1.wo = FE_U1.out;    // Se actualiza la frecuencia (velocidad angular)

    // *** Calculo de tension del PLL con los valores pasados de las distintas secuencias
    VPLL[0] = PSC_U1.out[0] + U1N_COMP_EN * NSC_U1.out[0] + U3P_COMP_EN * PSC_U3.out[0] + U3N_COMP_EN * NSC_U3.out[0] + U5P_COMP_EN * PSC_U5.out[0] + U5N_COMP_EN * NSC_U5.out[0] + U7P_COMP_EN * PSC_U7.out[0] + U7N_COMP_EN * NSC_U7.out[0];
    VPLL[1] = PSC_U1.out[1] + U1N_COMP_EN * NSC_U1.out[1] + U3P_COMP_EN * PSC_U3.out[1] + U3N_COMP_EN * NSC_U3.out[1] + U5P_COMP_EN * PSC_U5.out[1] + U5N_COMP_EN * NSC_U5.out[1] + U7P_COMP_EN * PSC_U7.out[1] + U7N_COMP_EN * NSC_U7.out[1];
    VPLL[2] = PSC_U1.out[2] + U1N_COMP_EN * NSC_U1.out[2] + U3P_COMP_EN * PSC_U3.out[2] + U3N_COMP_EN * NSC_U3.out[2] + U5P_COMP_EN * PSC_U5.out[2] + U5N_COMP_EN * NSC_U5.out[2] + U7P_COMP_EN * PSC_U7.out[2] + U7N_COMP_EN * NSC_U7.out[2];

    // *** Realimentamos los nucleos con la entrada - el valor calculado y la componente de continua
    PSC_U1.e[0] = Vr - VPLL[0] - INT_CC_A.out;
    PSC_U1.e[1] = Vs - VPLL[1] - INT_CC_B.out;
    PSC_U1.e[2] = Vt - VPLL[2] - INT_CC_C.out;
    SEQ_CORE_MACRO(PSC_U1); // Ejecucion de macro de comp. fundamental, sec. positiva

    NSC_U1.wo   = FE_U1.out;
    NSC_U1.e[0] = PSC_U1.e[0];
    NSC_U1.e[1] = PSC_U1.e[1];
    NSC_U1.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(NSC_U1); // Ejecucion de macro de comp. fundamental, sec. negativa

    NSC_U3.wo   = FE_U1.out;
    NSC_U3.e[0] = PSC_U1.e[0];
    NSC_U3.e[1] = PSC_U1.e[1];
    NSC_U3.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(NSC_U3); // Ejecucion de macro de terc arm. (sec negativa)

    PSC_U3.wo   = FE_U1.out;
    PSC_U3.e[0] = PSC_U1.e[0];
    PSC_U3.e[1] = PSC_U1.e[1];
    PSC_U3.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(PSC_U3); // Ejecucion de macro de terc arm. (sec positiva)

    NSC_U7.wo   = FE_U1.out;
    NSC_U7.e[0] = PSC_U1.e[0];
    NSC_U7.e[1] = PSC_U1.e[1];
    NSC_U7.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(NSC_U7); // Ejecucion de macro de septimo arm. sec negativa

    PSC_U5.wo   = FE_U1.out;
    PSC_U5.e[0] = PSC_U1.e[0];
    PSC_U5.e[1] = PSC_U1.e[1];
    PSC_U5.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(PSC_U5); // Ejecucion de macro de quinto arm. (sec popsitiva)

    NSC_U5.wo   = FE_U1.out;
    NSC_U5.e[0] = PSC_U1.e[0];
    NSC_U5.e[1] = PSC_U1.e[1];
    NSC_U5.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(NSC_U5); // Ejecucion de macro de quinto arm. (sec negativa)

    PSC_U7.wo   = FE_U1.out;
    PSC_U7.e[0] = PSC_U1.e[0];
    PSC_U7.e[1] = PSC_U1.e[1];
    PSC_U7.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(PSC_U7); // Ejecucion de macro de septimo arm (sec positiva).

    INT_CC_A.in = EPLL_u0 * PSC_U1.e[0];
    INT_CC_B.in = EPLL_u0 * PSC_U1.e[1];
    INT_CC_C.in = EPLL_u0 * PSC_U1.e[2];
    INTEGRATOR_MACRO(INT_CC_A); // Ejecucion de macro de integrador para elim. de offset, fase A.
    INTEGRATOR_MACRO(INT_CC_B); // Ejecucion de macro de integrador para elim. de offset, fase B.
    INTEGRATOR_MACRO(INT_CC_C); // Ejecucion de macro de integrador para elim. de offset, fase C.

    // Devolvemos los valores de interes
    salida.f     = FE_U1.out / TWO_PI;
    salida.theta = PSC_U1.CORE_VCO.out;
    salida.mod   = PSC_U1.Magn.out;
    salida.va    = PSC_U1.out[0] + U1N_OUT_EN * NSC_U1.out[0] + U3P_OUT_EN * PSC_U3.out[0] + U3N_OUT_EN * NSC_U3.out[0] + U5P_OUT_EN * PSC_U5.out[0] + U5N_OUT_EN * NSC_U5.out[0] + U7P_OUT_EN * PSC_U7.out[0] + U7N_OUT_EN * NSC_U7.out[0];
    salida.vb    = PSC_U1.out[1] + U1N_OUT_EN * NSC_U1.out[1] + U3P_OUT_EN * PSC_U3.out[1] + U3N_OUT_EN * NSC_U3.out[1] + U5P_OUT_EN * PSC_U5.out[1] + U5N_OUT_EN * NSC_U5.out[1] + U7P_OUT_EN * PSC_U7.out[1] + U7N_OUT_EN * NSC_U7.out[1];
    salida.vc    = PSC_U1.out[2] + U1N_OUT_EN * NSC_U1.out[2] + U3P_OUT_EN * PSC_U3.out[2] + U3N_OUT_EN * NSC_U3.out[2] + U5P_OUT_EN * PSC_U5.out[2] + U5N_OUT_EN * NSC_U5.out[2] + U7P_OUT_EN * PSC_U7.out[2] + U7N_OUT_EN * NSC_U7.out[2];

    return salida;
}
