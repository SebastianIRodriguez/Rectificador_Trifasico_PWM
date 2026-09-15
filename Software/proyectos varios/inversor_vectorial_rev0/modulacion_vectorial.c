#include "math.h"
#include "project.h"

float frecuencia = 50;
float V_ref = 5; // modulo? 1.6
float t0_min = 0.0001;

void actualizar_pwm_vector_rotante(float Vx, float Vy)
{
    EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - Vx / 3.3) - PROJECT_PERIOD / 2; // adjust duty for output EPWM1A
    EPwm1Regs.CMPB = PROJECT_PERIOD * (1 - Vy / 3.3) - PROJECT_PERIOD / 2;           // adjust duty for output EPWM1B
}

/**
 * @brief Secuencia de modulación vectorial según el órden convencional
 *
 * @return int nada
 */
int secuencia_simulada()
{
    static float fase = 0;
    float E = 60;    // Tension de bus
    float V_ref = 5; // Modulo de la senoidal

    float periodo = 1.0 / frecuencia;
    float Ts = 0.0001; // Siempre que Fs sea 10 kHz

    fase += Ts * 2 * M_PI / periodo;
    // fase = fase % (2 * M_PI);
    fase = fmod(fase, 2 * M_PI); // Creo que esto es lo que deberia ir

    float Vx = V_ref * cos(fase);
    float Vy = V_ref * sin(fase);

    // float fase_rel = fase % (M_PI / 3);
    float fase_rel = fmod(fase, M_PI / 3);
    int sext = (int)ceil(fase / (M_PI / 3));

    float ta, tb, t0;
    ta = sqrt(3) * V_ref / E * Ts * sin(M_PI / 3 - fase_rel);
    tb = sqrt(3) * V_ref / E * Ts * sin(fase_rel);
    t0 = Ts - ta - tb;

    float t_pwm1, t_pwm2, t_pwm3;
    switch (sext)
    {
    case 1:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;

    case 2:
        t_pwm1 = ta + t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;

    case 3:
        t_pwm1 = t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = tb + t0 / 2;
        break;

    case 4:
        t_pwm1 = t0 / 2;
        t_pwm2 = ta + t0 / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 5:
        t_pwm1 = tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 6:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = ta + t0 / 2;
        break;

    default:
        break;
    }

    // actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm2Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm3Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}

/**
 * @brief En teoría, es la modulación vectorial óptima que indica el apunte. Ésta invierte el órden de los vectores
 * en los cuadrantes pares
 *
 * @return int nada
 */
int secuencia_optima()
{
    static float fase = 0;
    float E = 1;

    float periodo = 1.0 / frecuencia;
    float Ts = 0.0001; // Siempre que Fs sea 10 kHz

    fase += Ts * 2 * M_PI / periodo;
    // fase = fase % (2 * M_PI);
    fase = fmod(fase, 2 * M_PI); // Creo que esto es lo que deberia ir

    float Vx = V_ref * cos(fase);
    float Vy = V_ref * sin(fase);

    // float fase_rel = fase % (M_PI / 3);
    float fase_rel = fmod(fase, M_PI / 3);
    int sext = (int)ceil(fase / (M_PI / 3));

    float ta, tb, t0;
    ta = sqrt(3) * V_ref / E * Ts * sin(M_PI / 3 - fase_rel); //    pi/3 = 60°
    tb = sqrt(3) * V_ref / E * Ts * sin(fase_rel);            // 98.72
    t0 = Ts - ta - tb;

    if (t0 < t0_min)
        t0_min = t0;

    float t_pwm1, t_pwm2, t_pwm3;
    switch (sext)
    {
    case 1:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;

    case 2:
        t_pwm1 = tb + t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;

    case 3:
        t_pwm1 = t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm1 = tb + t0 / 2;
        break;

    case 4:
        t_pwm1 = t0 / 2;
        t_pwm2 = tb + t0 / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 5:
        t_pwm1 = tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 6:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = tb + t0 / 2;
        break;

    default:
        break;
    }

    // actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}

/**
 * @brief Reproduce en la salida PWM la posición del vector rotante
 * Utiliza los ePWM 1, 2 y 3
 *
 * @param u modulo eficaz del vector
 * @param theta angulo del vector
 * @return int nada
 */
int imprimir_vector(float u, float theta)
{
    float E = 60;                 // Tension del bus de continua
    float fase;                   // Variable auxiliar para calcular la fase del vector rotante
    float Ts = 0.0001;            // Siempre que Fs sea 10 kHz
    float fase_rel;               // Fase relativa del vector rotante dentro del sextante
    int sext;                     // Sextante en el cual se encuentra el vector rotante
    float ta, tb, t0;             // Tiempos utilizados en la modulación vectorial
    float t_pwm1, t_pwm2, t_pwm3; // Tiempos que deben permanecer en alto cada uno de los PWMs para la reproducción del vector

    // Ajustamos la fase a un numero comprendido entre 0 y 2pi
    fase = theta;
    fase = fmod(fase, 2 * M_PI);

    // Calculamos el sextante en el que se encuentra el vector junto con su fase relativa dentro del mismo
    fase_rel = fmod(fase, M_PI / 3);
    sext = (int)ceil(fase / (M_PI / 3));

    // Calculamos los tiempos de la modulacion vectorial
    ta = sqrt(3) * u / E * Ts * sin(M_PI / 3 - fase_rel);
    tb = sqrt(3) * u / E * Ts * sin(fase_rel);
    t0 = Ts - ta - tb;

    // Calculamos el tiempo en alto de cada PWM
    switch (sext)
    {
    case 1:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;

    case 2:
        t_pwm1 = ta + t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;

    case 3:
        t_pwm1 = t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = tb + t0 / 2;
        break;

    case 4:
        t_pwm1 = t0 / 2;
        t_pwm2 = ta + t0 / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 5:
        t_pwm1 = tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 6:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = ta + t0 / 2;
        break;

    default:
        break;
    }

    // Reproducimos los PWM con el DC correspondiente
    EPwm1Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts);
    EPwm2Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts);
    EPwm3Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts);

    return 0;
}

// actualizar_pwm_vector_rotante(Vx, Vy);
