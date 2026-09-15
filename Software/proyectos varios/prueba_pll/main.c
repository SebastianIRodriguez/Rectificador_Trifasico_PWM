#include "Aplicacion.h"     // Header de programa principal
#include "Renolib.h"		// Libreria de Renovar
#include "LINX.h"			// Libreria de Lin-X
#include "math.h"

// ------------- Interfaces entrada/salida del controlador Lin-X ------------- //
ANALOG_MEASURES Measures;
CONVERTER Conv;
GPIO IO;
// -------------------- Buffers para conversion A/D -------------------- //
unsigned int ADC_A0[ADC_BUF_LEN], ADC_A1[ADC_BUF_LEN], ADC_A2[ADC_BUF_LEN], ADC_A3[ADC_BUF_LEN];
unsigned int ADC_A4[ADC_BUF_LEN], ADC_A5[ADC_BUF_LEN], ADC_A6[ADC_BUF_LEN], ADC_A7[ADC_BUF_LEN];
unsigned int ADC_B0[ADC_BUF_LEN], ADC_B1[ADC_BUF_LEN], ADC_B2[ADC_BUF_LEN], ADC_B3[ADC_BUF_LEN];
unsigned int ADC_B4[ADC_BUF_LEN], ADC_B5[ADC_BUF_LEN], ADC_B6[ADC_BUF_LEN], ADC_B7[ADC_BUF_LEN];
unsigned int *ADC_A0_Ptr = ADC_A0, *ADC_A1_Ptr = ADC_A1, *ADC_A2_Ptr = ADC_A2, *ADC_A3_Ptr = ADC_A3;
unsigned int *ADC_A4_Ptr = ADC_A4, *ADC_A5_Ptr = ADC_A5, *ADC_A6_Ptr = ADC_A6, *ADC_A7_Ptr = ADC_A7;
unsigned int *ADC_B0_Ptr = ADC_B0, *ADC_B1_Ptr = ADC_B1, *ADC_B2_Ptr = ADC_B2, *ADC_B3_Ptr = ADC_B3;
unsigned int *ADC_B4_Ptr = ADC_B4, *ADC_B5_Ptr = ADC_B5, *ADC_B6_Ptr = ADC_B6, *ADC_B7_Ptr = ADC_B7;
// -------------------- Scope para vista en CCS -------------------- //
float Scope_1[SCOPE_BUF_LEN], Scope_2[SCOPE_BUF_LEN], Scope_3[SCOPE_BUF_LEN], Scope_4[SCOPE_BUF_LEN];
float *Scope_1_Ptr = Scope_1, *Scope_2_Ptr = Scope_2; *Scope_3_Ptr = Scope_3, *Scope_4_Ptr = Scope_4;

const float h = 1/(FSW*1000.0);
double time = 0;



float U0 = 0, I0 = 0;



// ------------------------ Bloques para simulacionS ------------------------ //
SIGNAL_3Ph SGN_3Ph_V, SGN_3Ph_I;
VCO AUX_VCO;
float dtheta = 0.0;
float dtheta_aux = 0.0;
float dtheta_di = 0.0;
float dw_aux = 0.0;


// ------------------------ SRF-PLL Blocks ------------------------ //
VCO PLL_VCO;
PI_CONTROLLER PLL_PI;
PARK PLL_Park_V, PLL_Park_I;
CLARKE PLL_Clarke_V, PLL_Clarke_I;
LIMITS PLL_Vd_Lim;

// ------------------------ 3EPLL-2 Blocks ------------------------ //
SEQ_CORE PSC_U1, NSC_U1, SC_U5, SC_U7, PSC_I1;
float U1N_COMP_ENABLE = 1.0, U5_COMP_ENABLE = 1.0, U7_COMP_ENABLE = 1.0;
float VPLL[3] = {0,0,0};
INTEGRATOR INT_CC_A, INT_CC_B, INT_CC_C;
FE FE_U1;

