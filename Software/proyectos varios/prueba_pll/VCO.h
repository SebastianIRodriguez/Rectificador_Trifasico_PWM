#ifndef RENOLIB_INCLUDE_VCO_H
#define RENOLIB_INCLUDE_VCO_H

//*********** Structure Definition ********//
typedef struct{
    float in;
    float wn;
    float x;
    float dT;
    float out;
}VCO;

//*********** Function Declarations *******//
void VCO_init(VCO *v, float wn, float dT);

//*********** Macro Definition ***********//
#define VCO_MACRO(v) 					        \
    v.x = v.x + v.dT * (v.in + v.wn);           \
    if (v.x > TWO_PI)                           \
        v.x = v.x - TWO_PI;                     \
    else if (v.x < 0)                           \
        v.x = v.x + TWO_PI;                     \
    v.out = v.x;                                \

#endif /* RENOLIB_INCLUDE_VCO_H_ */
