#ifndef P_CONTROLLER_H
#define P_CONTROLLER_H

//*********** Structure Definition ********//
typedef struct{
	float sp;
	float fb;
	float out;
	float Kp;
}P_CONTROLLER;

//*********** Function Declarations *******//
void P_CONTROLLER_init(P_CONTROLLER *v, float Kp);

//*********** Macro Definition ***********//
#define P_CONTROLLER_MACRO(v) 									\
	v.out = v.Kp * (v.sp - v.fb);

#endif /* PI_CONTROLLER_H_ */
