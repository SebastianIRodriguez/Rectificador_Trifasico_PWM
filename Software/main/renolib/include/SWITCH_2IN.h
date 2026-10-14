#ifndef RENOLIB_INCLUDE_SWITCH_2IN_H_
#define RENOLIB_INCLUDE_SWITCH_2IN_H_

//*********** Structure Definition ********//
typedef struct{
    float in0, in1;
    int sel;
    float out;
}SWITCH_2IN;

//*********** Function Declarations *******//
void SWITCH_2IN_init(SWITCH_2IN *v, int sel);

//*********** Macro Definition ***********//
#define SWITCH_2IN_MACRO(v)                                 \
    switch(v.sel) {                                         \
        case 0:                                             \
            out = in0;                                      \
        case 1:                                             \
            out = in1;                                      \
        default:                                            \
            out = 0.0;                                      \
}

#endif /* RENOLIB_INCLUDE_SWITCH_2IN_H_ */
