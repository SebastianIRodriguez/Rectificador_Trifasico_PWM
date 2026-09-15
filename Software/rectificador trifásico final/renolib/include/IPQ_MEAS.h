#ifndef RENOLIB_INCLUDE_IPQ_MEAS_H_
#define RENOLIB_INCLUDE_IPQ_MEAS_H_

//*********** Structure Definition ********//
typedef struct{
	float ia, ib, ic;
	float va, vb, vc;
	float p, q;
}IPQ_MEAS;

//*********** Function Declarations *******//
void IPQ_MEAS_init(IPQ_MEAS *v);

//*********** Macro Definition ***********//
#define IPQ_MEAS_MACRO(v) 									                    \
	v.p = v.va*v.ia+v.vb*v.ib+v.vc*v.ic;                	                    \
	v.q = ISQRT3*( (v.vb-v.vc)*v.ia + (v.vc-v.va)*v.ib + (v.va-v.vb)*v.ic);     \

#endif /* RENOLIB_INCLUDE_IPQ_MEAS_H_ */
