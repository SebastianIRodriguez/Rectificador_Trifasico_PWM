#ifndef RENOLIB_INCLUDE_ANG_LIM_H_
#define RENOLIB_INCLUDE_ANG_LIM_H_

/*
-------------------------------------------------------------------------
BLOQUE:         ANG_LIM
DESCRIPCION:    Limitador de angulo
VERSION:        1.0 - 1/12/2024
DESARROLLADOR:  Pablo Amoedo
ENTRADAS:       in = angulo, en radianes (float)
PARAMETROS:     sel = Selector de rango (int)
				[1] --> 0 a 2PI
				[2] --> -PI a PI
				[cualquier otro valor] --> out = in (lim. deshabil.)
SALIDAS         out = angulo limitado segun selector de rango (float)
DETALLE:        Limitador de angulo seleccionable para evitar el crecim.
				de variables angulares respecto al tiempo.
DEFINICION & INICIALIZACION:
                ANG_LIM mi_Ang_Lim;
				int ang_sel = 1; // Ejemplo entre 0 y 2PI.
                ANG_LIM_init(&mi_Ang_Lim,ang_sel);
UTILIZACION:
                mi_Ang_Lim.in = mi_angulo;
                ANG_LIM_MACRO(mi_Ang_Lim);
                angulo_limitado = mi_Ang_Lim.out;
-------------------------------------------------------------------------
*/

typedef struct{
	float in;
	int sel;
	float out;
}ANG_LIM;

void ANG_LIM_init(ANG_LIM *v, int sel);

#define ANG_LIM_MACRO(v)			 							\
	switch (v.sel) {											\
	case 1:														\
		v.out = v.in-((trunc(v.in/TWO_PI))*TWO_PI);				\
	case 2:														\
		v.out = v.in-((trunc((v.in+PI)/TWO_PI))*TWO_PI);		\
	default:													\
		v.out = v.in;											\
	}

#endif /* RENOLIB_INCLUDE_ANG_LIM_H_ */