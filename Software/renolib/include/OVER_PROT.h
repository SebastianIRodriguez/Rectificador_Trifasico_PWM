#ifndef RENOLIB_INCLUDE_OVER_PROT_H_
#define RENOLIB_INCLUDE_OVER_PROT_H_

//*********** Structure Definition ********//
typedef struct{
    float in;
    float dT;
    float pup, dly, drp;
    float tp, td;
    int rst, st, out;
} OVER_PROT;

//*********** Function Declarations *******//
void OVER_PROT_init(OVER_PROT *v, float dT, float pickup, float delay, float dropout);

//*********** Macro Definition ***********//
#define OVER_PROT_MACRO(v)                                          \
    switch (v.st) {                                                 \
        case 1:                                                     \
            v.tp = 0.0;                                             \
            v.td = 0.0;                                             \
            v.out = 0;                                              \
            if (v.in >= v.pup) v.st = 2;                            \
            break;                                                  \
        case 2:                                                     \
            v.tp = v.tp + v.dT;                                     \
            v.td = 0.0;                                             \
            v.out = 0;                                              \
            if (v.in < 0.98*v.pup) v.st = 3;                        \
            if (v.tp >= v.dly) v.st = 4;                            \
            break;                                                  \
        case 3:                                                     \
            v.tp = v.tp + v.dT;                                     \
            v.td = v.td + v.dT;                                     \
            v.out = 0;                                              \
            if (v.in > v.pup) v.st = 2;                             \
            if (v.td >= v.drp) v.st = 1;                            \
            break;                                                  \
        case 4:                                                     \
            v.tp = 0.0;                                             \
            v.td = 0.0;                                             \
            v.out = 1;                                              \
            if (v.rst == TRUE && v.in < 0.98*v.pup) {               \
                v.st = 1;                                           \
                v.rst = 0;                                          \
            } break;                                                \
    }                                                               \
    

#endif /* RENOLIB_INCLUDE_OVER_PROT_H_ */
