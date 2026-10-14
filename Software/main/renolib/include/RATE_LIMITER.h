#ifndef RENOLIB_INCLUDE_RATE_LIMITER_H_
#define RENOLIB_INCLUDE_RATE_LIMITER_H_

//*********** Structure Definition ********//
typedef struct{
    float in;
    float dx, dx_lim;
    float max_rate, min_rate;
    float x;
    float dT;
    float out;
}RATE_LIMITER;

//*********** Function Declarations *******//
void RATE_LIMITER_init(RATE_LIMITER *v, float dT);

//*********** Macro Definition ***********//
#define RATE_LIMITER_MACRO(v)                                                                                                               \
    v.dx = v.in-v.x;                                                                                                                        \
    v.dx_lim = ((((v.dx)>(v.max_rate)?(v.max_rate):(v.dx)))<(v.min_rate)?(v.min_rate):(((v.dx)>(v.max_rate)?(v.max_rate):(v.dx))));         \
    v.x = v.dx_lim*v.dT + v.x;                                                                                                              \

#endif /* RENOLIB_INCLUDE_RATE_LIMITER_H_ */
