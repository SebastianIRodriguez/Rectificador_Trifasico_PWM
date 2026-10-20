#include <Renolib.h>

//*********** Structure Init Function ****//
void SIGNAL_1Ph_init(SIGNAL_1Ph *v, float K) {
    v->theta = 0.0;
    v->K = K;
    v->out = 0.0;
}
