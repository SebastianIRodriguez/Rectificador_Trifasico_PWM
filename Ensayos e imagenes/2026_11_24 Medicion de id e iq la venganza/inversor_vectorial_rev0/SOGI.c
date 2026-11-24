#include "Renolib.h"

//*********** Structure Init Function ****//
void SOGI_init(SOGI *v) {
	v->Vg[0] = 0.0;
	v->Vg[1] = 0.0;
	v->Vg[2] = 0.0;
	v->alpha[0] = 0.0;
	v->alpha[1] = 0.0;
	v->alpha[2] = 0.0;
	v->beta[0] = 0.0;
	v->beta[1] = 0.0;
	v->beta[2] = 0.0;
}

void SOGI_Update(SOGI *v, float k, float w, float dT) {
	float fs = 1/dT;
	float fs_sqr = fs*fs;
	float w_sqr = w*w;
	float m1 = 4*fs_sqr+2*k*w*fs+w_sqr;
	float m2 = 2*w_sqr-8*fs_sqr;
	float m3 = 4*fs_sqr-2*k*w*fs+w_sqr;
	v->alpha_coeff[0] = 2*k*w*fs/m1;
	v->alpha_coeff[1] = 0.0;
	v->alpha_coeff[2] = -v->alpha_coeff[0];
	v->alpha_coeff[3] = -m2/m1;
	v->alpha_coeff[4] = -m3/m1;
	v->beta_coeff[0] = k*w_sqr/m1;
	v->beta_coeff[1] = 2*v->beta_coeff[0];
	v->beta_coeff[2] = v->beta_coeff[0];
	v->beta_coeff[3] = v->alpha_coeff[3];
	v->beta_coeff[4] = v->alpha_coeff[4];
}




