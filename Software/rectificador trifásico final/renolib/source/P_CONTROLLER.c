#include <Renolib.h>

//*********** Structure Init Function ****//
void P_CONTROLLER_init(P_CONTROLLER *v, float Kp) {
	v->sp = 0.0;
    v->fb = 0.0;
    v->out = 0.0;
    v->Kp = Kp;
}
