
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
    // --- CPU Initialization
    InitSysCtrl();  // Initialize the CPU (FILE: SysCtrl.c)
    InitGpio();     // Initialize the shared GPIO pins (FILE: Gpio.c)
    InitPieCtrl();  // Initialize and enable the PIE (FILE: PieCtrl.c)
    InitWatchdog(); // Initialize the Watchdog Timer (FILE: WatchDog.c)

    // --- Ensayos
    //pwm_1_2_3_static_test();

    //pwm_1_2_3_sim_vectorial_test();

    //DelayUs(20/2);

    // Oportunidad de mejora, que la eleccion de la senoidal simulada sea parametro
    pwm_1_2_3_pll_test();

    while (1)
    {
    }
}
