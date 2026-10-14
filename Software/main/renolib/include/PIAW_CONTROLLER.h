#ifndef PIAW_CONTROLLER_H
#define PIAW_CONTROLLER_H

//*********** Structure Definition ********//
typedef struct{
	float sp;
	float fb;
	float out;
	float Kp;
	float Ki;
	float max;
	float min;
	unsigned int fzi;
	unsigned int fzo;
	float x;
	float dT;
}PIAW_CONTROLLER;

//*********** Function Declarations *******//
void PIAW_CONTROLLER_init(PIAW_CONTROLLER *v, float Kp, float Ki, float max, float min, float dT);

//*********** Macro Definition ***********//
#define PIAW_CONTROLLER_MACRO(v) 									\
	if (v.fzi == 0) {												\
		v.x = v.x + (v.sp-v.fb)*v.Ki*v.dT;					        \
		if (v.x > v.max) v.x = v.max;							    \
		else if (v.x < v.min) v.x = v.min;					        \
		v.out = v.x + (v.sp-v.fb)*v.Kp; 							\
		if (v.out > v.max) {										\
			v.out = v.max;											\
			v.fzo = 1; }											\
		if (v.out < v.min) {										\
			v.out = v.min;											\
			v.fzo = 1; }											\	
	}																\

#endif /* PIAW_CONTROLLER_H_ */
