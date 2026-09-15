#ifndef RENOLIB_INCLUDE_DC_H_
#define RENOLIB_INCLUDE_DC_H_

typedef struct {
    float v;
    float i;
    float r;
    float p;
} DC;

void DC_init(DC *v);

#define DC_MACRO(v)                                                                                 \
    v.Int.in = v.in*v.u2;																			\
	INTEGRATOR_MACRO(v.Int);																		\
	v.out = v.wn + v.Int.out;																		\

#endif /* RENOLIB_INCLUDE_DC_H_ */
