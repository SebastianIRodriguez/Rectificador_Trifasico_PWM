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
static void adc7();

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
 * Descripcion: Inicializa el modulo ADC con un comportamiento deter-
 *              minado.
 **********************************************************************/
int adc_init(int config_code)
{
    ADC_MODE cc;

    if (config_code >= ADC_LAST_CODE)
        return -1;

    cc = (ADC_MODE)config_code;

    switch (cc) {
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

        case ADC_RECTIFIER_SIM_CODE:
            adc7();
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
    IER |= 0x0001;                     // Enable INT1 in IER to enable PIE group
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
    IER |= 0x0001;                     // Enable INT1 in IER to enable PIE group
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

    // ********************* ACA SE CONFIGURA LA FUENTE DE SOC
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
    IER |= 0x0001;                     // Enable INT1 in IER to enable PIE group
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
    AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0b11; // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
    AdcRegs.ADCTRL3.bit.ADCPWDN   = 1;    // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
    AdcRegs.ADCTRL3.bit.ADCCLKPS  = 3;    // bit 4-1       0011:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 150 MHz/2*3 = 25 MHz (T = 40 ns)
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;    // bit 0         1:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling

    /*
	AdcRegs.ADCTRL3.all = 0x00ED; // Power-up reference and main ADC
								  // bit 15-8      0's:    reserved
								  // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
								  // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
								  // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 25 MHz/2*6 = 2.08333 MHz (T = 480 ns)
	                              // bit 4-1       0110:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 150 MHz/2*6 = 12.5 MHz (T = 80 ns)
	                              // bit 0         1:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling //11101100
	*/

    DelayUs(5000); // Wait 5ms before using the ADC

    // ***** Indicamos la cantidad de conversiones analogicas que se realizaran por secuencia
    AdcRegs.ADCMAXCONV.all = 0x0007; //7
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

    // ADCTRL1[7]
    //AdcRegs.ADCTRL1.all = 0x0710; // = 0111 0001 0000
    // bit 15        0:      reserved
    // bit 14        0:      RESET, 0=no action, 1=reset ADC
    // bit 13-12     00:     SUSMOD, 00=ignore emulation suspend
    // bit 11-8      0111:   ACQ_PS (Acquisition), 0111 = 8 x ADCCLK
    // bit 7         0:      CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
    // bit 6         0:      CONT_RUN, 0=start/stop mode, 1=continuous run
    // bit 5         0:      SEQ_OVRD, 0=disabled, 1=enabled
    // bit 4         1:      SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
    // bit 3-0       0000:   reserved

    // bit 15        0:      reserved
    AdcRegs.ADCTRL1.bit.RESET    = 0; // bit 14    RESET, 0=no action, 1=reset ADC
    AdcRegs.ADCTRL1.bit.SUSMOD   = 0; // bit 13-12 SUSMOD, 00=ignore emulation suspend
    AdcRegs.ADCTRL1.bit.ACQ_PS   = 3; // bit 11-8  ACQ_PS (Acquisition) (7 + 1) = 8 x ADCCLK
    AdcRegs.ADCTRL1.bit.CPS      = 0; // bit 7     CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0; // bit 6     CONT_RUN, 0=start/stop mode, 1=continuous run
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0; // bit 5     SEQ_OVRD, 0=disabled, 1=enabled
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1; // bit 4     SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
    // bit 3-0       0000:   reserved

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
    //PieCtrlRegs.PIEIER1.bit.INTx6 = 1; // Enable ADCINT in PIE group 1
    //IER |= 0x0001;                     // Enable INT1 in IER to enable PIE group
}

/***********************************************************************
 * adc7()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Configura el ADC para muestrear los canales utilizados
 *              en el convertidor, dos veces seguidas. Toma el SOC
 *              desde ePWM y no habilita interrupcion.
 **********************************************************************/
static void adc7()
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
    // bit 15-8      0's:    reserved
    AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0b11; // bit 7-6       11:     ADCBGRFDN, reference power, 00=off, 11=on
    AdcRegs.ADCTRL3.bit.ADCPWDN   = 1;    // bit 5         1:      ADCPWDN, main ADC power, 0=off, 1=on
    AdcRegs.ADCTRL3.bit.ADCCLKPS  = 3;    // bit 4-1       0011:   ADCCLKPS, clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS) -> FCLK = 150 MHz/2*3 = 25 MHz (T = 40 ns)
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;    // bit 0         1:      SMODE_SEL, 0=sequential sampling, 1=simultaneous sampling

    DelayUs(5000); // Wait 5ms before using the ADC

    // ***** Indicamos la cantidad de conversiones analogicas que se realizaran por secuencia
    AdcRegs.ADCMAXCONV.all = 13; //(Hace 13 + 1 conversiones)
    // bit 15-7      0's:    reserved
    // bit 6-4       000:    MAX_CONV2 value
    // bit 3-0       0000:   MAX_CONV1 value (0 means 1 conversion)

    // ***** Asignamos los canales de conversion al secuenciador, dos muestreos a cada senal
    // ***** Primera rafaga de 7 canales (Muestreo 1)
    /*
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 6;  // Rv (ADCINA6) -> Resultado en ADCRESULT0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 13; // Ri (ADCINB5) -> Resultado en ADCRESULT1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 7;  // Sv (ADCINA7) -> Resultado en ADCRESULT2
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 12; // Si (ADCINB4) -> Resultado en ADCRESULT3
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 15; // Tv (ADCINB7) -> Resultado en ADCRESULT4
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 11; // Ti (ADCINB3) -> Resultado en ADCRESULT5
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 4;  // Yv (ADCINA4) -> Resultado en ADCRESULT6

    // ***** Segunda rafaga de los mismos 7 canales (Muestreo 2)
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 6;  // Rv (ADCINA6) -> Resultado en ADCRESULT7
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 13; // Ri (ADCINB5) -> Resultado en ADCRESULT8
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 7;  // Sv (ADCINA7) -> Resultado en ADCRESULT9
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 12; // Si (ADCINB4) -> Resultado en ADCRESULT10
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 15; // Tv (ADCINB7) -> Resultado en ADCRESULT11
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 11; // Ti (ADCINB3) -> Resultado en ADCRESULT12
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 4;  // Yv (ADCINA4) -> Resultado en ADCRESULT13
    */

    // ***** Primera ráfaga de 7 canales (Muestreo 1) - CON INVERSIÓN DE LADO
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 14; // Antes 6  (Ahora ADCINB6 - Rv)
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 5;  // Antes 13 (Ahora ADCINA5 - Ri)
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 15; // Antes 7  (Ahora ADCINB7 - Sv)
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 4;  // Antes 12 (Ahora ADCINA4 - Si)
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 7;  // Antes 15 (Ahora ADCINA7 - Tv)
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 3;  // Antes 11 (Ahora ADCINA3 - Ti)
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 12; // Antes 4  (Ahora ADCINB4 - Yv / Vbus)

    // ***** Segunda ráfaga de los mismos 7 canales (Muestreo 2) - CON INVERSIÓN DE LADO
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 14; // Antes 6  (Ahora ADCINB6 - Rv)
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 5;  // Antes 13 (Ahora ADCINA5 - Ri)
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 15; // Antes 7  (Ahora ADCINB7 - Sv)
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 4;  // Antes 12 (Ahora ADCINA4 - Si)
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 7;  // Antes 15 (Ahora ADCINA7 - Tv)
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 3;  // Antes 11 (Ahora ADCINA3 - Ti)
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 12; // Antes 4  (Ahora ADCINB4 - Yv / Vbus)

    // ***** ADCTRL1
    // bit 15        0:      reserved
    AdcRegs.ADCTRL1.bit.RESET    = 0; // bit 14    RESET, 0=no action, 1=reset ADC
    AdcRegs.ADCTRL1.bit.SUSMOD   = 0; // bit 13-12 SUSMOD, 00=ignore emulation suspend
    AdcRegs.ADCTRL1.bit.ACQ_PS   = 3; // bit 11-8  ACQ_PS (Acquisition) (3 + 1) = 4 x ADCCLK
    AdcRegs.ADCTRL1.bit.CPS      = 0; // bit 7     CPS (Core clock), 0: ADCCLK=FCLK/1, 1: ADCCLK=FCLK/2
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0; // bit 6     CONT_RUN, 0=start/stop mode, 1=continuous run
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0; // bit 5     SEQ_OVRD, 0=disabled, 1=enabled
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1; // bit 4     SEQ_CASC, 0=dual sequencer, 1=cascaded sequencer
    // bit 3-0       0000:   reserved

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
