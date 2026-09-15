#include <Renolib.h>

void DC_init(DC *dc) {
    dc->v = 0.0;
    dc->i = 0.0;
    dc->r = 0.0;
    dc->p = 0.0;
}