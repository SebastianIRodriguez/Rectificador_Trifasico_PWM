#ifndef RENOLIB_INCLUDE_SIGNAL_3PH_
#define RENOLIB_INCLUDE_SIGNAL_3PH_

//*********** Structure Definition ********//
typedef struct{
    float theta;
    float K;
    float a, b, c;
}SIGNAL_3Ph;

//*********** Function Declarations *******//
void SIGNAL_3Ph_init(SIGNAL_3Ph *v, float K);

//*********** Macro Definition ***********//
#define SIGNAL_3Ph_MACRO(v)                                                         \
	if (v.theta > TWO_PI)															\
		v.a = v.K*cos((v.theta - TWO_PI));											\
    else                                                                            \
        v.a = v.K*cos(v.theta);                                                     \
	if (v.theta + FOUR_THIRDS_PI > TWO_PI)											\
		v.b = v.K*cos((v.theta + FOUR_THIRDS_PI - TWO_PI));							\
    else                                                                            \
        v.b = v.K*cos((v.theta + FOUR_THIRDS_PI));                                  \
	if (v.theta + TWO_THIRDS_PI > TWO_PI)											\
		v.c = v.K*cos((v.theta + TWO_THIRDS_PI - TWO_PI));							\
    else                                                                            \
        v.c = v.K*cos((v.theta + TWO_THIRDS_PI));

#endif /* RENOLIB_INCLUDE_SIGNAL_3PH_ */
