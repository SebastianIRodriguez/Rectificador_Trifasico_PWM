/**
 * @file pll.c
 * @author Luciano Raffagnini (lraffagnini21@gmail.com.com)
 * @brief En este archivo se encuentra la implementación de un PLL que debe correrse en una interrupción de 10 kHz
 * @version 0.1
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Lab.h"
#include "Renolib.h"
#include "calibration.h"

// extern void VCO_init(VCO *v, float wn, float dT);

// ********** Control de versiones
#define BUILD 1   // Build actual
#define BUILD_0 0 // Build donde se encuentra la adquisición de datos analógica y simulada
#define BUILD_1 1 // Build donde

// ********** Control de funcionamiento
#define SIM_ADQ FALSE // Indica si el PLL utiliza datos reales o simulados

#define BUILD_1_SHOW_VARIABLES 0 // Indica si se representan las variables en salidas PWM
int show_phase_error = 0;        // Elije exteriorizar el error en fase
int show_phase = 1;              // Elije exteriorizar la fase simulada y la fase que percibe el PLL
int show_freq = 0;               // Elije exteriorizar la frecuencia simulada y la percibida por el PLL
int show_phase_a = 0;            // Elije exteriorizar la fase A de la senoidal generada digitalmente

typedef struct
{
    float Va, Vb, Vc;
    float Ia, Ib, Ic;

} Trifasica;

// ********** Variables para la trifásica simulada
SIGNAL_3Ph SGN_3Ph_V, SGN_3Ph_I; // Estructuras que contienen 3 variables, una amplitud y un ángulo
float amplitud_v = 1;            // Amplitud de la tension simulada
float amplitud_i = 0.5;          // Amplitud de la corriente simulada
VCO AUX_VCO;                     // Guarda el ángulo de la señal simulada
float amp_aux = 0;               // Variable auxiliar para dar un escalon en tension
float dw_aux = 0.0;              // Variable auxiliar para dar un escalon de frecuencia
float dtheta_aux = 0.0;          // Variable auxiliar para dar un escalon de fase en tensión
float dtheta_di = 0.0;           // Variable auxiliar para dar un escalon de fase en corriente
float time_step = 1.0 / F_PWM;   // Paso de tiempo entre cada interrupcion
float offet_Va = 1.5;            // Offset de tensión para representar a la señal Va
float factor_tercero = 0;        // Porcentaje de la fundamental que es de tercer armónico de la fundamental
float factor_quinto = 0;         // Porcentaje de la fundamental que es de quinto armonico
float factor_septimo = 0;        // Porcentaje de la fundamentar que es de septimo armonico
float factor_NS = 0;             // Porcentaje de la fundamental que es de armonico principal secuencia negativa
VCO FIFTH_VCO;                   // Guarda el angulo de la señal simulada de quinto armonico
VCO SEVENTH_VCO;                 // Guarda el angulo de la señal simulada de septimo armonico
VCO ELEVENTH_VCO;                // Guarda el angulo de la señal simulada de onceavo armonico
SIGNAL_3Ph SGN_NSC_V;            // Contiene la señal de armonico principal, secuencia negativa simulada
SIGNAL_3Ph SGN_5th_V;            // Contiene la señal de quinto armonico simulada
SIGNAL_3Ph SGN_7th_V;            // Contiene la señal de septimo armonico simulada
SIGNAL_3Ph SGN_11th_V;           // Contiene la señal de onceavo armonico simulada
float k_nsc = 0.05;              // Porcentaje de la amplitud principal que representa la secuencia negativa
float k_5th = 0.02;              // Porcentaje de la amplitud principal que representa el quinto armonico
float k_7th = 0.02;              // Porcentaje de la amplitud principal que representa el septimo armonico
float k_11th = 0.0;              // Porcentaje de la amplitud principal que representa el onceavo armonico

// ********** Variables para el funcionamiento general
Trifasica Conv; // Contiene los valores de la trifasica medida en tension y corriente

// ********** Variables para el funcionamiento del EPLL
FE FE_U1;                                // Estimador de frecuencia
SEQ_CORE PSC_U1;                         // Core de componente fundamental, sec. positiva
SEQ_CORE NSC_U1;                         // Core de componente fundamental, sec. negativa
SEQ_CORE SC_U5;                          // Core de componente de quinto armonico
SEQ_CORE SC_U7;                          // Core de componente de septimo armonico
float U1N_COMP_ENABLE = 1.0;             // Habilita la compensación de secuencia fundamental negativa
float U5_COMP_ENABLE = 1.0;              // Habilita la compensación de quinto armonico
float U7_COMP_ENABLE = 1.0;              // Habilita la compensación de septimo armonico
float VPLL[3] = {0, 0, 0};               // Tensión de cada fase calculada por el PLL
GRID_VALUES Grid;                        // Estructura para ver los valores de interes de la trifasica medida
INTEGRATOR INT_CC_A, INT_CC_B, INT_CC_C; // Integradores utilizados para quitar la componente de continua del EPLL
float EPLL_u0 = 0;                       // Variable de ajuste para la integracion de la componenete de continua
float EPLL_u1 = 200;                     // Variable de ajuste para los cores - 200
float EPLL_u2 = 12000;                   // Ganancia del estimulador, una constante que se ajusta para el funcionamiento del estimador de frecuencia, originalmente 20
float EPLL_u3 = 200;                     // Variable de ajuste para los cores - 200

ANG_DIFF theta_error;          // Variable utilizada para representar el error de fase
RATE_LIMITER rampa;            // Rampa digital para probar la respuesta al escalón en frecuencia
float pendiente_chilena = 200; // Maxima velocidad de variacion de la frecuencia de red, en un dt de 100 us segun el estandar chileno

// SEQ_CORE PSC_I1;

/**
 * @brief Inicia los sistemas necesarios para la simulacion de las señales de entrada
 *
 */
