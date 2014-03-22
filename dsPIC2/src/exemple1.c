#include "DataTypes.h"

INT8U a,b,c,d;
INT16U e,f;

void main(void){
	a=201;
	b=11;
	c=5;
	f=0;
	
	d=(a*b)/c;
	d=(a/c)*b;
	
 	f=a*b/c; 
	f=(a*b)/c;
	f=(a/c)*b;
	f=a*(b/c);

	a=201;
	b=2;
	c=5;
	f=0;

	f=(a*b)/c;
	d=(a/c)*b;
}