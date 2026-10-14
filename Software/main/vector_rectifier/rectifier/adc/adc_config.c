/***********************************************************************
 * File:        adc_config.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se definen las funciones para la inicializacion del
 *              ADC. Se reutilizo parte del material del archivo
 *              "ADC_config.c" de la practica de texas
 *
 * History:
 *   13/05/2026 - original
 *   19/09/2026 - emprolijamiento del archivo
 **********************************************************************/

#include <DSP2833x_Device.h>
#include <rectifier/adc/adc.h>

extern void DelayUs(Uint16);

// ---------------- Declaracion de funciones locales ---------------- //

static void adc2();
static void adc4();
static void adc5();
static void adc6();


// ---------------- Definicion de funciones globales ---------------- //

/***********************************************************************
 * adc_init(config_code)
 *
 * Entrada:     config_code     Codigo de configuracion del adc
 *
 * Salida:      No posee
 *
 * Retorno:     0 si se ingreso un codigo valido, -1 caso contrario
 *
 * Descripcion: Salva un array de elementos
 **********************************************************************/
int adc_init(int config_code)
{
    ADC_MODE cc;

    if(config_code >= ADC_LAST_CODE)
        return -1;

    cc = (ADC_MODE)config_code;

	switch (cc)
	{
	case ADC_CH0_SEC_INT_CODE:
		adc2();
		break;

	case ADC_CH0_SIM_INT_CODE:
		adc4();
		break;

	case ADC_ALL_SIM_INT_CODE:
		adc5();
		break;

	case ADC_ALL_SIM_CODE:
		adc6();
		break;

	default:
		return -1;
	}

	return 0;
}

// ----------------- Definicion de funciones locales ---------------- //


/***********************************************************************
 * adc2()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Configura el ADC para muestrear el canal A0 y tomar el
 *              SOC desde ePWM. Ademas, habilita la interrupcion al
 *              finalizar la secuencia 1 (SEQ1)
 **********************************************************************/
static void adc2()
{
	//--- Reset the ADC module
	AdcRegs.ADCTRL1.bit.RESET = 1; // Reset the ADC

	// Must wait 2 ADCCLK periods for the reset to take effect.  The ADC is
	// already reset after a DSP reset, but this example is just showing good
	// coding practice to reset the peripheral before configuring it (as you
	// never know why the DSP has started the code over again from the
	// beginning).  Assuming a 12.5 MHz ADCCLK was previously configured, and
	// a 150 MHz SYSCLKOUT, the wait period of 2 ADCCLK periods equates to 24
	// CPU clocks.  This is the example being used below.

	asm(" RPT #22 || NOP"); // Must wait for ADC reset to take effect

	//--- Call the ADC_cal() function located in the Boot ROM.
	//    ADC_cal_func_ptr is a macro defined in the file example_nonBios.h or
	//    example_BIOS.h (as may be the case for the example being used).  This
	//    macro simply defines ADC_cal_func_ptr to be a function pointer to
	//    the correct address in the boot ROM.
	(*ADC_cal_func_ptr)();

	//--- Select the ADC reference
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0; // 0=internal, 1=external

	//--- Power-up the ADC
	AdcRegs.ADCTRL3.all = 0x00EC; // Power-up reference and main ADC
								  // bit 15-8      0's:    reserved
								  // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
								  // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
								  // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 25 MHz/2*6 = 2.08333 MHz
								  // bit 0         0:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling //11101100

	DelayUs(5000); // Wait 5ms before using the ADC

	//--- Configure the other ADC register
	AdcRegs.ADCMAXCONV.all = 0x0000;
	// bit 15-7      0's:    reserved
	// bit 6-4       000:    MAX_CONV2 value
	// bit 3-0       0000:   MAX_CONV1 value (0 means 1 conversion)

	// For CPU servicing of ADC, we are only doing 1 conversion in the
	// sequence.  So, we only need to configure the first channel
	// selection in the sequence.  All other channel selection fields
	// are don't cares in this example.
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // Convert Channel 0

	AdcRegs.ADCTRL1.all = 0x0710;
	// bit 15        0:      reserved
	// bit 14        0:      RESET, 0=no action, 1=reset ADC
	// bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
	// bit 11-8      0111:   ACQ_PS (Acquisition), 0111 = 8 x ADCCLK
	// bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
	// bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
	// bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
	// bit 4         1:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
	// bit 3-0       0000:   reserved
	AdcRegs.ADCTRL1.bit.ACQ_PS = 3; // (3 + 1) x ADCCLK

	AdcRegs.ADCTRL2.all = 0x0900;
	// bit 15        0:      ePWM_SOCB_SEQ, 0=no action
	// bit 14        0:      RST_SEQ1, 0=no action
	// bit 13        0:      SOC_SEQ1, 0=clear any pending SOCs
	// bit 12        0:      reserved
	// bit 11        1:      INT_ENA_SEQ1, 1=enable interrupt
	// bit 10        0:      INT_MOD_SEQ1, 0=int on every SEQ1 conv
	// bit 9         0:      reserved
	// bit 8         1:      ePWM_SOCA_SEQ1, 1=SEQ1 start from ePWM_SOCA trigger
	// bit 7         0:      EXT_SOC_SEQ1, 1=SEQ1 start from ADCSOC pin
	// bit 6         0:      RST_SEQ2, 0=no action
	// bit 5         0:      SOC_SEQ2, no effect in cascaded mode
	// bit 4         0:      reserved
	// bit 3         0:      INT_ENA_SEQ2, 0=int disabled
	// bit 2         0:      INT_MOD_SEQ2, 0=int on every other SEQ2 conv
	// bit 1         0:      reserved
	// bit 0         0:      ePWM_SOCB_SEQ2, 0=no action

	//--- Enable the ADC interrupt
	PieCtrlRegs.PIEIER1.bit.INTx6 = 1; // Enable ADCINT in PIE group 1
	IER |= 0x0001;					   // Enable INT1 in IER to enable PIE group
}

