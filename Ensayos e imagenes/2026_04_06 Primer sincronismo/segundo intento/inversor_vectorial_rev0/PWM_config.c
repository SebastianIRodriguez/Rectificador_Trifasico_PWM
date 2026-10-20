/**********************************************************************
 * File: EPwm_7_8_9_10_12.c -- Solution File for Lab 7, 8, 9, 10 and 12
 * Devices: TMS320F2833x
 * Author: Technical Training Organization (TTO), Texas Instruments
 * History:
 *   07/15/08 - original
 **********************************************************************/

#include "project.h"
#include "PWM_config.h"

// TBCTL (Time-Base Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// TBCTR MODE bits
#define TB_COUNT_UP 0x0
#define TB_COUNT_DOWN 0x1
#define TB_COUNT_UPDOWN 0x2
#define TB_FREEZE 0x3
// PHSEN bit
#define TB_DISABLE 0x0
#define TB_ENABLE 0x1
// PRDLD bit
#define TB_SHADOW 0x0
#define TB_IMMEDIATE 0x1
// SYNCOSEL bits
#define TB_SYNC_IN 0x0
#define TB_CTR_ZERO 0x1
#define TB_CTR_CMPB 0x2
#define TB_SYNC_DISABLE 0x3
// HSPCLKDIV and CLKDIV bits
#define TB_DIV1 0x0
#define TB_DIV2 0x1
#define TB_DIV4 0x2
// PHSDIR bit
#define TB_DOWN 0x0
#define TB_UP 0x1

// CMPCTL (Compare Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// LOADAMODE and LOADBMODE bits
#define CC_CTR_ZERO 0x0
#define CC_CTR_PRD 0x1
#define CC_CTR_ZERO_PRD 0x2
#define CC_LD_DISABLE 0x3
// SHDWAMODE and SHDWBMODE bits
#define CC_SHADOW 0x0
#define CC_IMMEDIATE 0x1

// AQCTLA and AQCTLB (Action-qualifier Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define AQ_NO_ACTION 0x0
#define AQ_CLEAR 0x1
#define AQ_SET 0x2
#define AQ_TOGGLE 0x3

// DBCTL (Dead-Band Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// MODE bits
#define DB_DISABLE 0x0
#define DBA_ENABLE 0x1
#define DBB_ENABLE 0x2
#define DB_FULL_ENABLE 0x3
// POLSEL bits
#define DB_ACTV_HI 0x0
#define DB_ACTV_LOC 0x1
#define DB_ACTV_HIC 0x2
#define DB_ACTV_LO 0x3

// PCCTL (chopper control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// CHPEN bit
#define CHP_ENABLE 0x0
#define CHP_DISABLE 0x1
// CHPFREQ bits
#define CHP_DIV1 0x0
#define CHP_DIV2 0x1
#define CHP_DIV3 0x2

// TBCTL (Time-Base Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// TBCTR MODE bits
#define TB_COUNT_UP 0x0
#define TB_COUNT_DOWN 0x1
#define TB_COUNT_UPDOWN 0x2
#define TB_FREEZE 0x3
// PHSEN bit
#define TB_DISABLE 0x0
#define TB_ENABLE 0x1
// PRDLD bit
#define TB_SHADOW 0x0
#define TB_IMMEDIATE 0x1
// SYNCOSEL bits
#define TB_SYNC_IN 0x0
#define TB_CTR_ZERO 0x1
#define TB_CTR_CMPB 0x2
#define TB_SYNC_DISABLE 0x3
// HSPCLKDIV and CLKDIV bits
#define TB_DIV1 0x0
#define TB_DIV2 0x1
#define TB_DIV4 0x2
// PHSDIR bit
#define TB_DOWN 0x0
#define TB_UP 0x1

// CMPCTL (Compare Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// LOADAMODE and LOADBMODE bits
#define CC_CTR_ZERO 0x0
#define CC_CTR_PRD 0x1
#define CC_CTR_ZERO_PRD 0x2
#define CC_LD_DISABLE 0x3
// SHDWAMODE and SHDWBMODE bits
#define CC_SHADOW 0x0
#define CC_IMMEDIATE 0x1

// AQCTLA and AQCTLB (Action-qualifier Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define AQ_NO_ACTION 0x0
#define AQ_CLEAR 0x1
#define AQ_SET 0x2
#define AQ_TOGGLE 0x3

