#include <Renolib.h>

//*********** Structure Init Function ****//
void RATE_LIMITER_init(RATE_LIMITER *v, float dT) {
    v->in = 0.0;
    v->dx = 0.0;
    v->dx_lim = 0.0;
    v->x = 0.0;
    v->dT = dT;
    v->out = 0.0;
}