/**********************************************************************
 * adc4()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Configura el ADC para muestrear el canal A0 y B0 en modo
 *              simultaneo. Toma el SOC desde ePWM y habilita la
 *              interrupcion al finalizar la secuencia 1 (SEQ1)
 **********************************************************************/
static void adc4()
{
	//--- Reset the ADC module
	AdcRegs.ADCTRL1.bit.RESET = 1; // Reset the ADC

	// Must wait 2 ADCCLK periods for the reset to take effect.  The ADC is
	// already reset after a DSP reset, but this example is just showing good
	// coding practice to reset the peripheral before configuring it (as you
	// never know why the DSP has started the code over again from the
	// beginning).  Assuming a 12.5 MHz ADCCLK was previously configured, and
	// a 150 MHz SYSCLKOUT, the wait period of 2 ADCCLK periods equates to 24
	// CPU clocks.  This is the example being used below.

	asm(" RPT #22 || NOP"); // Must wait for ADC reset to take effect

	//--- Call the ADC_cal() function located in the Boot ROM.
	//    ADC_cal_func_ptr is a macro defined in the file example_nonBios.h or
	//    example_BIOS.h (as may be the case for the example being used).  This
	//    macro simply defines ADC_cal_func_ptr to be a function pointer to
	//    the correct address in the boot ROM.
	(*ADC_cal_func_ptr)();

	//--- Select the ADC reference
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0; // 0=internal, 1=external

	//--- Power-up the ADC
	AdcRegs.ADCTRL3.all = 0x00ED; // Power-up reference and main ADC
								  // bit 15-8      0's:    reserved
								  // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
								  // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
								  // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 25 MHz/2*6 = 2.08333 MHz
								  // bit 0         1:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling //11101100

	DelayUs(5000); // Wait 5ms before using the ADC

	//--- Configure the other ADC register
	AdcRegs.ADCMAXCONV.all = 0x0000;
	// bit 15-7      0's:    reserved
	// bit 6-4       000:    MAX_CONV2 value
	// bit 3-0       0000:   MAX_CONV1 value (0 means 1 conversion)

	// For CPU servicing of ADC, we are only doing 1 conversion in the
	// sequence.  So, we only need to configure the first channel
	// selection in the sequence.  All other channel selection fields
	// are don't cares in this example.
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // Convert Channel 0

	AdcRegs.ADCTRL1.all = 0x0710;
	// bit 15        0:      reserved
	// bit 14        0:      RESET, 0=no action, 1=reset ADC
	// bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
	// bit 11-8      0111:   ACQ_PS (Acquisition), 0111 = 8 x ADCCLK
	// bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
	// bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
	// bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
	// bit 4         1:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
	// bit 3-0       0000:   reserved
	AdcRegs.ADCTRL1.bit.ACQ_PS = 3; // (3 + 1) x ADCCLK

	AdcRegs.ADCTRL2.all = 0x0900;
	// bit 15        0:      ePWM_SOCB_SEQ, 0=no action
	// bit 14        0:      RST_SEQ1, 0=no action
	// bit 13        0:      SOC_SEQ1, 0=clear any pending SOCs
	// bit 12        0:      reserved
	// bit 11        1:      INT_ENA_SEQ1, 1=enable interrupt
	// bit 10        0:      INT_MOD_SEQ1, 0=int on every SEQ1 conv
	// bit 9         0:      reserved
	// bit 8         1:      ePWM_SOCA_SEQ1, 1=SEQ1 start from ePWM_SOCA trigger
	// bit 7         0:      EXT_SOC_SEQ1, 1=SEQ1 start from ADCSOC pin
	// bit 6         0:      RST_SEQ2, 0=no action
	// bit 5         0:      SOC_SEQ2, no effect in cascaded mode
	// bit 4         0:      reserved
	// bit 3         0:      INT_ENA_SEQ2, 0=int disabled
	// bit 2         0:      INT_MOD_SEQ2, 0=int on every other SEQ2 conv
	// bit 1         0:      reserved
	// bit 0         0:      ePWM_SOCB_SEQ2, 0=no action

	//--- Enable the ADC interrupt
	PieCtrlRegs.PIEIER1.bit.INTx6 = 1; // Enable ADCINT in PIE group 1
	IER |= 0x0001;					   // Enable INT1 in IER to enable PIE group
}

