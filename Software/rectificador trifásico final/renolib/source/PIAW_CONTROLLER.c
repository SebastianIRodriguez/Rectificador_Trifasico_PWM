#include <Renolib.h>

//*********** Structure Init Function ****//
void PIAW_CONTROLLER_init(PIAW_CONTROLLER *v, float Kp, float Ki, float max, float min, float dT) {
	v->sp = 0.0;
    v->fb = 0.0;
    v->out = 0.0;
    v->Kp = Kp;
    v->Ki = Ki;
    v->dT = dT;
    v->max = max;
    v->min = min;
    v->fzi = 0;
    v->fzo = 0;
    v->x = 0.0;
}
