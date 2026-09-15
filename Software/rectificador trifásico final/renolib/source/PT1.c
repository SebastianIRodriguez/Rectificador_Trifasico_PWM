#include <Renolib.h>

//*********** Structure Init Function ****//
void PT1_init(PT1 *v, float K, float T, float dT){
	v->in = 0.0;
	v->K = K;
	v->T = T;
	v->x = 0.0;
	v->dT = dT;
	v->out = 0.0;
}