/***********************************************************************
 * adc5()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Configura el ADC para muestrear todos los canales en
 *              modo simultaneo. Toma el SOC desde ePWM y habilita la
 *              interrupcion al finalizar la secuencia 1 (SEQ1)
 **********************************************************************/
static void adc5()
{
	//--- Reset the ADC module
	AdcRegs.ADCTRL1.bit.RESET = 1; // Reset the ADC

	// Must wait 2 ADCCLK periods for the reset to take effect.  The ADC is
	// already reset after a DSP reset, but this example is just showing good
	// coding practice to reset the peripheral before configuring it (as you
	// never know why the DSP has started the code over again from the
	// beginning).  Assuming a 12.5 MHz ADCCLK was previously configured, and
	// a 150 MHz SYSCLKOUT, the wait period of 2 ADCCLK periods equates to 24
	// CPU clocks.  This is the example being used below.

	asm(" RPT #22 || NOP"); // Must wait for ADC reset to take effect

	//--- Call the ADC_cal() function located in the Boot ROM.
	//    ADC_cal_func_ptr is a macro defined in the file example_nonBios.h or
	//    example_BIOS.h (as may be the case for the example being used).  This
	//    macro simply defines ADC_cal_func_ptr to be a function pointer to
	//    the correct address in the boot ROM.
	(*ADC_cal_func_ptr)();

	//--- Select the ADC reference
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0; // 0=internal, 1=external

	//--- Power-up the ADC
	AdcRegs.ADCTRL3.all = 0x00ED; // Power-up reference and main ADC
								  // bit 15-8      0's:    reserved
								  // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
								  // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
								  // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 25 MHz/2*6 = 2.08333 MHz
								  // bit 0         1:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling //11101100

	DelayUs(5000); // Wait 5ms before using the ADC

	// ***** Indicamos la cantidad de conversiones analÃ³gicas que se realizarÃ¡n por secuencia
	AdcRegs.ADCMAXCONV.all = 0x0003;
	// bit 15-7      0's:    reserved
	// bit 6-4       000:    MAX_CONV2 value
	// bit 3-0       0000:   MAX_CONV1 value (0 means 1 conversion)

	// ***** Asignamos los canales de conversion al secuenciador
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // Convert Channel 0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // Convert Channel 1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; // Convert Channel 2
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; // Convert Channel 3
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4; // Convert Channel 4
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5; // Convert Channel 5
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 6; // Convert Channel 6
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 7; // Convert Channel 7

	// ********************* ACA SE CONFIGURA EL PERIODO DE ADQUISICION
	AdcRegs.ADCTRL1.all = 0x0710;
	// bit 15        0:      reserved
	// bit 14        0:      RESET, 0=no action, 1=reset ADC
	// bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
	// bit 11-8      0111:   ACQ_PS (Acquisition), 0111 = 8 x ADCCLK
	// bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
	// bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
	// bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
	// bit 4         1:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
	// bit 3-0       0000:   reserved
	AdcRegs.ADCTRL1.bit.ACQ_PS = 3; // (3 + 1) x ADCCLK

	// ********************* ACÃ� SE CONFIGURA LA FUENTE DE SOC
	AdcRegs.ADCTRL2.all = 0x0900;
	// bit 15        0:      ePWM_SOCB_SEQ, 0=no action
	// bit 14        0:      RST_SEQ1, 0=no action
	// bit 13        0:      SOC_SEQ1, 0=clear any pending SOCs
	// bit 12        0:      reserved
	// bit 11        1:      INT_ENA_SEQ1, 1=enable interrupt
	// bit 10        0:      INT_MOD_SEQ1, 0=int on every SEQ1 conv
	// bit 9         0:      reserved
	// bit 8         1:      ePWM_SOCA_SEQ1, 1=SEQ1 start from ePWM_SOCA trigger
	// bit 7         0:      EXT_SOC_SEQ1, 1=SEQ1 start from ADCSOC pin
	// bit 6         0:      RST_SEQ2, 0=no action
	// bit 5         0:      SOC_SEQ2, no effect in cascaded mode
	// bit 4         0:      reserved
	// bit 3         0:      INT_ENA_SEQ2, 0=int disabled
	// bit 2         0:      INT_MOD_SEQ2, 0=int on every other SEQ2 conv
	// bit 1         0:      reserved
	// bit 0         0:      ePWM_SOCB_SEQ2, 0=no action

	//--- Enable the ADC interrupt
	PieCtrlRegs.PIEIER1.bit.INTx6 = 1; // Enable ADCINT in PIE group 1
	IER |= 0x0001;					   // Enable INT1 in IER to enable PIE group
}

