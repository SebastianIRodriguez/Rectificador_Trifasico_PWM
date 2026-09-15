#include <Renolib.h>

//*********** Structure Init Function ****//
void SWITCH_2IN_init(SWITCH_2IN *v, int sel) {
    v->sel = sel;
    v->in0 = 0.0;
    v->in1 = 0.0;
    v->out = 0.0;
}
