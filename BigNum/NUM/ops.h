#include<stdio.h>
#include<malloc.h>
#include<string.h>


typedef struct
{
	unsigned int* number;
	size_t size;
	int sign;
} BigNum;

void getBigNumByStr(const char* str, BigNum* bignum);
void printBigNum(BigNum* bignum);
int greaterBigNum(BigNum* bignum1, BigNum* bignum2);
void subBigNum(BigNum* bignum1, BigNum* bignum2,BigNum* res);
void addBigNum(BigNum* bignum1, BigNum* bignum2,BigNum* res);