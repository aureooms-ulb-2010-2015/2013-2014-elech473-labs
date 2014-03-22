#include "DataTypes.h"



INT16U Add3(INT8U var11, INT8U var12, INT16U var13){
	INT16U a=346;
	return(var11 + var12 + var13 + a);
}

INT32U Mul2( INT8U var21, INT8U var22){
	return((INT32U)var21*(INT32U)var22);
}

void main(void){
	INT8U a,b;
	INT16U c,d;
	INT32U e;

	a=201;
	b=11;
	c=53000;
	d=0;
	e=0;
	
	d = Add3(a,b,c);
	e = Mul2(a,b);
		
}