/***********************************************************************
 * adc6()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Configura el ADC para muestrear todos los canales en
 *              modo simultaneo. Toma el SOC desde ePWM y no habilita
 *              interrupcion.
 **********************************************************************/
static void adc6()
{
	//--- Reset the ADC module
	AdcRegs.ADCTRL1.bit.RESET = 1; // Reset the ADC

	// Must wait 2 ADCCLK periods for the reset to take effect.  The ADC is
	// already reset after a DSP reset, but this example is just showing good
	// coding practice to reset the peripheral before configuring it (as you
	// never know why the DSP has started the code over again from the
	// beginning).  Assuming a 12.5 MHz ADCCLK was previously configured, and
	// a 150 MHz SYSCLKOUT, the wait period of 2 ADCCLK periods equates to 24
	// CPU clocks.  This is the example being used below.

	asm(" RPT #22 || NOP"); // Must wait for ADC reset to take effect

	//--- Call the ADC_cal() function located in the Boot ROM.
	//    ADC_cal_func_ptr is a macro defined in the file example_nonBios.h or
	//    example_BIOS.h (as may be the case for the example being used).  This
	//    macro simply defines ADC_cal_func_ptr to be a function pointer to
	//    the correct address in the boot ROM.
	(*ADC_cal_func_ptr)();

	//--- Select the ADC reference
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0; // 0=internal, 1=external

	//--- Power-up the ADC
	AdcRegs.ADCTRL3.all = 0x00ED; // Power-up reference and main ADC
								  // bit 15-8      0's:    reserved
								  // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
								  // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
								  // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 25 MHz/2*6 = 2.08333 MHz
								  // bit 0         1:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling //11101100

	DelayUs(5000); // Wait 5ms before using the ADC

	// ***** Indicamos la cantidad de conversiones analogicas que se realizaran por secuencia
	AdcRegs.ADCMAXCONV.all = 0x0007;
	// bit 15-7      0's:    reserved
	// bit 6-4       000:    MAX_CONV2 value
	// bit 3-0       0000:   MAX_CONV1 value (0 means 1 conversion)

	// ***** Asignamos los canales de conversion al secuenciador
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // Convert Channel 0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // Convert Channel 1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; // Convert Channel 2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; // Convert Channel 3
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4; // Convert Channel 4
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5; // Convert Channel 5
	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 6; // Convert Channel 6
	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 7; // Convert Channel 7

	AdcRegs.ADCTRL1.all = 0x0710;
	// bit 15        0:      reserved
	// bit 14        0:      RESET, 0=no action, 1=reset ADC
	// bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
	// bit 11-8      0111:   ACQ_PS (Acquisition), 0111 = 8 x ADCCLK
	// bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
	// bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
	// bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
	// bit 4         1:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
	// bit 3-0       0000:   reserved
	AdcRegs.ADCTRL1.bit.ACQ_PS = 3; // (3 + 1) x ADCCLK, antes 3

	AdcRegs.ADCTRL2.all = 0x0900;
	// bit 15        0:      ePWM_SOCB_SEQ, 0=no action
	// bit 14        0:      RST_SEQ1, 0=no action
	// bit 13        0:      SOC_SEQ1, 0=clear any pending SOCs
	// bit 12        0:      reserved
	// bit 11        1:      INT_ENA_SEQ1, 1=enable interrupt
	// bit 10        0:      INT_MOD_SEQ1, 0=int on every SEQ1 conv
	// bit 9         0:      reserved
	// bit 8         1:      ePWM_SOCA_SEQ1, 1=SEQ1 start from ePWM_SOCA trigger
	// bit 7         0:      EXT_SOC_SEQ1, 1=SEQ1 start from ADCSOC pin
	// bit 6         0:      RST_SEQ2, 0=no action
	// bit 5         0:      SOC_SEQ2, no effect in cascaded mode
	// bit 4         0:      reserved
	// bit 3         0:      INT_ENA_SEQ2, 0=int disabled
	// bit 2         0:      INT_MOD_SEQ2, 0=int on every other SEQ2 conv
	// bit 1         0:      reserved
	// bit 0         0:      ePWM_SOCB_SEQ2, 0=no action

}
//--- end of file -----------------------------------------------------