// DBCTL (Dead-Band Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// MODE bits
#define DB_DISABLE 0x0
#define DBA_ENABLE 0x1
#define DBB_ENABLE 0x2
#define DB_FULL_ENABLE 0x3
// POLSEL bits
#define DB_ACTV_HI 0x0
#define DB_ACTV_LOC 0x1
#define DB_ACTV_HIC 0x2
#define DB_ACTV_LO 0x3

// PCCTL (chopper control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// CHPEN bit
#define CHP_ENABLE 0x0
#define CHP_DISABLE 0x1
// CHPFREQ bits
#define CHP_DIV1 0x0
#define CHP_DIV2 0x1
#define CHP_DIV3 0x2

// ********** Declaracion de funciones locales
void enableEPWMClock();
void disableEPWMClock();
void pwm4_init();

/**
 * @brief Habilita la llegada del clock a los ePWM
 *
 */
void enableEPWMClock()
{
	asm(" EALLOW");						   // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // HSPCLK to ePWM modules enabled
	asm(" EDIS");						   // Disable EALLOW protected register access
}

/**
 * @brief Deshabilita la llegada del clock a los ePWM
 *
 */
void disableEPWMClock()
{
	asm(" EALLOW");						   // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0; // HSPCLK to ePWM modules enabled
	asm(" EDIS");						   // Disable EALLOW protected register access
}

/**
 * @brief Configura al EPWM1 para generar un pedido de interrupción cuando llega a 0
 *
 */
void pwm1_configure_interrupt()
{
	disableEPWMClock();
	EPwm1Regs.ETSEL.bit.INTEN = 1;	// habilita interrupcion
	EPwm1Regs.ETSEL.bit.INTSEL = 1; // trigger cuando el timer es cero
	EPwm1Regs.ETPS.bit.INTPRD = 1;	// Dispara al primer evento
	enableEPWMClock();
}

/**
 * @brief Habilita la interrupcion desde el ePWM1
 * Para que la interrupción se lance, debe existir un pedido de interrupcion desde dicho ePWM
 *
 */
void pwm1_enable_interrupt()
{
	//---------------------------------------------------------------------
	//--- Enable the clocks to the ePWM module.
	//--- Note: this should be done after all ePWM modules are configured
	//--- to ensure synchronization between the ePWM modules.
	//---------------------------------------------------------------------
	disableEPWMClock();
	asm(" EALLOW");						   // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // HSPCLK to ePWM modules enabled
	PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	   // Disable the PIE
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;	   // Enable EPWM1_INT in PIE group 3
	IER |= 0b0100;						   // Enable INT3 in IER to enable PIE group
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	   // Enable the PIE
	asm(" CLRC INTM");
	asm(" EDIS"); // Disable EALLOW protected register access
	enableEPWMClock();
}

/**
 * @brief Inicia los ePWM1, 2 y 3 y los deja al 40, 50 y 60% de su DC respectivamente
 * No habilita interrupcion en ningun PWM
 */
void InitEPwm123_static()
{
	static int PERIOD = 7500;		// Cuentas para lograr una frecuencia de 10 KHz, antes 7500
	static int SOC_HALF_TIME = 240; // Estimacion de cuentas para las conversiones analogicas
	disableEPWMClock();				// Necesario para que esten sincronizados

	// EPWM Module 4 config
	EPwm1Regs.TBPRD = PERIOD;					   // Period
	EPwm1Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;		   // Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // Sync down-stream module
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;			// Clock sin divisiones
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPwm4A
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm1Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm1Regs.DBRED = 50;						   // RED = 50 TBCLKs

	// EPWM Module 5 config
	EPwm2Regs.TBPRD = PERIOD;					   // Period
	EPwm2Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   // Slave module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPWM5A
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm2Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm2Regs.DBRED = 50;						   // RED = 50 TBCLKs

	// EPWM Module 6 config
	EPwm3Regs.TBPRD = PERIOD;					   // Period = 1600 TBCLK counts
	EPwm3Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   // Slave module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPWM6A
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm3Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm3Regs.DBRED = 50;						   // RED = 50 TBCLKs

	// Run Time (Note: Example execution of one run-time instant)
	//=========================================================
	EPwm1Regs.CMPB = PERIOD - SOC_HALF_TIME; // Para marcar el inicio de las conversiones analogicas
	EPwm1Regs.CMPA.half.CMPA = PERIOD * 0.6; // DC = 40%
	EPwm2Regs.CMPA.half.CMPA = PERIOD * 0.5; // DC = 50%
	EPwm3Regs.CMPA.half.CMPA = PERIOD * 0.4; // DC = 60%

	enableEPWMClock();
}

