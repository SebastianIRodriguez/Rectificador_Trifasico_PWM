#include <Renolib.h>

//*********** Structure Init Function ****//
void LIMITS_init(LIMITS *v, float max, float min) {
    v->in = 0.0;
    v->out = 0.0;
    v->high = max;
    v->low = min;
}
