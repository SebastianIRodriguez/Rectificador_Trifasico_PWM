#include <Renolib.h>
#include <Aplicacion.h>

//*********** Structure Init Function ****//
void PO_init(PO *v, float h, float T){
	v->h = h;
	v->T = T;
	v->timer = 0.0;
	INTEGRATOR_init(&(v->Paccum), h);
	v->Pin = 0.0;
	v->P[1] = 0.0;
	v->P[2] = 0.0;
	v->Enable = FALSE;
	v->Inc[1] = FALSE;
	v->Inc[2] = FALSE;
	v->Dec[1] = FALSE;
	v->Dec[2] = TRUE;
}
