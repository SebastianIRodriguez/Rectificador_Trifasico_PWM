#ifndef RENOLIB_INCLUDE_SIGNAL_1PH_
#define RENOLIB_INCLUDE_SIGNAL_1PH_

//*********** Structure Definition ********//
typedef struct{
    float theta;
    float K;
    float out;
}SIGNAL_1Ph;

//*********** Function Declarations *******//
void SIGNAL_1Ph_init(SIGNAL_1Ph *v, float K);

//*********** Macro Definition ***********//
#define SIGNAL_1Ph_MACRO(v)                                 \
	if (v.theta > TWO_PI)								    \
		v.out = sin((v.theta - TWO_PI));					\
	else if (v.theta < 0)                                   \
	    v.out = sin((v.theta + TWO_PI));                    \
    else                                                    \
        v.out = sin(v.theta);
	
#endif /* RENOLIB_INCLUDE_SIGNAL_1PH_ */
