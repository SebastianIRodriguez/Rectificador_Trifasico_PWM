#include <Renolib.h>

//*********** Structure Init Function ****//
void VCO_init(VCO *v, float wn, float dT){
	v->in = 0.0;
    v->wn = wn;
    v->dT = dT;
    v->x = 0.0;
    v->out = 0.0;
}
