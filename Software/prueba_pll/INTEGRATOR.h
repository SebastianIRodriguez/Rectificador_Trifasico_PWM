#ifndef RENOLIB_INCLUDE_INTEGRATOR_H_
#define RENOLIB_INCLUDE_INTEGRATOR_H_

//*********** Structure Definition ********//
typedef struct{
    float in;
    float x;
    float dT;
    float out;
}INTEGRATOR;

//*********** Function Declarations *******//
void INTEGRATOR_init(INTEGRATOR *v, float dT);

//*********** Macro Definition ***********//
#define INTEGRATOR_MACRO(v)                     \
    v.x = v.in*v.dT + v.x;                      \
    v.out = v.x;                                \

#endif /* RENOLIB_INCLUDE_INTEGRATOR_H_ */
