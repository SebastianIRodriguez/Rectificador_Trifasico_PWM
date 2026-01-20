#ifndef RENOLIB_INCLUDE_ANG_DIFF_H_
#define RENOLIB_INCLUDE_ANG_DIFF_H_

/*
-------------------------------------------------------------------------
BLOQUE:         ANG_DIFF
DESCRIPCION:    Diferencia entre dos angulos
VERSION:        1.0 - 1/12/2024
DESARROLLADOR:  Pablo Amoedo
ENTRADAS:       a = angulo 1, en radianes (float)
                b = angulo 2, en radianes (float)
PARAMETROS:     No tiene
SALIDAS         out = diferencia entre angulo 1 y 2 = ang1 - ang2 (float)
DETALLE:        Calculo de diferencia existente entre dos angulos, dados
                por las entradas a y b. Valor limitado entre -PI y PI.
DEFINICION & INICIALIZACION:
                ANG_DIFF mi_Ang_Diff;
                ANG_DIFF_init(&mi_Ang_Diff);
UTILIZACION:
                mi_Ang_Diff.a = mi_angulo_1;
                mi_Ang.Diff.b = mi_angulo_2;
                ANG_DIFF_MACRO(mi_Ang_Diff);
                dif_entre_angulos = mi_Ang_Diff.out;
-------------------------------------------------------------------------
*/

typedef struct{
    float a, b;
    float out;
} ANG_DIFF;

void ANG_DIFF_init(ANG_DIFF *v);

#define ANG_DIFF_MACRO(v)                                       \
    if ((v.a-v.b) > PI) v.out = v.a - v.b - TWO_PI;             \
    else if ((v.a-v.b) < -PI) v.out = v.a - v.b + TWO_PI;       \
    else v.out = v.a - v.b;

#endif /* RENOLIB_INCLUDE_ANG_DIFF_H_ */