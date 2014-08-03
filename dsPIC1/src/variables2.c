#include "Datatypes.h"
#include <p33FJ256GP710.h>

register INT8U *glob1 asm ("w8");

int main (void)
{
INT8U a = 300;
INT8S b = -150;

INT16U c = 0x43210;
INT16S d = -35000;

INT32U e = 129856789876543210;
INT32S f = -128423;

INT64U g = 0x2FEDCBA9876543210;
INT64S h = -0x123456789ABCDEF2;

FP32 i = 1.27E+50;
FP64 j = 8.76543E-70;

glob1=300;

}
