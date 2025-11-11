#ifndef RENOLIB_INCLUDE_LIMITS_H_
#define RENOLIB_INCLUDE_LIMITS_H_

/*********** Structure Definitions ***********/
typedef struct{
    float in;
    float out;
    float high;
    float low;
}LIMITS;

//*********** Function Declarations *******//
void LIMITS_init(LIMITS *v, float max, float min);

//*********** Macro Definition ***********//
#define LIMITS_MACRO(v)                 \
    if (v.in>v.high) v.out=v.high;      \
    else if (v.in<v.low) v.out=v.low;   \
    else v.out = v.in;                  \

#endif /* RENOLIB_INCLUDE_LIMITS_H_ */
