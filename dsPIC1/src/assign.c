#include "DataTypes.h"
#include "p33FJ256GP710.h"


INT8U int8var;
INT16U  int16var;
INT32U int32var;
FP32 floatvar;

void main(void){

	int8var = 128;
	int16var = int8var;

	int16var = 32002;
	int8var = int16var;
	int8var = int16var / 256;
	int8var = int16var >> 8;


	int32var = int16var;
	
	int32var = 0x76543210;
	int16var = int32var;

	floatvar = int16var;

	floatvar = int32var;	
}