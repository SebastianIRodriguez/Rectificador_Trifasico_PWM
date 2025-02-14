#ifndef RENOLIB_INCLUDE_PO_H_
#define RENOLIB_INCLUDE_PO_H_
#include "Aplicacion.h"
#include "Integrator.h"

typedef struct {
    float h;
    float T;
    float timer;
    float Pin;
    INTEGRATOR Paccum;
    float P[2];
    int Enable;
    int Inc[2], Dec[2];
} PO;

void PO_init(PO *v, float h, float T);

#define PO_MACRO(v)                                                                         \
    v.timer = v.timer + v.h;                                                                \
    v.Paccum.in = v.Pin;                                                                    \
    INTEGRATOR_MACRO(v.Paccum);                                                             \
    if (v.timer >= v.T) {                                                                   \
        v.P[2] = v.P[1];                                                                     \
        v.P[1] = v.Paccum.x/v.timer;                                                        \
        v.timer = 0.0;                                                                      \
        v.Paccum.x = 0.0;                                                                   \
        if (v.Enable) {                                                                     \
            if (v.P[1] > v.P[2]) {                                                          \
                v.Inc[1] = v.Inc[2];                                                        \
                v.Dec[1] = v.Dec[2];                                                        \
            } else {                                                                        \
                v.Inc[1] = v.Dec[2];                                                        \
                v.Dec[1] = v.Inc[2];                                                        \
            }                                                                               \
            v.Inc[2] = v.Inc[1];                                                            \
            v.Dec[2] = v.Dec[1];                                                            \
        }                                                                                   \
    } else {                                                                                \
        v.Inc[1] = FALSE;                                                                   \
        v.Dec[1] = FALSE;                                                                   \
    }                                                                                       \


#endif /* RENOLIB_INCLUDE_PO_H_ */
