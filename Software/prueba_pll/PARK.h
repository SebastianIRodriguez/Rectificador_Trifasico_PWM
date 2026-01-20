#ifndef PARK_H
#define PARK_H

//*********** Structure Definition ********//
typedef struct{
	float alpha;
	float beta;
	float zero;
	float sin;
	float cos;
	float d;
	float q;
	float z;
}PARK;

//*********** Function Declarations *******//
void PARK_init(PARK *v);

//*********** Macro Definition ***********//
#define PARK_MACRO(v) 					\
	v.d = v.alpha*v.cos + v.beta*v.sin;	\
	v.q =-v.alpha*v.sin + v.beta*v.cos;	\
	v.z = v.zero;

#define iPARK_MACRO(v) 					\
	v.alpha = v.d*v.cos - v.q*v.sin;	\
	v.beta  = v.d*v.sin + v.q*v.cos;	\
	v.zero  = v.z;

#endif /* PARK_H_ */
