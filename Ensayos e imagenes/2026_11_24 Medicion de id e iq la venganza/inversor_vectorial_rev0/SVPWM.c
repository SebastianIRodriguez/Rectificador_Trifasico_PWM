#include <Renolib.h>

//*********** Structure Init Function ****//
void SVPWM_init(SVPWM *v, float Ts){
    v->Ts = Ts;
    v->V = 0.0;
    v->theta = 0.0;
    v->theta_rel = 0.0;
    v->E = 1.0;
    v->Ta = 0.0;
    v->Tb = 0.0;
    v->Half_T0 = 0.0;
    v->Vma = 0;
    v->Vmb = 0;
    v->Vmc = 0;
}
