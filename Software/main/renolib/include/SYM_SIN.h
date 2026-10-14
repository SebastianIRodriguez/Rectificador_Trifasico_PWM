#ifndef RENOLIB_INCLUDE_SYM_SIN_H_
#define RENOLIB_INCLUDE_SYM_SIN_H_

//*********** Structure Definition ********//
typedef struct{
    float sin, cos;
    float aux_1, aux_2;
    float out[3];
} SYM_SIN;

//*********** Function Declarations *******//
void SYM_SIN_init(SYM_SIN *v);

//*********** Macro Definition ***********//
#define SYM_SIN_MACRO(v)                                       	\
    v.aux_1 = -0.500000*v.sin;									\
	v.aux_2 = 0.866025*v.cos;									\
	v.out[0] = v.sin;											\
    v.out[1] = v.aux_1 - v.aux_2;								\
    v.out[2] = v.aux_1 + v.aux_2;

#endif /* RENOLIB_INCLUDE_SYM_SIN_H_ */
