#ifndef RENOLIB_INCLUDE_SYM_COS_H_
#define RENOLIB_INCLUDE_SYM_COS_H_

//*********** Structure Definition ********//
typedef struct{
    float sin, cos;
    float aux_1, aux_2;
    float out[3];
} SYM_COS;

//*********** Function Declarations *******//
void SYM_COS_init(SYM_COS *v);

//*********** Macro Definition ***********//
#define SYM_COS_MACRO(v)                                       	\
    v.aux_1 = -0.500000*v.cos;									\
	v.aux_2 = 0.866025*v.sin;									\
	v.out[0] = v.cos;											\
    v.out[1] = v.aux_1 + v.aux_2;								\
    v.out[2] = v.aux_1 - v.aux_2;

#endif /* RENOLIB_INCLUDE_SYM_COS_H_ */
