#include "DataTypes.h"

INT8U a,b,c;
INT16U d,e;
FP32 f1,f2,f3;

void main(void){

// classical method
f1 = 8.431;
f2 = 6.764;
f3 = f1*f2;
c = (INT8U)f3;

// 2nd method
a = 135;  	//8.431*16
b = 108;	//6.764*16
d = (INT16U)a*(INT16U)b;
c = d >>8; //division by 256=16*16
;

// 3rd method
a = 135;  	//8.431*16
b = 108;	//6.764*16
d = (INT16U)a*(INT16U)b;
c = (d+128)>>8; //division by 256=16*16
;
}