void sim_init()
{
    // Senales simuladas
    SIGNAL_3Ph_init(&SGN_3Ph_V, amplitud_v);
    SIGNAL_3Ph_init(&SGN_3Ph_I, amplitud_i);
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

/*
interrupt void EPWM4_INT_ISR(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; // Must acknowledge the PIE group
    EPwm4Regs.ETCLR.bit.INT = 1;
    secuencia_comun();
}
*/
/**
 * @brief Es la funcion que se ejecuta cuando se genera una interrupcion por EPWM4
 *
 * @return interrupt
 * 
 * El nombre real es interrupt void EPWM4_INT_ISR(void)
 */
interrupt void EPWM4_INT_ISR (void)// PIE3.4 @ 0x000D66  EPWM4_INT (EPWM4)
{
    static int inicio = 1;
    GpioDataRegs.GPASET.bit.GPIO18 = 1; // Pin testigo de que se esta ejectuando la ISR

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; // Must acknowledge the PIE group

    EPwm4Regs.ETCLR.bit.INT = 1;

    if (inicio)
    {
        inicio = 0;
#if SIM_ADQ == TRUE
        sim_init();
#endif

#if BUILD >= BUILD_1
        // Cores
        SEQ_CORE_init(&PSC_U1, 1, 1, EPLL_u1, EPLL_u3, TWO_PI * F_NOM, time_step);
        SEQ_CORE_init(&NSC_U1, 1, -1, EPLL_u1, EPLL_u3, TWO_PI * F_NOM, time_step);
        // SEQ_CORE_init(&PSC_I1, 1, 1, EPLL_u1, EPLL_u3, TWO_PI * F_NOM, time_step);
        SEQ_CORE_init(&SC_U5, 5, -1, EPLL_u1, EPLL_u3, TWO_PI * F_NOM, time_step);
        SEQ_CORE_init(&SC_U7, 7, 1, EPLL_u1, EPLL_u3, TWO_PI * F_NOM, time_step);

        // Integradores para la eliminacion de offset
        INTEGRATOR_init(&INT_CC_A, time_step);
        INTEGRATOR_init(&INT_CC_B, time_step);
        INTEGRATOR_init(&INT_CC_C, time_step);

        // Estimador de frecuencia
        FE_init(&FE_U1, EPLL_u2, TWO_PI * F_NOM, time_step);

        // Estructura que contiene los datos de la red electrica
        GRID_VALUES_init(&Grid);

        // Estructura para mostrar la fase
        ANG_DIFF_init(&theta_error);

        // Rampa en frecuencia
        RATE_LIMITER_init(&rampa, time_step);
        rampa.max_rate = pendiente_chilena;
        rampa.min_rate = -pendiente_chilena;

#endif
    }

// ************************************************************ Adquisicion de datos ************************************************************
#if BUILD >= BUILD_0
    // ****************************** Adquisicon analogica de datos

    int mrv = (AdcRegs.ADCRESULT13 >> 4);
    int mtv = (AdcRegs.ADCRESULT14 >> 4);
    int msv = (AdcRegs.ADCRESULT15 >> 4);

    float Wv = ((AdcRegs.ADCRESULT1 >> 4) - OFFSET_WV) * KWV;
    float Uv = ((AdcRegs.ADCRESULT1 >> 4) - OFFSET_UV) * KUV;
    float Vv = ((AdcRegs.ADCRESULT3 >> 4) - OFFSET_VV) * KVV;
    float Rv = (mrv - OFFSET_RV) * KRV;
    float Tv = (mtv - OFFSET_TV) * KTV;
    float Sv = (msv - OFFSET_SV) * KSV;

    Conv.Va = Rv;
    Conv.Vb = Sv;
    Conv.Vc = Tv;

// --- Aqui habria una adquisicion analogica, si la tuviera... ---

// ****************************** Adquisición simulada de datos
#if SIM_ADQ == true

    // Rampa de frecuencia
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
    SIGNAL_3Ph_MACRO(SGN_NSC_V)
        SIGNAL_3Ph_MACRO(SGN_5th_V);
    SIGNAL_3Ph_MACRO(SGN_7th_V);
    SIGNAL_3Ph_MACRO(SGN_11th_V);
    Conv.Va = SGN_3Ph_V.a + SGN_NSC_V.a + SGN_5th_V.a + SGN_7th_V.a + SGN_11th_V.a; // Modificacion de valores medidos por ADC, se reemplazan
    Conv.Vb = SGN_3Ph_V.b + SGN_NSC_V.b + SGN_5th_V.b + SGN_7th_V.b + SGN_11th_V.b; // por los generador por el SGN_3Ph_V
    Conv.Vc = SGN_3Ph_V.c + SGN_NSC_V.c + SGN_5th_V.c + SGN_7th_V.c + SGN_11th_V.c;

    // Corriente
    SGN_3Ph_I.theta = AUX_VCO.out + dtheta_di; // dtheta_di: variable auxiliar para escalon de fase en corriente
    SIGNAL_3Ph_MACRO(SGN_3Ph_I);
    Conv.Ia = SGN_3Ph_I.a;
    Conv.Ib = SGN_3Ph_I.b;
    Conv.Ic = SGN_3Ph_I.c;

    // EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - (Conv.Va + offet_Va) / 3.3); // Mostramos una fase de la trifasica simulada en el pin 0
#endif
#endif

// ************************************************************ Aplicacion del PLL ************************************************************
#if BUILD >= BUILD_1
    // ****************************** Se utiliza el EPLL (Enhanced PLL 2) basado en cores
    // *** Estimacion de frecuencia
    FE_U1.in = PSC_U1.fe_out; // Se da como entrada la frecuencia calculada en la secuencia positiva pasada (salida del propio PSC_CORE)
    FE_MACRO(FE_U1);          // Se corre el estimador de frecuencia
    PSC_U1.wo = FE_U1.out;    // Se actualiza la frecuencia (velocidad angular)

    // *** Calculo de tension del PLL con los valores pasados de las distintas secuencias
    VPLL[0] = PSC_U1.out[0] + U1N_COMP_ENABLE * NSC_U1.out[0] + U5_COMP_ENABLE * SC_U5.out[0] + U7_COMP_ENABLE * SC_U7.out[0];
    VPLL[1] = PSC_U1.out[1] + U1N_COMP_ENABLE * NSC_U1.out[1] + U5_COMP_ENABLE * SC_U5.out[1] + U7_COMP_ENABLE * SC_U7.out[1];
    VPLL[2] = PSC_U1.out[2] + U1N_COMP_ENABLE * NSC_U1.out[2] + U5_COMP_ENABLE * SC_U5.out[2] + U7_COMP_ENABLE * SC_U7.out[2];

    // *** Realimentamos los nucleos con la entrada - el valor calculado y la componente de continua
    PSC_U1.e[0] = Conv.Va - VPLL[0] - INT_CC_A.out;
    PSC_U1.e[1] = Conv.Vb - VPLL[1] - INT_CC_B.out;
    PSC_U1.e[2] = Conv.Vc - VPLL[2] - INT_CC_C.out;
    SEQ_CORE_MACRO(PSC_U1); // Ejecucion de macro de comp. fundamental, sec. positiva

    NSC_U1.wo = FE_U1.out;
    NSC_U1.e[0] = PSC_U1.e[0];
    NSC_U1.e[1] = PSC_U1.e[1];
    NSC_U1.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(NSC_U1); // Ejecucion de macro de comp. fundamental, sec. negativa

    SC_U5.wo = FE_U1.out;
    SC_U5.e[0] = PSC_U1.e[0];
    SC_U5.e[1] = PSC_U1.e[1];
    SC_U5.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(SC_U5); // Ejecucion de macro de quinto arm.

    SC_U7.wo = FE_U1.out;
    SC_U7.e[0] = PSC_U1.e[0];
    SC_U7.e[1] = PSC_U1.e[1];
    SC_U7.e[2] = PSC_U1.e[2];
    SEQ_CORE_MACRO(SC_U7); // Ejecucion de macro de septimo arm.

    INT_CC_A.in = EPLL_u0 * PSC_U1.e[0];
    INT_CC_B.in = EPLL_u0 * PSC_U1.e[1];
    INT_CC_C.in = EPLL_u0 * PSC_U1.e[2];
    INTEGRATOR_MACRO(INT_CC_A); // Ejecucion de macro de integrador para elim. de offset, fase A.
    INTEGRATOR_MACRO(INT_CC_B); // Ejecucion de macro de integrador para elim. de offset, fase B.
    INTEGRATOR_MACRO(INT_CC_C); // Ejecucion de macro de integrador para elim. de offset, fase C.

    Grid.U = ISQRT2 * PSC_U1.Magn.out * 1; //  Conv.Vrng; // Actualizacion de valores medidos de la red electrica.
    Grid.f = FE_U1.out / TWO_PI;
    Grid.theta = PSC_U1.CORE_VCO.out;

    float miU = Grid.U;
    float miF = Grid.f;
    float miT = Grid.theta;

    imprimir_vector(Grid.U, Grid.theta);

#if BUILD_1_SHOW_VARIABLES == 1

    // *** Representacion de las variables
    theta_error.a = SGN_3Ph_V.theta;
    theta_error.b = Grid.theta;
    ANG_DIFF_MACRO(theta_error);

    if (show_phase_error)
    {
        EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - ((theta_error.out) / ONE_SIXTH_PI + 1.5) / 3.3); // Mostramos el error en la fase (ESCALA: 1v=30 GRADOS)
    }
    else if (show_phase)
    {

        EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - (SGN_3Ph_V.theta) / TWO_PI);
        EPwm1Regs.CMPB = PROJECT_PERIOD * (1 - (Grid.theta) / TWO_PI);
    }
    else if (show_freq)
    {
        // Frecuencia digital
        EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - (F_NOM + rampita / TWO_PI - 49.75) * 2 / 3.3); // Exterioriza la variable
        // EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - (F_NOM + rampa.out / TWO_PI - 49.75) * 2 / 3.3); // Exterioriza la variable
        // Frecuencia estimada por el PLL
        EPwm1Regs.CMPB = PROJECT_PERIOD * (1 - (Grid.f - 49.75) * 2 / 3.3); // Exterioriza la variable (49.5Hz = 0.5V, 50 Hz = 1.5V) (ESCALA: 1Hz=2V)
    }
    else if (show_phase_a)
    {
        // Fase A completa
        EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - (Conv.Va + 1.5) / 3.3);

        // Secuencia positiva de la fase A
        EPwm1Regs.CMPB = PROJECT_PERIOD * (1 - (SGN_3Ph_V.a + 1.5) / 3.3);

        // Secuencia negativa de la fase A
        // EPwm1Regs.CMPB = PROJECT_PERIOD * (1 - (SGN_NSC_V.a + 1.5) / 3.3);

        // Quinto armonico, secuencia negativa de la fase A
        // EPwm1Regs.CMPB = PROJECT_PERIOD * (1 - (SGN_5th_V.a + 1.5) / 3.3);
    }

#endif

    GpioDataRegs.GPACLEAR.bit.GPIO18 = 1; // Pin testigo de que se abandona la ISR
}


#endif
