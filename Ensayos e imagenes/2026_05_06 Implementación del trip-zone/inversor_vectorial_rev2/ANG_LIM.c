#include <Renolib.h>

//*********** Structure Init Function ****//
void ANG_LIM_init(ANG_LIM *v, int sel){
	v->in = 0.0;
	v->sel = sel;
	v->out = 0.0;
}
