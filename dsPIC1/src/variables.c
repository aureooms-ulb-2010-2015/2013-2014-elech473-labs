#include "Datatypes.h"
#include <p33FJ256GP710.h>

register INT8U *glob1 asm ("w8");

int main (void)
{
INT8U a = 180;
INT8S b = -105;

INT16U c = 0x3210;
INT16S d = -30000;

INT32U e = 129856;
INT32S f = -128423;

INT64U g = 0xFEDCBA9876543210;
INT64S h = -0x123456789ABCDF0;

FP32 i = 1.27E+33;
FP64 j = 8.76543E-18;

glob1=132;

}