/**
 * @brief Inicia los ePWM1, 2 y 3 y los deja al 40, 50 y 60% de su DC respectivamente
 * Ademas, habilita la interrupcion y SOCA del ePWM1
 *
 */
void InitEPwm123_withInterrupt()
{
	static int PERIOD = PROJECT_PERIOD;		// Cuentas para lograr una frecuencia de 10 KHz, antes 7500
	static int SOC_HALF_TIME = 240; // Estimacion de cuentas para las conversiones analogicas
	disableEPWMClock();				// Necesario para que esten sincronizados

	// EPWM Module 4 config
	EPwm1Regs.TBPRD = PERIOD;					   // Period
	EPwm1Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;		   // Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // Sync down-stream module
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;			// Clock sin divisiones
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPwm4A
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm1Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm1Regs.DBRED = 50;						   // RED = 50 TBCLKs
	EPwm1Regs.TZSEL.bit.CBC1 = 1; // TZ1 as one-shot trip event source
	EPwm1Regs.TZCTL.bit.TZA = 2; // Force EPWMxA to a low state
	EPwm1Regs.TZCTL.bit.TZB = 2; // Force EPWMxB to a low state

	// EPWM Module 5 config
	EPwm2Regs.TBPRD = PERIOD;					   // Period
	EPwm2Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   // Slave module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPWM5A
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm2Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm2Regs.DBRED = 50;						   // RED = 50 TBCLKs
	EPwm2Regs.TZSEL.bit.CBC1 = 1; // TZ1 as one-shot trip event source
    EPwm2Regs.TZCTL.bit.TZA = 2; // Force EPWMxA to a low state
    EPwm2Regs.TZCTL.bit.TZB = 2; // Force EPWMxB to a low state

	// EPWM Module 6 config
	EPwm3Regs.TBPRD = PERIOD;					   // Period = 1600 TBCLK counts
	EPwm3Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   // Slave module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPWM6A
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm3Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm3Regs.DBRED = 50;						   // RED = 50 TBCLKs
	EPwm3Regs.TZSEL.bit.CBC1 = 1; // TZ1 as one-shot trip event source
    EPwm2Regs.TZCTL.bit.TZA = 2; // Force EPWMxA to a low state
    EPwm2Regs.TZCTL.bit.TZB = 2; // Force EPWMxB to a low state

	// Run Time (Note: Example execution of one run-time instant)
	//=========================================================
	EPwm1Regs.CMPB = PERIOD - SOC_HALF_TIME; // Para marcar el inicio de las conversiones analogicas
	EPwm1Regs.CMPA.half.CMPA = PERIOD * 0.6; // DC = 40%
	EPwm2Regs.CMPA.half.CMPA = PERIOD * 0.5; // DC = 50%
	EPwm3Regs.CMPA.half.CMPA = PERIOD * 0.4; // DC = 60%

	// ADC SOCA desde el ePWM1
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;	 // Genera el SOCA al primer evento
	EPwm1Regs.ETSEL.bit.SOCASEL = 6; // 110, SOCA on CMPB rising
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;	 // Habilita el SOCA

	// Configura la interrupcion desde el pwm1
	EPwm1Regs.ETSEL.bit.INTEN = 1;	// habilita interrupcion
	EPwm1Regs.ETSEL.bit.INTSEL = 1; // trigger cuando el timer es cero
	EPwm1Regs.ETPS.bit.INTPRD = 1;	// Dispara al primer evento

	// Habilita la interrupcion
	asm(" EALLOW");						   // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // HSPCLK to ePWM modules enabled
	PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	   // Disable the PIE
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;	   // Enable EPWM1_INT in PIE group 3
	IER |= 0b0100;						   // Enable INT3 in IER to enable PIE group
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	   // Enable the PIE
	asm(" CLRC INTM");
	asm(" EDIS"); // Disable EALLOW protected register access

	enableEPWMClock();
}

/**
 * @brief Inicia los ePWM1, 2 y 3 y los deja al 40, 50 y 60% de su DC respectivamente
 * Ademas, habilita la interrupcion y SOCA del ePWM1
 * Ademas, habilita al ePWM4 para poder reproducir una salida de debug
 *
 */
