
/**
 * Proyecto que se utiliza para hacer pruebas varias sobre el HW existente como parte del proyecto final
 *
 * Dependencias:
 *      - SysCtrl.c:    Implementa InitSysCtrl() que configura el PLL interno del chip para obtener 48MHz de trabajo y le brinda clock a m�ltiples m�dulos de HW
 *      - Gpio.c:       Implementa InitGpio(), funcion que le asigna la funci�n a cada pin del chip (gpio, salida PWM, etc)
 *      - Pie_config.c  Implementa InitPieCtrl, funcion que habilita las interrupciones del PIE (Peripheral Interrupt Expansion)
 */

// Maxima tension de salida 0.57 Vbus

#include "Lab.h"
#include "pruebas.h"
#include "renolib.h"

int main(void)
{
    //--- CPU Initialization
    InitSysCtrl();  // Initialize the CPU (FILE: SysCtrl.c)
    InitGpio();     // Initialize the shared GPIO pins (FILE: Gpio.c)
    InitPieCtrl();  // Initialize and enable the PIE (FILE: PieCtrl.c)
    InitWatchdog(); // Initialize the Watchdog Timer (FILE: WatchDog.c)

    // InitAdc();
    // InitEPwm();
    //adc_interrupt_test();
    //adc_interrupt_test_2();
    //adc_interrupt_test_3();
    //adc_interrupt_test_4();
    //adc_interrupt_test_5();
    //pwm4_interrupt_test();

    // LA QUE VA ES SECUENCIA COMUN

    //pll_test();
    //adc_test();
    //pwm_test();
    //pwm_1_2_3_static_test();
    pwm1_interrupt_test();

    while (1)
    {
        // interrupcion();
        //secuencia_optima();
    }
}
