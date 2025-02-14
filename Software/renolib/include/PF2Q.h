#ifndef RENOLIB_INCLUDE_PF2Q_H_
#define RENOLIB_INCLUDE_PF2Q_H_

//*********** Structure Definition ********//
typedef struct{
    float pf;
    float q, p;
} PF2Q;

//*********** Function Declarations *******//
void PF2Q_init(PF2Q *v);

//*********** Macro Definition ***********//
#define Q2PF_MACRO(v)                                       \
    if (v.q >= 0) v.pf = v.p/sqrt(v.q*v.q+v.p*v.p);         \
    else v.pf = -v.p/sqrt(v.q*v.q+v.p*v.p);                 \

#define PF2Q_MACRO(v)                                       \
    if (v.pf >= 0) v.q = v.p*sqrt(1/(v.pf*v.pf)-1);         \
    else v.q = -v.p*sqrt(1/(v.pf*v.pf)-1);                  \

#endif /* RENOLIB_INCLUDE_PF2Q_H_ */
