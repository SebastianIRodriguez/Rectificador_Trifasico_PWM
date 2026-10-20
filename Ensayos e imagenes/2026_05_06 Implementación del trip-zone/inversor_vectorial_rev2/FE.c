#include <Renolib.h>

//*********** Structure Init Function ****//
void FE_init(FE *v, float u2, float wn, float dT){
	v->in = 0.0;
	v->u2 = u2;
	v->wn = wn;
	INTEGRATOR_init(&(v->Int), dT);
	v->out = 0.0;
}
