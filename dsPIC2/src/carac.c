#include <p33FJ256GP710.h>
#include <stdio.h>
#include "Datatypes.h"

const char a[]="hello";

void main (void){
	char m[] = "world";
	char n[6];
	char p[6];

	INT8U i;
	for (i=0;i<5;i++){
		n[i] = m[i];
	}
	n[i]=0;


	INT16U j;
	for (j=0;j<5;j++){
		p[j]=a[j];
	}
	p[j]=0; 
}

