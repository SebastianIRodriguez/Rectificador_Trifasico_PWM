#include <Renolib.h>

//*********** Structure Init Function ****//
void SIGNAL_3Ph_init(SIGNAL_3Ph *v, float K) {
    v->theta = 0.0;
    v->K = K;
    v->a = 0.0;
    v->b = 0.0;
    v->c = 0.0;
}
