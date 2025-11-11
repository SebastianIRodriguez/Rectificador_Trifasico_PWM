#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

//*********** Structure Definition ********//
typedef struct{
	float sp;
	float fb;
	float out;
	float Kp;
	float Ki;
	float x;
	float dT;
}PI_CONTROLLER;

//*********** Function Declarations *******//
void PI_CONTROLLER_init(PI_CONTROLLER *v, float Kp, float Ki, float dT);

//*********** Macro Definition ***********//
#define PI_CONTROLLER_MACRO(v) 									\
	v.x = v.x + (v.sp-v.fb)*v.Ki*v.dT;					        \
	v.out = v.x + (v.sp-v.fb)*v.Kp; 							\

#endif /* PI_CONTROLLER_H_ */
