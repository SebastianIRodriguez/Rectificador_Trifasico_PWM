#ifndef CLARKE_H
#define CLARKE_H

//*********** Structure Definition ********//
typedef struct{
	float a;
	float b;
	float c;
	float alpha;
	float beta;
	float zero;
}CLARKE;

//*********** Function Declarations *******//
void CLARKE_init(CLARKE *v);

//*********** Macro Definition ***********//
#define CLARKE_MACRO(v) 									\
	v.alpha = 0.66666666667 * (v.a - 0.5*(v.b + v.c));		\
	v.beta = 0.57735026913 * (v.b - v.c);					\
	v.zero = 0.33333333333 * (v.a + v.b + v.c);

#define iCLARKE_MACRO(v)									\
	v.a = v.alpha + 0.5*v.zero;								\
	v.b = -0.5*v.alpha + 0.8660254*v.beta + 0.5*v.zero;		\
	v.c = -0.5*v.alpha - 0.8660254*v.beta + 0.5*v.zero;
	
#endif /* CLARKE_H_ */
