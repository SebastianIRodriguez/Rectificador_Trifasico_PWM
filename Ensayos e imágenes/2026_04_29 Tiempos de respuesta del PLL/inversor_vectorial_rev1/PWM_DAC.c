#include <Renolib.h>

//*********** Structure Init Function ****//
void PWM_DAC_init(PWM_DAC *v, float zscale, float fscale){
	v->in = 0.0;
	v->zscale = zscale;
	v->fscale = fscale;
	v->out = 0x0;
}
