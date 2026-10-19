/***********************************************************************
 * File:        control_scheme.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se define la funcion que ejecuta el esquema de control
 *              del rectificador
 *
 * History:
 *   30/09/2026 - original
 **********************************************************************/

#include "project.h"
#include <Renolib.h>

#define I_FF_ENABLED 0

#define F_INTERRUPT  10000.0
#define KP_CONTROL_I 0.1
#define KI_CONTROL_I 20.0
#define KP_CONTROL_V 1.0
#define KI_CONTROL_V 0.0

#define UPPER_I_SAT 5  // Maximo valor positivo de la salida del control corriente
#define LOW_I_SAT   -5 // Maximo valor negativo de la salida del control corriente
#define UPPER_I_WUL 6  // Maximo valor positivo del error integral acumulado del control corriente
#define LOW_I_WUL   -6 // Maximo valor negativo de la salida del control corriente

static PARK            i_park;
static PARK            v_park;
static PARK            v_park_out;
static CLARKE          i_clarke;
static CLARKE          v_clarke;
static CLARKE          v_clark_out;
static PI_CONTROLLER   pi_vbus;
static E_PI_CONTROLLER pi_id;
static E_PI_CONTROLLER pi_iq;

static const float L = 0.001; // La inductacia por fase del tablero

/***********************************************************************
 * project_control_init()
 *
 * Entrada:     No posee
 *
 * Salida:      No posee
 *
 * Retorno:     No posee
 *
 * Descripcion: Inicializa el sistema de control
 **********************************************************************/
void project_control_init()
{
    CLARKE_init(&i_clarke);
    CLARKE_init(&v_clarke);
    CLARKE_init(&v_clark_out);

    PARK_init(&i_park);
    PARK_init(&v_park);
    PARK_init(&v_park_out);

    E_PI_CONTROLLER_init(&pi_id, KP_CONTROL_I, KI_CONTROL_I, 1 / F_INTERRUPT, UPPER_I_SAT, LOW_I_SAT, UPPER_I_WUL, LOW_I_WUL);
    E_PI_CONTROLLER_init(&pi_iq, KP_CONTROL_I, KI_CONTROL_I, 1 / F_INTERRUPT, UPPER_I_SAT, LOW_I_SAT, UPPER_I_WUL, LOW_I_WUL);
    PI_CONTROLLER_init(&pi_vbus, KP_CONTROL_V, KI_CONTROL_V, 1 / F_INTERRUPT);

    // Lo vuelvo a poner, poque por motivos que me exceden, al micro se le olvida que lo puse en 0
    pi_vbus.x = 0;
    pi_id.x   = 0;
    pi_iq.x   = 0;
}

/***********************************************************************
 * project_control_run(v, i, vbus, vsp, out)
 *
 * Entrada:     v               Magnitudes de las 3 tensiones
 *              i               Magnitudes de las 3 corrientes
 *              red_trifasica   Frecuencia de la red trifasica
 *              vbus            Tension actual del bus de CC
 *              vsp             Set point de tension de CC
 *
 * Salida:      out             Puntero a la estructura de salida
 *
 * Retorno:     No posee
 *
 * Descripcion: Ejecuta el esquema de control del rectificador
 **********************************************************************/
void project_control_run(SIGNAL_3Magn v, SIGNAL_3Magn i, PLL_VARS red_trifasica, float vbus, float vsp, ROTATING_VECTOR *out)
{
    float w;
    float mod_out;
    float theta_out;
    float coseno;
    float seno;
    float idsp;
    float iqsp = 0;

    // Frecuencia angular
    w = 2 * M_PI * red_trifasica.f;

    // CLARKE
    v_clarke.a = v.a;
    v_clarke.b = v.b;
    v_clarke.c = v.c;

    i_clarke.a = i.a;
    i_clarke.b = i.b;
    i_clarke.c = i.c;

    CLARKE_MACRO(v_clarke);
    CLARKE_MACRO(i_clarke);

    // PARK
    coseno = cos(red_trifasica.theta);
    seno   = sin(red_trifasica.theta);

    v_park.alpha = v_clarke.alpha;
    v_park.beta  = v_clarke.beta;
    v_park.zero  = v_clarke.zero;
    v_park.cos   = coseno;
    v_park.sin   = seno;

    i_park.alpha = i_clarke.alpha;
    i_park.beta  = i_clarke.beta;
    i_park.zero  = i_clarke.zero;
    i_park.cos   = coseno;
    i_park.sin   = seno;

    PARK_MACRO(v_park);
    PARK_MACRO(i_park);

    // Control tension (proporcional)
    pi_vbus.sp = vsp;
    pi_vbus.fb = vbus;
    //pi_vbus.Kp = kp_control_v;
    pi_vbus.Ki = 0;
    PI_CONTROLLER_MACRO(pi_vbus);

    // Control corriente (P+I)
    idsp = vbus * (1 / v_park.d) * pi_vbus.out * (-2.0 / 3.0);

    // Rama superior
    pi_id.sp = idsp;
    pi_id.fb = i_park.d;
    pi_id.Kp = KP_CONTROL_I;
    pi_id.Ki = KI_CONTROL_I;
    E_PI_CONTROLLER_MACRO(pi_id);
    v_park_out.d = v_park.d + pi_id.out - I_FF_ENABLED * i_park.q * w * L;

    // Rama inferior
    pi_iq.sp = iqsp;
    pi_iq.fb = i_park.q;
    pi_iq.Kp = KP_CONTROL_I;
    pi_iq.Ki = KI_CONTROL_I;
    E_PI_CONTROLLER_MACRO(pi_iq);
    v_park_out.q = v_park.q + pi_iq.out + I_FF_ENABLED * i_park.d * w * L;

    mod_out = sqrt(v_park_out.d * v_park_out.d + v_park_out.q * v_park_out.q);

    // ******************** Antitransformadas

    // ***** PARK
    v_park_out.cos = cos(red_trifasica.theta);
    v_park_out.sin = sin(red_trifasica.theta);
    iPARK_MACRO(v_park_out);

    // ***** Clarke
    v_clark_out.alpha = v_park_out.alpha;
    v_clark_out.beta  = v_park_out.beta;
    v_clark_out.zero  = v_park_out.zero;
    iCLARKE_MACRO(v_clark_out);

    theta_out = atan2(v_clark_out.beta, v_clark_out.alpha) + 2 * PI;

    out->mod   = mod_out;
    out->theta = theta_out;
}
