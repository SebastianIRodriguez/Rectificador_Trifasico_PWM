#include "Aplicacion.h"
#include "Renolib.h"
#include "LINX.h"

// Esto lo agrego yo:
#define EPLL 1
#define SRF 2
#define PLL_TYPE EPLL
#define ENABLE_PLL_NOTCH 1

// Referencias externas a SRF - PLL
extern VCO PLL_VCO;
extern PI_CONTROLLER PLL_PI;
extern PARK PLL_Park_V, PLL_Park_I;
extern CLARKE PLL_Clarke_V, PLL_Clarke_I;
extern SIGNAL_3Ph SGN_3Ph_V;
extern NOTCH Valpha_FILT, Vbeta_FILT, Ialpha_FILT, Ibeta_FILT;
extern float PLL_NOTCH_c1, PLL_NOTCH_c2;
extern unsigned int ENABLE_PLL_NOTCH;
extern LIMITS PLL_Vd_Lim;
extern unsigned int PLL_Norm;
extern float dtheta;
extern float U0, I0;
extern GRID_VALUES Grid;
// Referencias externas a 3EPLL-2
extern SEQ_CORE PSC_U1, NSC_U1, SC_U5, SC_U7, PSC_I1;
extern float U1N_COMP_ENABLE, U5_COMP_ENABLE, U7_COMP_ENABLE;
extern float VPLL[];
extern INTEGRATOR INT_CC_A, INT_CC_B, INT_CC_C;
extern FE FE_U1;

interrupt void EPWM4_INT_ISR(void)				// PIE3.4 @ 0x000D66  EPWM4_INT (EPWM4)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;     // Must acknowledge the PIE group
	EPwm4Regs.ETCLR.bit.INT = 1;
    time = time + h;
    //*********************************************************************************************************//
    // *** SE TOMAN LAS LECTURAS ANALÓGICAS
    unsigned int n = ADC_B7_Ptr - &ADC_B7[0];
    ADC_A0_Ptr = &ADC_A0[0];                                // Reinicio posicion de puntero a posicion inicial
    ADC_A1_Ptr = &ADC_A1[0];
    ADC_A2_Ptr = &ADC_A2[0];
    ADC_A3_Ptr = &ADC_A3[0];
    ADC_A4_Ptr = &ADC_A4[0];
    ADC_A5_Ptr = &ADC_A5[0];
    ADC_A6_Ptr = &ADC_A6[0];
    ADC_A7_Ptr = &ADC_A7[0];
    ADC_B0_Ptr = &ADC_B0[0];
    ADC_B1_Ptr = &ADC_B1[0];
    ADC_B2_Ptr = &ADC_B2[0];
    ADC_B3_Ptr = &ADC_B3[0];
    ADC_B4_Ptr = &ADC_B4[0];
    ADC_B5_Ptr = &ADC_B5[0];
    ADC_B6_Ptr = &ADC_B6[0];
    ADC_B7_Ptr = &ADC_B7[0];

    // *** SE TRANSFORMAN LAS LECTRUAS ANALÓGICAS EN VALORES REALES
    Conv.Va = (-((BuffAvg(ADC_B0_Ptr,n))-ADC_HALF_COUNTS-B0_OFFSET)/ADC_FS_COUNTS);
    Conv.Vb = (-((BuffAvg(ADC_B1_Ptr,n))-ADC_HALF_COUNTS-B1_OFFSET)/ADC_FS_COUNTS);
    Conv.Vc = (-((BuffAvg(ADC_B2_Ptr,n))-ADC_HALF_COUNTS-B2_OFFSET)/ADC_FS_COUNTS);
    Conv.Vbus = ((BuffAvg(ADC_B3_Ptr,n))/ADC_FS_COUNTS);
    Conv.Vaux = (((BuffAvg(ADC_A3_Ptr,n))-ADC_HALF_COUNTS)/ADC_FS_COUNTS);
    Conv.Ia = (((BuffAvg(ADC_A2_Ptr,n))-ADC_HALF_COUNTS)/ADC_FS_COUNTS);
    Conv.Ib = (((BuffAvg(ADC_A1_Ptr,n))-ADC_HALF_COUNTS)/ADC_FS_COUNTS);
    Conv.Ic = (((BuffAvg(ADC_A0_Ptr,n))-ADC_HALF_COUNTS)/ADC_FS_COUNTS);

    // *** NO SE QUE ES ESTO
    //*********************************************************************************************************//
    UPDATE_GPI_MACRO(IO);                                       // Actualizacion de estado de entradas digitales
    UPDATE_GPO_MACRO(IO);                                       // Actualizacion de estado de salidas digitales