void InitEPwm123_withInterrupt_pwm4_debug()
{
	static int PERIOD = 7500;		// Cuentas para lograr una frecuencia de 10 KHz, antes 7500
	static int SOC_HALF_TIME = 240; // Estimacion de cuentas para las conversiones analogicas
	disableEPWMClock();				// Necesario para que esten sincronizados

	// EPWM Module 4 config
	EPwm1Regs.TBPRD = PERIOD;					   // Period
	EPwm1Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;		   // Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // Sync down-stream module
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;			// Clock sin divisiones
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPwm4A
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm1Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm1Regs.DBRED = 50;						   // RED = 50 TBCLKs

	// EPWM Module 5 config
	EPwm2Regs.TBPRD = PERIOD;					   // Period
	EPwm2Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   // Slave module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPWM5A
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm2Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm2Regs.DBRED = 50;						   // RED = 50 TBCLKs

	// EPWM Module 6 config
	EPwm3Regs.TBPRD = PERIOD;					   // Period = 1600 TBCLK counts
	EPwm3Regs.TBPHS.half.TBPHS = 0;				   // Set Phase register to zero
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Symmetrical mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;		   // Slave module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;			  // set actions for EPWM6A
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;	   // Active Hi complementary
	EPwm3Regs.DBFED = 50;						   // FED = 50 TBCLKs
	EPwm3Regs.DBRED = 50;						   // RED = 50 TBCLKs

	// Run Time (Note: Example execution of one run-time instant)
	//=========================================================
	EPwm1Regs.CMPB = PERIOD - SOC_HALF_TIME; // Para marcar el inicio de las conversiones analogicas
	EPwm1Regs.CMPA.half.CMPA = PERIOD;		 // DC = 0
	EPwm2Regs.CMPA.half.CMPA = PERIOD;		 // DC = 0
	EPwm3Regs.CMPA.half.CMPA = PERIOD;		 // DC = 0

	// ADC SOCA desde el ePWM1
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;	 // Genera el SOCA al primer evento
	EPwm1Regs.ETSEL.bit.SOCASEL = 6; // 110, SOCA on CMPB rising
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;	 // Habilita el SOCA

	// Configura la interrupcion desde el pwm1
	EPwm1Regs.ETSEL.bit.INTEN = 1;	// habilita interrupcion
	EPwm1Regs.ETSEL.bit.INTSEL = 1; // trigger cuando el timer es cero
	EPwm1Regs.ETPS.bit.INTPRD = 1;	// Dispara al primer evento

	// Habilita la interrupcion
	asm(" EALLOW");						   // Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // HSPCLK to ePWM modules enabled
	PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	   // Disable the PIE
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;	   // Enable EPWM1_INT in PIE group 3
	IER |= 0b0100;						   // Enable INT3 in IER to enable PIE group
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	   // Enable the PIE
	asm(" CLRC INTM");
	asm(" EDIS"); // Disable EALLOW protected register access

	// Inciamos el ePWM4 con fines de debug
	pwm4_init();

	enableEPWMClock();
}

/**
 * @brief Inicializa los registros del EPWM4 para generar un PWM de 10 KHz y DC variable en los pines 06 y 07
 * El 06 responde al CMPA y el 07 responde al CMPB.
 * La escala es de 0 a 7500, siendo 0 el 100% el alto y 7500 el bajo.
 *
 * Oportunidad de mejora: no se estan utilizando prescalers, por lo cual "se desperdicia" resoluciÃ³n del timer
 * ya que este es de 16 bits (alcanza para 65535 cuentas)
 *
 */
