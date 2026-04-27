#include <Renolib.h>

//*********** Structure Init Function ****//
void PI_CONTROLLER_init(PI_CONTROLLER *v, float Kp, float Ki, float dT) {
	v->sp = 0.0;
    v->fb = 0.0;
    v->out = 0.0;
    v->Kp = Kp;
    v->Ki = Ki;
    v->dT = dT;
    v->x = 0.0;
}
