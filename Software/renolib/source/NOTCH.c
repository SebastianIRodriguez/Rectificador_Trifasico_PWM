#include <Renolib.h>

void NOTCH_init(NOTCH *v) {
	v->in[0] = 0.0;
	v->in[1] = 0.0;
	v->in[2] = 0.0;
	v->out[0] = 0.0;
	v->out[1] = 0.0;
	v->out[2] = 0.0;
}

void NOTCH_update(float dT, float wn,float c2, float c1, NOTCH *v) {
	float x, y, z;
	x = 2.0*c2*wn*dT;
	y = 2.0*c1*wn*dT;
	z = wn*dT*wn*dT;
	v->A[0] = (y-2);
	v->A[1] = (z-y+1);
	v->B[0] = (x-2);
	v->B[1] = (z-x+1);
}
