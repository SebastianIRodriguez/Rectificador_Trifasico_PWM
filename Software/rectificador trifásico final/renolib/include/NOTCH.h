#ifndef NOTCH_H_
#define NOTCH_H_

/*********** Structure Definitions ***********/
typedef struct{
	float B[2];
	float A[2];
	float out[3];
	float in[3];
}NOTCH;

//*********** Function Declarations *******//
void NOTCH_init(NOTCH *v);
void NOTCH_update(float dT, float wn,float c2, float c1, NOTCH *v);

//*********** Macro Definition ***********//
#define NOTCH_MACRO(v)																					\
	v.out[0]=-(v.A[0]*v.out[1])-(v.A[1]*v.out[2])+(v.in[0])+(v.B[0]*v.in[1])+(v.B[1]*v.in[2]); 			\
	v.out[2]=v.out[1];																					\
	v.out[1]=v.out[0];																					\
	v.in[2]=v.in[1];																					\
	v.in[1]=v.in[0];

#endif
