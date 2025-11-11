#ifndef RENOLIB_INCLUDE_SWITCH_2IN_3PH_H_
#define RENOLIB_INCLUDE_SWITCH_2IN_3PH_H_

//*********** Structure Definition ********//
typedef struct{
    float in0[3], in1[3];
    int sel;
    float out[3];
}SWITCH_2IN_3Ph;

//*********** Function Declarations *******//
void SWITCH_2IN_3Ph_init(SWITCH_2IN_3Ph *v, int sel);

//*********** Macro Definition ***********//
#define SWITCH_2IN_3Ph_MACRO(v)                             \
    switch(v.sel) {                                         \
        case 0:                                             \
            out[0] = in0[0];                                \
            out[1] = in0[1];                                \
            out[2] = in0[2];                                \
        case 1:                                             \
            out[0] = in1[0];                                \
            out[1] = in1[1];                                \
            out[2] = in1[2];                                \
        default:                                            \
            out[0] = 0.0;                                   \
            out[1] = 0.0;                                   \
            out[2] = 0.0;                                   \
}

#endif /* RENOLIB_INCLUDE_SWITCH_2IN_3PH_H_ */
