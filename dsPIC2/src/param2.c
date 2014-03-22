#include "Datatypes.h"

INT16S a,b;
INT16S table[4];

void function1(INT16S var11, INT16S var12, INT16S tab[4]){

	tab[0] = var11;
	tab[1] = var12;
	tab[2] = var11 + var12;
	tab[3] = (var11 + var12)/2;
	return;
}

void swapnum(INT16S *i, INT16S *j) {
  	INT16S temp;
	temp = *i;
 	*i = *j;
  	*j = temp;
}

void main(void){
	a=20145;
	b=1133;
	function1(a,b,table);
	swapnum(&a,&b);
}
