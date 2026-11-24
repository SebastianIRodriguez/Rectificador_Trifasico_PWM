#include "Renolib.h"

//*********** Structure Init Function ****//
void IPQ_MEAS_init(IPQ_MEAS *v) {
	v->va = 0.0;
	v->vb = 0.0;
	v->vc = 0.0;
	v->ia = 0.0;
	v->ib = 0.0;
	v->ic = 0.0;
	v->p = 0.0;
	v->q = 0.0;
}
