#ifndef RENOLIB_INCLUDE_SEQ_CORE_H_
#define RENOLIB_INCLUDE_SEQ_CORE_H_
#include "SYM_COS.h"
#include "SYM_SIN.h"
#include "VCO.h"
#include "INTEGRATOR.h"
#include "LIMITS.h"

//*********** Structure Definition ********//
typedef struct {
	float e[3], wo;
	float u1, u3;
	float k;
	float seq;
	SYM_COS C;
	SYM_SIN S;
	VCO CORE_VCO;
	INTEGRATOR Magn;
	LIMITS Ulim;
	float fe_out, out[3];
}SEQ_CORE;

//*********** Function Declarations *******//
void SEQ_CORE_init(SEQ_CORE *v, float k, float seq, float u1, float u3, float wn, float dT);

#define SEQ_CORE_MACRO(v)																		\
	v.Ulim.in = v.Magn.out;																		\
	LIMITS_MACRO(v.Ulim);																		\
	v.Magn.in = (v.e[0]*v.C.out[0]+v.e[1]*v.C.out[1]+v.e[2]*v.C.out[2])*v.u1;					\
	v.fe_out = -v.seq*(v.e[0]*v.S.out[0]+v.e[1]*v.S.out[1]+v.e[2]*v.S.out[2])/v.Ulim.out;		\
	v.CORE_VCO.in = v.u3*v.fe_out;																\
	v.CORE_VCO.wn = v.k*v.wo;																	\
	INTEGRATOR_MACRO(v.Magn);																	\
	VCO_MACRO(v.CORE_VCO);																		\
    v.C.sin = sin(v.seq*v.CORE_VCO.out+0.5);													\
	v.C.cos = cos(v.seq*v.CORE_VCO.out+0.5);													\
    v.S.sin = v.C.sin;																			\
	v.S.cos = v.C.cos;																			\
	SYM_COS_MACRO(v.C);																			\
	SYM_SIN_MACRO(v.S);																			\
	v.out[0] = v.Magn.out*v.C.out[0];															\
	v.out[1] = v.Magn.out*v.C.out[1];															\
	v.out[2] = v.Magn.out*v.C.out[2];															\

#endif /* RENOLIB_INCLUDE_SEQ_CORE_H_ */
