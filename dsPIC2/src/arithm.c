#include "DataTypes.h"
#include "p33FJ256GP710.h"

INT8U a,b,c,d;
INT8S s1,s2,s3;
INT16U e,f,g;
INT32U h,i;
INT64U j;

void main(void){

// usigned addition
	a = 64;
	b = 128;
	e = 256;
	f = 512;

	c = a+b; //INT8U = INT8U + INT8U
	g = a+b; //INT16U = INT8U + INT8U
	
	g = e+f; //INT16U = INT16U + INT16U
	g = e-f; //INT16U = INT16U - INT16U

	e = 63457;
	f = 5478;
	g = e+f; //INT16U = INT16U + INT16U
	if(SRbits.C) g=0xFFFF;
	
// signed addition	
	s1 = 96;
	s2 = 40;
	s3 = s1 + s2; //INT8S = INT8S + INT8S
	a = SR; // save LSByte of Status
	if(a==0x0C)s3 = 127; //Overflow (bit2) and Negative (bit3)
	if(a==0x05)s3 = -128;//Overflow (bit2) and Carry (bit1)
	
	s1 = -96;
	s2 = -40;
	s3 = s1 + s2; //INT8S = INT8S + INT8S
	a = SR; // save LSByte of Status
	if(a==0x0C)s3 = 127; //Overflow (bit2) and Negative (bit3)
	if(a==0x05)s3 = -128;//Overflow (bit2) and Carry (bit1)
	a = 8;
	b = 64;
	e = 12000;
	f = 80;
	i = 100000;

// multiplications
	c = a*b; 					//INT8U = INT8U * INT8U
	g = a*b; 					//INT16U = INT8U * INT8U
	g = e*f; 					//INT16U = INT16U * INT16U
	h = e*f; 					//INT32U = INT16U * INT16U
	h = (INT32U)e*(INT32U)f; 	//typecast before multiplying
	j = h*i; 					//INT64U = INT32U * INT32U
	j = (INT64U)h*(INT64U)i;	//typecast before multiplying
}
