#ifndef RENOLIB_INCLUDE_SVPWM_H_
#define RENOLIB_INCLUDE_SVPWM_H_

//*********** Structure Definition ********//
typedef struct{
    float V, theta;
    float theta_rel;
    float Ts;
    float E;
    float Ta, Tb, Half_T0;
    int N;
    float Vma, Vmb, Vmc;
}SVPWM;

//*********** Function Declarations *******//
void SVPWM_init(SVPWM *v, float Ts);

//*********** Macro Definition ***********//
#define SVPWM_MACRO(v)                                   \
    v.N = (int) (v.theta/ONE_THIRDS_PI);                 \
    if (v.N >= 6) v.N = v.N-6;                           \
    v.theta_rel = v.theta-v.N*ONE_THIRDS_PI;             \
    float k = SQRT3*(v.V/v.E)*v.Ts;                      \
    v.Ta = k*sin(ONE_THIRDS_PI-v.theta_rel);             \
    v.Tb = k*sin(v.theta_rel);                           \
    v.Half_T0 = (v.Ts-v.Ta-v.Tb)/2.0;                    \
    switch (v.N) {                                       \
        case 0:                                          \
            v.Vma = (1-(v.Ta+v.Tb+v.Half_T0)/v.Ts);      \
            v.Vmb = (1-(v.Tb+v.Half_T0)/v.Ts);           \
            v.Vmc = (1-v.Half_T0/v.Ts);                  \
            break;                                       \
        case 1:                                          \
            v.Vma = (1-(v.Ta+v.Half_T0)/v.Ts);           \
            v.Vmb = (1-(v.Ta+v.Tb+v.Half_T0)/v.Ts);      \
            v.Vmc = (1-v.Half_T0/v.Ts);                  \
            break;                                       \
        case 2:                                          \
            v.Vma = (1-v.Half_T0/v.Ts);                  \
            v.Vmb = (1-(v.Ta+v.Tb+v.Half_T0)/v.Ts);      \
            v.Vmc = (1-(v.Tb+v.Half_T0)/v.Ts);           \
            break;                                       \
        case 3:                                          \
            v.Vma = (1-v.Half_T0/v.Ts);                  \
            v.Vmb = (1-(v.Ta+v.Half_T0)/v.Ts);           \
            v.Vmc = (1-(v.Ta+v.Tb+v.Half_T0)/v.Ts);      \
            break;                                       \
        case 4:                                          \
            v.Vma = (1-(v.Tb+v.Half_T0)/v.Ts);           \
            v.Vmb = (1-v.Half_T0/v.Ts);                  \
            v.Vmc = (1-(v.Ta+v.Tb+v.Half_T0)/v.Ts);      \
            break;                                       \
        case 5:                                          \
            v.Vma = (1-(v.Ta+v.Tb+v.Half_T0)/v.Ts);      \
            v.Vmb = (1-v.Half_T0/v.Ts);                  \
            v.Vmc = (1-(v.Ta+v.Half_T0)/v.Ts);           \
            break;                                       \
    }                                                    \

#endif /* RENOLIB_INCLUDE_SVPWM_H_ */
