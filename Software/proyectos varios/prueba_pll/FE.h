#ifndef RENOLIB_INCLUDE_FE_H_
#define RENOLIB_INCLUDE_FE_H_
#include "Integrator.h"

/*********** Structure Definitions ***********/
typedef struct{
    float in, wn;
    float u2;
    INTEGRATOR Int;
    float out;
}FE;

//*********** Function Declarations *******//
void FE_init(FE *v, float u2, float wn, float dT);

//*********** Macro Definition ***********//
#define FE_MACRO(v)                                                                                 \
    v.Int.in = v.in*v.u2;																			\
	INTEGRATOR_MACRO(v.Int);																		\
	v.out = v.wn + v.Int.out;																		\

#endif /* RENOLIB_INCLUDE_FE_H_ */
