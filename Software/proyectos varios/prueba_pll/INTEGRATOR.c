#include <Renolib.h>

//*********** Structure Init Function ****//
void INTEGRATOR_init(INTEGRATOR *v, float dT) {
    v->in = 0.0;
    v->x = 0.0;
    v->dT = dT;
    v->out = 0.0;
}
