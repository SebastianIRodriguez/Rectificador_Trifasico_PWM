#include <Renolib.h>

//*********** Structure Init Function ****//
void SEQ_CORE_init(SEQ_CORE *v, float k, float seq, float u1, float u3, float wn, float dT){
	v->u1 = u1;
	v->u3 = u3;
	v->k = k;
	if (seq > 0) v->seq = 1.0;
	else v->seq = -1.0;
	VCO_init(&v->CORE_VCO, wn, dT);
	INTEGRATOR_init(&v->Magn, dT);
	SYM_COS_init(&v->C);
	SYM_SIN_init(&v->S);
	LIMITS_init(&v->Ulim, 9999, 0.01);
	v->fe_out = 0.0;
	v->out[0] = 0.0;
	v->out[1] = 0.0;
	v->out[2] = 0.0;
}
