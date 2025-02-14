#ifndef RENOLIB_INCLUDE_PWM_DAC_H_
#define RENOLIB_INCLUDE_PWM_DAC_H_

//*********** Structure Definition ********//
typedef struct{
	float in;
	float zscale, fscale;
	float out;
}PWM_DAC;

//*********** Function Declarations *******//
void PWM_DAC_init(PWM_DAC *v, float zscale, float fscale);

//*********** Macro Definition ***********//
#define PWM_DAC_MACRO(v)															\
	v.out = (v.in - v.zscale)/(v.fscale - v.zscale);								\

#endif /* RENOLIB_INCLUDE_PWM_DAC_H_ */
