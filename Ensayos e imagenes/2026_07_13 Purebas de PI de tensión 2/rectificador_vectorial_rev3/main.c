
/**
 * Proyecto que controla un inversor pwm con modulacion vectorial.
 * Al mismo se le pueden entregar setpoints de Id e Iq para que entregue o consuma potencia activa o reactiva
 * El pin GPIO11 realiza un toggle en cada interrupción a modo de controlar su ejecucion
 * La máxima tensión de pico de salida es 0.57*Vbus
 *
 *      rev 1: se agregaron las estructuras de software a "project_interrupts" para poder ver
 *      la respuesta del PLL a los saltos en fase, amplitud y frecuencia.
 *
 *      rev 2: se agrego la proteccion por trip-zone. Para ello se altero el inicio de ePWM (PWM_config)
 *
 *      rev 4: se retir� la proteccion de tripzone por one-shot en el pin de tripzone. Ahora se hace un promedio de polling en la interrupcion principal
 *
 *      rev 5: se agrego la macro para abrir y cerrar el modulo IPM
 *
 *      rev 6: agregados cores para el 5to arm�nico sec positiva
 *
 *      rev 7: incorporado el sistema de precarga
 *
 * ------------ Rectificador vectorial
 *
 *      rev 1: ex rev 7: incorporado el sistema de precarga
 *
 *      rev 2: modificadas las funciones para darles señal a los IGBTs luego de construido el bus.
 *
 *      rev 3: se modifica la maquina de estados para hacer el cambio en la ki del control tensi�n acorde a la etapa del equipo
 *
 * Dependencias:
 *      - SysCtrl.c:    Implementa InitSysCtrl() que configura el PLL interno del chip para obtener 48MHz de trabajo y le brinda clock a m�ltiples m�dulos de HW
 *      - Gpio.c:       Implementa InitGpio(), funcion que le asigna la funcion a cada pin del chip (gpio, salida PWM, etc)
 *      - Pie_config.c  Implementa InitPieCtrl, funcion que habilita las interrupciones del PIE (Peripheral Interrupt Expansion)
 */

#include "project.h"
#include "renolib.h"
#include "inversor.h"

int habilitar_cdc = 0;
int habilitar_cac = 0;
int habilitar_ipm = 0;
int habilitar_precarga = 0;
int encender_convertidor = 0;

int encender_sistema = 0; // Enciende todo el sistema, arrancando por la precarga

int cac_state = 0;
int cdc_state = 0;
int qac_state = 0;
int qdc_state = 0;
int relay_state = 0;

// CAC1_CLOSE;
//  FORCE_TRIPZONE_INTERRUPT;

int main(void)
{
    // --- CPU Initializationy
    InitSysCtrl();  // Initialize the CPU (FILE: SysCtrl.c)
    InitGpio();     // Initialize the shared GPIO pins (FILE: Gpio.c)
    InitPieCtrl();  // Initialize and enable the PIE (FILE: PieCtrl.c)
    InitWatchdog(); // Initialize the Watchdog Timer (FILE: WatchDog.c)

    // InitTripzone();  // Inicializa la proteccion por trip-zone
    pwm4_init(); // Utilizado solo para exteriorizar las variables de control

    // Iniciamos el ADC para el muestreo de las tensiones y corrientes RST, ademas del bus de continua
    InitAdc(6);

    // Inicializamos las macros del proyecto
    init_interrupts_macros();

    // Inciar los PWM 1, 2 y 3 en formato de modulacion vectorial y habilitamos la interrupcion del ePWM1
    InitEPwm123_withInterrupt();

    // Conecto los ePWM a los pines correspondientes
    //enable_epwm_mux();

    while (1)
    {
        cac_state = GET_CAC1_STATE;
        cdc_state = GET_CDC1_STATE;
        qac_state = GET_QAC1_STATE;
        qdc_state = GET_QDC1_STATE;
        relay_state = GET_PRECHARGE_RELAY_STATE;

        /*
        if (habilitar_cdc)
        {
            CDC1_CLOSE;
        }
        else
        {
            CDC1_OPEN;
        }

        if (habilitar_cac)
        {
            CAC1_CLOSE;
        }
        else
        {
            CAC1_OPEN;
        }

        if (habilitar_ipm)
        {
            IPM_CLOSE;
        }
        else
        {
            IPM_OPEN;
        }

        if (habilitar_precarga)
        {
            PRECHARGE_CLOSE;
        }
        else
        {
            PRECHARGE_OPEN;
        }

        if (encender_convertidor)
        {
            StartInversor();
        }
        else
        {
            StopInversor();
        }
        */
    }
}