// -------------------------- VOC Blocks -------------------------- //
SVPWM SVM;
VCO VOC_VCO;
PARK VOC_Park;
CLARKE VOC_Clarke;
SIGNAL_3Ph SGN_Conv;
PI_CONTROLLER Id_PI, Iq_PI, P_PI, Q_PI, VDC_PI;
IPQ_MEAS IPQ_Meas;
PT1 P_FILT, Q_FILT;
LIMITS Id_Lim, Iq_Lim;
float Vdref = 0.0, Vqref = 0.0;
float Idref = 0.01, Iqref = 0.0;
float Pref = 10.0, Qref = 0.0, Vdcref = 64/CONV_VRNG, Sbase = 1.5*(CONV_VRNG*CONV_IRNG);
// ----------------------- Auxiliary Blocks ----------------------- //
OVER_PROT OV_Bus;
GRID_VALUES Grid;

float Vbus = 0.0;
float I = 0.0;
ANG_DIFF theta_Diff;

float timer = TIMER_IDLE_ST, ISR_Cycle = 0.0;
int DAC_Select[2] = {0, 0};



// ------------------ Convertidores digitales - analogicos  ------------------ //
PWM_DAC DAC_1;
PWM_DAC DAC_2;
PWM_DAC DAC_3;
PWM_DAC DAC_4;

void main(void) {

    // --------- INICIALIZACION DE SISTEMA --------- //

    InitSysCtrl();                      // Inicializacion de CPU (Archivo: System_Control.c)
    InitGpio();                         // Inicializacion de pines E/S (Archivo: GPIO.c)
    InitPieCtrl();                      // Inicializacion y habilitacion PIE (Archivo: PIE_Control.c)
    InitWatchdog();                     // Inicializar watchdog (Archivo: WatchDog.c)
    InitAdc();                          // ADC (Archivo: ADC.c)
    InitTimer();
    InitEPwm();                         // PWM (Archivo: EPWM.c)
    InitBootstrap(5000);
    BlockConverter();                   // Deshabilita salidas PWM, se�ales en estado bajo

    // --------- INICIALIZACION DE BLOQUES --------- //
    // SRF - PLL:
    VCO_init(&PLL_VCO, TWO_PI*Fnom, h);
    PI_CONTROLLER_init(&PLL_PI, PLL_PI_Kp, PLL_PI_Ki, h);
    PARK_init(&PLL_Park_V);
    PARK_init(&PLL_Park_I);
    CLARKE_init(&PLL_Clarke_V);
    CLARKE_init(&PLL_Clarke_I);
    SIGNAL_3Ph_init(&SGN_3Ph_V, 0.25);
    SIGNAL_3Ph_init(&SGN_3Ph_I, 0.10);
    LIMITS_init(&PLL_Vd_Lim, 1.00, 0.01);
    GRID_VALUES_init(&Grid);
    // 3EPLL-2
    SEQ_CORE_init(&PSC_U1, 1, 1 ,EPLL_u1, EPLL_u3, TWO_PI*Fnom, h);
    SEQ_CORE_init(&NSC_U1, 1, -1 ,EPLL_u1, EPLL_u3, TWO_PI*Fnom, h);
    SEQ_CORE_init(&PSC_I1, 1, 1, EPLL_u1, EPLL_u3, TWO_PI*Fnom, h);
    SEQ_CORE_init(&SC_U5, 5, -1, EPLL_u1, EPLL_u3, TWO_PI*Fnom, h);
    SEQ_CORE_init(&SC_U7, 7, 1, EPLL_u1, EPLL_u3, TWO_PI*Fnom, h);
    INTEGRATOR_init(&INT_CC_A, h);
    INTEGRATOR_init(&INT_CC_B, h);
    INTEGRATOR_init(&INT_CC_C, h);
    FE_init(&FE_U1, EPLL_u2, TWO_PI*Fnom, h);

    asm(" CLRC INTM, DBGM");            // Habilitacion de interrupciones globales
    while(1){
}

