
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
 *      rev 4: se le dan los ultimos retoques al proyecto para su entrega
 *
 * Dependencias:
 *      - SysCtrl.c:    Implementa InitSysCtrl() que configura el PLL interno del chip para obtener 48MHz de trabajo y le brinda clock a m�ltiples m�dulos de HW
 *      - Gpio.c:       Implementa InitGpio(), funcion que le asigna la funcion a cada pin del chip (gpio, salida PWM, etc)
 *      - Pie_config.c  Implementa InitPieCtrl, funcion que habilita las interrupciones del PIE (Peripheral Interrupt Expansion)
 */

#include "project.h"

// Variables globales para el control de la bestia
volatile int main_encender_sistema       = 0; // Enciende todo el sistema, arrancando por la precarga
volatile int main_stop_without_discharge = 0; // Apaga la maquina sin descargar el bus de CC
volatile int main_stop_with_discharge    = 0; // Apaga la maquina descargando el bus de CC

// Variables gloables para el monitoreo del funcionamiento
int cac_state   = 0;
int cdc_state   = 0;
int qac_state   = 0;
int qdc_state   = 0;
int relay_state = 0;
int start_state = 0;

int estado_cdc = 1;
int estado_cac = 1;
int estado_ipm = 1;
int estado_relay = 1;

int main(void)
{
    // CPU Initialization
    InitSysCtrl();  // Initialize the CPU (FILE: SysCtrl.c)
    gpio_init();    // Initialize the shared GPIO pins (FILE: Gpio.c)
    InitPieCtrl();  // Initialize and enable the PIE (FILE: PieCtrl.c)
    InitWatchdog(); // Initialize the Watchdog Timer (FILE: WatchDog.c)

    // Iniciamos el ADC para el muestreo de las tensiones y corrientes RST, ademas del bus de continua
    adc_init(ADC_ALL_SIM_CODE);

    // Dentro de esta funcion se inicializa y arranca todo lo relacionado a la maquina
    project_interrupt_init();

    while (1) {
        // Variables utilizadas para control y debug
        board_digitalRead(BOARD_CAC1_STATE_PIN, &cac_state);
        board_digitalRead(BOARD_CDC1_STATE_PIN, &cdc_state);
        board_digitalRead(BOARD_QAC1_STATE_PIN, &qac_state);
        board_digitalRead(BOARD_QDC1_STATE_PIN, &qdc_state);
        board_digitalRead(BOARD_PRECHARGE_RELAY_STATE_PIN, &relay_state);
        board_digitalRead(BOARD_START_STATE_PIN, &start_state);

        // Comando manual
        board_digitalWrite(BOARD_CAC1_COMMAND_PIN, estado_cac);
        board_digitalWrite(BOARD_CDC1_COMMAND_PIN, estado_cdc);
        board_digitalWrite(BOARD_IPM_COMMAND_PIN, estado_ipm);
        board_digitalWrite(BOARD_PRECHARGE_RELAY_COMMAND_PIN, estado_relay);
    }
}