// *******************************************************************************************************************************************************************//
    #if BUILD >= BUILD_01
        Vbus = Conv.Vbus*Conv.Vrng;
        U0 = 0.33333*(Conv.Va + Conv.Vb + Conv.Vc); // Calculo de secuencia cero en medicion de tension
        I0 = 0.33333*(Conv.Ia + Conv.Ib + Conv.Ic); // Calculo de secuencia cero en medicion de corriente
        Conv.Va = Conv.Va - U0;                     // Remocion de secuencia cero en tensiones de fase
        Conv.Vb = Conv.Vb - U0;
        Conv.Vc = Conv.Vc - U0;
        Conv.Ia = Conv.Ia - I0;                     // Remocion de secuencia cero en corrientes de fase
        Conv.Ib = Conv.Ib - I0;
        Conv.Ic = Conv.Ic - I0;
        #if SIM_MEAS == TRUE                                            // Seleccion de medicion de tension de red electrica
            AUX_VCO.in = dw_aux;                                        // dw_aux: variable auxiliar para escalon de frecuencia
            VCO_MACRO(AUX_VCO);                                         // AUX_VCO.K: variable auxiliar para escalon de amplitud
            SGN_3Ph_V.theta = AUX_VCO.out + dtheta_aux;                 // dtheta_aux: variable auxiliar para escalon de fase
            SIGNAL_3Ph_MACRO(SGN_3Ph_V);
            Conv.Va = SGN_3Ph_V.a;                                      // Modificacion de valores medidos por ADC, se reemplazan
            Conv.Vb = SGN_3Ph_V.b;                                      // por los generador por el SGN_3Ph_V
            Conv.Vc = SGN_3Ph_V.c;
            SGN_3Ph_I.theta = AUX_VCO.out + dtheta_di;                  // dtheta_aux: variable auxiliar para escalon de fase
            SIGNAL_3Ph_MACRO(SGN_3Ph_I);
            Conv.Ia = SGN_3Ph_I.a;                                      //
            Conv.Ib = SGN_3Ph_I.b;                                      //
            Conv.Ic = SGN_3Ph_I.c;
        #endif
    #endif
            //*********************************************************************************************************//
    #if BUILD >= BUILD_02
        #if (PLL_TYPE == EPLL)                                              // Seleccion de PLL -> Enhanced PLL 2
            FE_U1.in = PSC_U1.fe_out;                                       // Estimador de frecuencia
            FE_MACRO(FE_U1);
            PSC_U1.wo = FE_U1.out;
            VPLL[0] =  PSC_U1.out[0] + U1N_COMP_ENABLE*NSC_U1.out[0] + U5_COMP_ENABLE*SC_U5.out[0] + U7_COMP_ENABLE*SC_U7.out[0];
            VPLL[1] = PSC_U1.out[1] + U1N_COMP_ENABLE*NSC_U1.out[1] + U5_COMP_ENABLE*SC_U5.out[1] + U7_COMP_ENABLE*SC_U7.out[1];
            VPLL[2] = PSC_U1.out[2] + U1N_COMP_ENABLE*NSC_U1.out[2] + U5_COMP_ENABLE*SC_U5.out[2] + U7_COMP_ENABLE*SC_U7.out[2];
            PSC_U1.e[0] = Conv.Va - VPLL[0] - INT_CC_A.out;
            PSC_U1.e[1] = Conv.Vb - VPLL[1] - INT_CC_B.out;
            PSC_U1.e[2] = Conv.Vc - VPLL[2] - INT_CC_C.out;
            SEQ_CORE_MACRO(PSC_U1);                                         // Ejecucion de macro de comp. fundamental, sec. positiva
            NSC_U1.wo = FE_U1.out;
            NSC_U1.e[0] = PSC_U1.e[0];
            NSC_U1.e[1] = PSC_U1.e[1];
            NSC_U1.e[2] = PSC_U1.e[2];
            SEQ_CORE_MACRO(NSC_U1);                                         // Ejecucion de macro de comp. fundamental, sec. negativa
            SC_U5.wo = FE_U1.out;
            SC_U5.e[0] = PSC_U1.e[0];
            SC_U5.e[1] = PSC_U1.e[1];
            SC_U5.e[2] = PSC_U1.e[2];
            SEQ_CORE_MACRO(SC_U5);                                          // Ejecucion de macro de quinto arm.
            SC_U7.wo = FE_U1.out;
            SC_U7.e[0] = PSC_U1.e[0];
            SC_U7.e[1] = PSC_U1.e[1];
            SC_U7.e[2] = PSC_U1.e[2];
            SEQ_CORE_MACRO(SC_U7);                                          // Ejecucion de macro de septimo arm.
            INT_CC_A.in = EPLL_u0*PSC_U1.e[0];
            INT_CC_B.in = EPLL_u0*PSC_U1.e[1];
            INT_CC_C.in = EPLL_u0*PSC_U1.e[2];
            INTEGRATOR_MACRO(INT_CC_A);                                     // Ejecucion de macro de integrador para elim. de offset, fase A.
            INTEGRATOR_MACRO(INT_CC_B);                                     // Ejecucion de macro de integrador para elim. de offset, fase B.
            INTEGRATOR_MACRO(INT_CC_C);                                     // Ejecucion de macro de integrador para elim. de offset, fase C.
            Grid.U = ISQRT2*PSC_U1.Magn.out*Conv.Vrng;                      // Actualizacion de valores medidos de la red electrica.
            Grid.f = FE_U1.out/TWO_PI;
            Grid.theta = PSC_U1.CORE_VCO.out;
        #elif (PLL_TYPE == SRF)
            // ***** Aplica un filtro de Notch a la tensión alpha y beta
            // Realiza la transformada de Clarke
            PLL_Clarke_V.a = Conv.Va;
            PLL_Clarke_V.b = Conv.Vb;
            PLL_Clarke_V.c = Conv.Vc;
            CLARKE_MACRO(PLL_Clarke_V); 

            // Aplica el filtro a la tensión Alpha
            Valpha_FILT.in[0] = PLL_Clarke_V.alpha;
            NOTCH_MACRO(Valpha_FILT);

            // Aplica el filtro a la tensión Beta
            Vbeta_FILT.in[0] = PLL_Clarke_V.beta;
            NOTCH_MACRO(Vbeta_FILT);

            // Realiza la transformada de Park
            PLL_Park_V.alpha = PLL_Clarke_V.alpha - ENABLE_PLL_NOTCH*Valpha_FILT.out[0];
            PLL_Park_V.beta = PLL_Clarke_V.beta - ENABLE_PLL_NOTCH*Vbeta_FILT.out[0];
            PLL_Park_V.zero = PLL_Clarke_V.zero;
            PLL_Park_V.sin = sin(PLL_VCO.out);
            PLL_Park_V.cos = cos(PLL_VCO.out);
            PARK_MACRO(PLL_Park_V);

            // Ingresa los valores p y q en el PI
            if (PLL_Norm == FALSE) 
                PLL_PI.sp = PLL_Park_V.q;
            else {
                PLL_Vd_Lim.in = PLL_Park_V.d;
                LIMITS_MACRO(PLL_Vd_Lim);
                PLL_PI.sp = PLL_Park_V.q/PLL_Vd_Lim.out;
            }
            PI_CONTROLLER_MACRO(PLL_PI);

            // Se calcula el ángulo del PLL
            PLL_VCO.in = PLL_PI.out;
            VCO_MACRO(PLL_VCO);
            Grid.U = ISQRT2*PLL_Park_V.d*Conv.Vrng;
            Grid.f = (PLL_VCO.in + PLL_VCO.wn)/TWO_PI;
            Grid.theta = PLL_VCO.out;
        #endif
    #endif
