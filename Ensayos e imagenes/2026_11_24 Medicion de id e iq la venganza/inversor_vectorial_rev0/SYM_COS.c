#include <Renolib.h>

//*********** Structure Init Function ****//
void SYM_COS_init(SYM_COS *v){
	v->sin = 0.0;
	v->cos = 0.0;
	v->aux_1 = 0.0;
	v->aux_2 = 0.0;
	v->out[0] = 0.0;
	v->out[1] = 0.0;
	v->out[2] = 0.0;
}