void pwm4_init()
{
	EPwm4Regs.TBCTL.bit.CTRMODE = 0x3; // Disable the timer -> NO DEBERIA SER NECESARIA POR LA LINEA 37

	EPwm4Regs.TBCTL.all = 0xC033; // Configure timer control register
	// bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend
	// bit 13        0:      PHSDIR, 0 = count down after sync event
	// bit 12-10     000:    CLKDIV, 000 => TBCLK = HSPCLK/1
	// bit 9-7       000:    HSPCLKDIV, 000 => HSPCLK = SYSCLKOUT/1
	// bit 6         0:      SWFSYNC, 0 = no software sync produced
	// bit 5-4       11:     SYNCOSEL, 11 = sync-out disabled
	// bit 3         0:      PRDLD, 0 = reload PRD on counter=0 PRD=PERIOD
	// bit 2         0:      PHSEN, 0 = phase control disabled
	// bit 1-0       11:     CTRMODE, 11 = timer stopped (disabled)

	EPwm4Regs.TBCTR = 0x0000;			 // Clear timer counter
	EPwm4Regs.TBPRD = PROJECT_PERIOD;	 // Levanto la frecuencia a 10 kHz PWM_HALF_PERIOD
	EPwm4Regs.TBPHS.half.TBPHS = 0x0000; // Set timer phase

	// EPwm4Regs.CMPA.half.CMPA = PWM_DUTY_CYCLE;	// Set PWM duty cycle
	EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * 0.5;
	EPwm4Regs.CMPB = PROJECT_PERIOD * 0.75;

	// EPwm4Regs.ETSEL.all = 0b01100;			// Activo la interrupciÃ³n EPWM
	//  bit 15        0:      SOCBEN, 0 = disable SOCB
	//  bit 14-12     000:    SOCBSEL, don't care
	//  bit 11        0:      SOCAEN, 0 = enable SOCA
	//  bit 10-8      000:    SOCASEL, 000 = SOCA on PRD event
	//  bit 7-4       0000:   reserved
	//  bit 3         1:      INTEN, 1 = enable interrupt
	//  bit 2-0       000:    INTSEL, don't care
	// EPwm4Regs.ETSEL.bit.INTEN = 1;	// habilita interrupcion
	// EPwm4Regs.ETSEL.bit.INTSEL = 2; // trigger a en el TBPRD
	// EPwm4Regs.ETPS.bit.INTPRD = 1;	// EPWMxINTn Period Select

	// **** COMPARE CONTROL REGISTER
	// EPwm4Regs.CMPCTL.all = 0x0002; // Compare control register
	// bit 15-10     0's:    reserved
	// bit 9         0:      SHDWBFULL, read-only
	// bit 8         0:      SHDWAFULL, read-only
	// bit 7         0:      reserved
	// bit 6         0:      SHDWBMODE, don't care
	// bit 5         0:      reserved
	// bit 4         0:      SHDWAMODE, 0 = shadow mode
	// bit 3-2       00:     LOADBMODE, don't care
	// bit 1-0       10:     LOADAMODE, 10 = load on zero or PRD match
	EPwm4Regs.CMPCTL.bit.LOADBMODE = 2;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = 2;

	EPwm4Regs.AQCTLA.all = 0x0060; // Action-qualifier control register A
	EPwm4Regs.AQCTLB.all = 0x0600; // Action-qualifier control register B
	// bit 15-12     0000:   reserved
	// bit 11-10     00:     CBD, 00 = do nothing
	// bit 9-8       00:     CBU, 00 = do nothing
	// bit 7-6       01:     CAD, 01 = clear
	// bit 5-4       10:     CAU, 10 = set
	// bit 3-2       00:     PRD, 00 = do nothing
	// bit 1-0       00:     ZRO, 00 = do nothing

	EPwm4Regs.AQSFRC.all = 0x0000; // Action-qualifier s/w force register
	// bit 15-8      0's:    reserved
	// bit 7-6       00:     RLDCSF, 00 = reload AQCSFRC on zero
	// bit 5         0:      OTSFB, 0 = do not initiate a s/w forced event on output B
	// bit 4-3       00:     ACTSFB, don't care
	// bit 2         0:      OTSFA, 0 = do not initiate a s/w forced event on output A
	// bit 1-0       00:     ACTSFA, don't care

	EPwm4Regs.AQCSFRC.all = 0x0000; // Action-qualifier continuous s/w force register
	// bit 15-4      0's:    reserved
	// bit 3-2       00:     CSFB, 00 = forcing disabled
	// bit 1-0       00:     CSFA, 00 = forcing disabled

	EPwm4Regs.DBCTL.bit.OUT_MODE = 0; // Deadband disabled
	EPwm4Regs.PCCTL.bit.CHPEN = 0;	  // PWM chopper unit disabled

	EPwm4Regs.TZCTL.bit.TZA = 0x3; // Trip action disabled for output A
	EPwm4Regs.TZCTL.bit.TZB = 0x3; // Trip action disabled for output B

	EPwm4Regs.TBCTL.bit.CTRMODE = 0x2; // Enable the timer in count up/down mode

	// TODO: BORRAR LA SIGUIENTE LÃ�NEA
	// enableEPWMClock();
}
