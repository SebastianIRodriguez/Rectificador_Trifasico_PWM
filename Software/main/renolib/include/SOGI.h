#ifndef SOGI_H_
#define SOGI_H_

//*********** Structure Definition ********//
typedef struct {
	float Vg[3];
	float alpha[3];
	float beta[3];
	float alpha_coeff[5];
	float beta_coeff[5];
}SOGI;

//*********** Function Declarations *******//
void SOGI_init(SOGI *v);
void SOGI_Update(SOGI *v, float k, float w, float dT);

//*********** Macro Definition ***********//
#define SOGI_MACRO(v) 																																	\
	v.alpha[0] = v.Vg[0]*v.alpha_coeff[0]+v.Vg[1]*v.alpha_coeff[1]+v.Vg[2]*v.alpha_coeff[2]+v.alpha[1]*v.alpha_coeff[3]+v.alpha[2]*v.alpha_coeff[4];		\
	v.beta[0] = v.Vg[0]*v.beta_coeff[0]+v.Vg[1]*v.beta_coeff[1]+v.Vg[2]*v.beta_coeff[2]+v.beta[1]*v.beta_coeff[3]+v.beta[2]*v.beta_coeff[4];				\																								\
	v.alpha[2] = v.alpha[1];																																\
	v.alpha[1] = v.alpha[0];																																\
	v.beta[2] = v.beta[1];																																	\
	v.beta[1] = v.beta[0];																																	\
	v.Vg[2] = v.Vg[1];																																		\
	v.Vg[1] = v.Vg[0];

#endif /* SOGI_FT_H_ */
