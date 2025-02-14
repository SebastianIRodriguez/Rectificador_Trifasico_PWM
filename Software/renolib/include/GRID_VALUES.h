#ifndef RENOLIB_INCLUDE_GRID_VALUES_H_
#define RENOLIB_INCLUDE_GRID_VALUES_H_

typedef struct{
    float theta;
    float f;
    float Ufn;
    float Ull;
    float I;
    float P;
    float Q;
    float S;
} GRID_VALUES;

void GRID_VALUES_init(GRID_VALUES *v);

#endif /* RENOLIB_INCLUDE_GRID_VALUES_H_ */
