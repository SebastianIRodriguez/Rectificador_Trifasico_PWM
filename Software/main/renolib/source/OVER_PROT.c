#include <Renolib.h>

//*********** Structure Init Function ****//
void OVER_PROT_init(OVER_PROT *v, float dT, float pickup, float delay, float dropout) {
    v->dT = dT;
    v->pup = pickup;
    v->dly = delay;
    v->drp = dropout;
    v->st = 1;
    v->rst = 0;
    v->tp = 0.0;
    v->td = 0.0;
    v->out = 0;
}