// *******************************************************************************************************************************************************************//
    #if BUILD >= BUILD_04           // Conversion de corriente medida en dominio abc a dq

        PLL_Clarke_I.a = Conv.Ia;
        PLL_Clarke_I.b = Conv.Ib;
        PLL_Clarke_I.c = Conv.Ic;
        CLARKE_MACRO(PLL_Clarke_I); // Transformamos de abc -> alpha, beta, zero

        PLL_Park_I.alpha = PLL_Clarke_I.alpha;
        PLL_Park_I.beta = PLL_Clarke_I.beta;
        PLL_Park_I.zero = PLL_Clarke_I.zero;

        // Depende del PLL utilizado, el seno de la transformada de Park se obtiene de distintas fuentes
        #if (PLL_TYPE == EPLL)
            PLL_Park_I.sin = PSC_U1.C.sin;
            PLL_Park_I.cos = PSC_U1.C.cos;
        #elif (PLL_TYPE == SRF)
            PLL_Park_I.sin = sin(PLL_VCO.out);
            PLL_Park_I.cos = cos(PLL_VCO.out);
        #endif
        PARK_MACRO(PLL_Park_I); // Transformamos de alpha, beta, zero -> d, q, zero

        // grid.i
        I = Conv.Irng*ISQRT2*sqrt(PLL_Park_I.d*PLL_Park_I.d + PLL_Park_I.q*PLL_Park_I.q);
    #endif
}


//--- end of file -----------------------------------------------------

