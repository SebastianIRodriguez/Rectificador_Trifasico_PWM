#include <Renolib.h>

//*********** Structure Init Function ****//
void SWITCH_2IN_3Ph_init(SWITCH_2IN_3Ph *v, int sel) {
    v->sel = sel;
    v->in0[0] = 0.0;
    v->in0[1] = 0.0;
    v->in0[2] = 0.0;
    v->in1[0] = 0.0;
    v->in1[1] = 0.0;
    v->in1[2] = 0.0;
    v->out[0] = 0.0;
    v->out[1] = 0.0;
    v->out[2] = 0.0;
}
