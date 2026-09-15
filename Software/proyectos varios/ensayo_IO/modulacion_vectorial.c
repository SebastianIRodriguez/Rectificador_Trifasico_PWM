#include "math.h"
#include "Lab.h"

float frecuencia = 50;
float V_ref = 0.5; // modulo? 1.6
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
int secuencia_comun()
{
    static float fase = 0;
    float E = 3.3;

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

    actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

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

    //actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}

/**
 * @brief Reproduce en la salida PWM la posición del vector rotante
 * 
 * @param u amplitud del vector
 * @param theta ángulo del vector
 * @return int nada
 */
int imprimir_vector(float u, float theta)
{
    float fase = theta;
    float E = 20.7; // Tension del bus de continua

    float periodo = 1.0 / frecuencia;
    float Ts = 0.0001; // Siempre que Fs sea 10 kHz

    fase += Ts * 2 * M_PI / periodo;
    // fase = fase % (2 * M_PI);
    fase = fmod(fase, 2 * M_PI); // Creo que esto es lo que deberia ir

    float Vx = u * cos(fase);
    float Vy = u * sin(fase);

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

    //actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}

/**
 * @brief Genera una señal PWM senoidal en cada fase, cada fase posee la misma señal desfasada 60°
 *
 * @return int nada
 */
int modulacion_senoidal()
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
    ta = 1.73205 * V_ref * Ts * sin(M_PI / 3 - fase_rel) / E;
    tb = 1.73205 * V_ref * Ts * sin(fase_rel) / E;
    t0 = Ts - ta - tb;

    float t_pwm1, t_pwm2, t_pwm3;
    switch (sext)
    {
    case 1:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = Ts / 2 + tb / 2;
        t_pwm3 = Ts / 2 - ta / 2;
        break;

    case 2:
        t_pwm1 = Ts / 2 + ta / 2; // ta + tb/2 + t0/2
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = Ts / 2 + tb / 2;
        break;

    case 3:
        t_pwm1 = Ts / 2 - tb / 2; // ta + t0 / 2;
        t_pwm2 = Ts / 2 + ta / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 4:
        t_pwm1 = t0 / 2; // t0/2
        t_pwm2 = Ts / 2 - tb / 2;
        t_pwm3 = Ts / 2 + ta / 2;
        break;

    case 5:
        t_pwm1 = Ts / 2 - ta / 2; // tb/2 + t0/2;
        t_pwm2 = t0 / 2;
        t_pwm3 = Ts / 2 - tb / 2;
        break;

    case 6:
        t_pwm1 = Ts / 2 + tb / 2; // ta/2 + tb + t0 / 2;
        t_pwm2 = Ts / 2 - ta / 2;
        t_pwm3 = t0 / 2;
        break;

    default:
        break;
    }

    actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}

/**
 * @brief Genera una señal PWM senoidal en cada fase, cada fase posee la misma señal desfasada 120°
 *
 * @return int nada
 */
int modulacion_senoidal_2()
{
    static float fase = 0;
    float E = 3.3;

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
    ta = 1.73205 * V_ref * Ts * sin(M_PI / 3 - fase_rel) / E;
    tb = 1.73205 * V_ref * Ts * sin(fase_rel) / E;
    t0 = Ts - ta - tb;

    float t_pwm1, t_pwm2, t_pwm3;
    switch (sext)
    {
    case 1:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = Ts / 2 - ta / 2;
        t_pwm3 = Ts / 2 - tb / 2;
        break;

    case 2:
        t_pwm1 = Ts / 2 + ta / 2; // ta + tb/2 + t0/2
        t_pwm2 = Ts / 2 + tb / 2;
        t_pwm3 = t0 / 2;
        break;

    case 3:
        t_pwm1 = Ts / 2 - tb / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = Ts / 2 - ta / 2;
        break;

    case 4:
        t_pwm1 = t0 / 2;
        t_pwm2 = Ts / 2 + ta / 2;
        t_pwm3 = Ts / 2 + tb / 2;
        break;

    case 5:
        t_pwm1 = Ts / 2 - ta / 2; // tb/2 + t0/2;
        t_pwm2 = Ts / 2 - tb / 2;
        t_pwm3 = ta + tb + t0 / 2;
        break;

    case 6:
        t_pwm1 = Ts / 2 + tb / 2; // ta/2 + tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm3 = Ts / 2 + ta / 2;
        break;

    default:
        break;
    }

    actualizar_pwm_vector_rotante(Vx, Vy);

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
    EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
    EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}

int sec = 0;
int interrupcion()
{
    if (sec == 0)
        secuencia_optima();
    else if (sec == 1)
        secuencia_comun();
    else if (sec == 2)
        modulacion_senoidal_2();
    return 0;
}

