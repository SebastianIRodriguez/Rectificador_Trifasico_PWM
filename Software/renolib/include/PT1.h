#ifndef PT1_H
#define PT1_H

//*********** Structure Definition ********//
typedef struct{
	float in;
	float K;
	float T;
	float x;
	float dT;
	float out;
}PT1;

//*********** Function Declarations *******//
void PT1_init(PT1 *v, float K, float T, float dT);

//*********** Macro Definition ***********//
#define PT1_MACRO(v) 					            \
    v.x = v.x + (v.dT/v.T)*(v.in-v.x);              \
	v.out = v.x;                                    \

#endif /* PI_CONTROLLER_H_ */
