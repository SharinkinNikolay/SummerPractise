#pragma once
#include"ops.h"



int main(int argc,char* argv[])
{
	if (argc != 3) 
	{
		printf_s("ERROR: \t1 OR 2 ARGUMENTS ARE MISSING");
		return -1;
	}

	BigNum bignum1;
	BigNum bignum2;
	BigNum bignum3;
	BigNum bignum4;

	getBigNumByStr(argv[1], &bignum1);
	getBigNumByStr(argv[2], &bignum2);

	if (bignum1.sign == 0 || bignum2.sign == 0)
		return -1;

	printf_s("big number 1:\t");
	printBigNum(&bignum1);
	printf_s("big number 2:\t");
	printBigNum(&bignum2);

	addBigNum(&bignum1, &bignum2, &bignum3);
	subBigNum(&bignum1, &bignum2, &bignum4);

	printf_s("big number 1+2:\t");
	printBigNum(&bignum3);

	printf_s("big number 1-2:\t");
	printBigNum(&bignum4);
	return 0;
}

