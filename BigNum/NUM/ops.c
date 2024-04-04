#pragma once
#include"ops.h"


void getBigNumByStr(const char* str, BigNum* bignum)
{
	char* dupstr = str;
	if (dupstr[0] == '-')
	{
		bignum->sign = -1;
		dupstr++;
	}
	else
		bignum->sign = 1;


	int len = strlen(dupstr);
	bignum->size = len / 8;
	if (len % 8 != 0)
		bignum->size += 1;
	bignum->number = (unsigned int*)malloc(sizeof(unsigned int) * bignum->size);
	for (int i = 0; i < bignum->size; i++)
		bignum->number[i] = 0;
	unsigned int pow = 1;

	for (int i = len - 1; i >= 0; i--)
	{
		int tmp = 0;
		if (dupstr[i] >= 'A' && dupstr[i] <= 'F')
			tmp = 10 + dupstr[i] - 'A';
		else
			if (dupstr[i] >= '0' && dupstr[i] <= '9')
				tmp = dupstr[i] - '0';
			else
			{
				printf_s("ERROR:INCORRECT SYMBOL DETECTED:%c\n", dupstr[i]);
				bignum->sign = 0;
				return;
			}

		bignum->number[((len - i - 1) / 8)] += tmp * pow;
		pow *= 16;
		if ((len - i) % 8 == 0)
			pow = 1;
	}
	return;
}


void printBigNum(BigNum* bignum)
{
	int hexLen = 8;
	char* hex = (char*)malloc(sizeof(char) * (hexLen + 1));
	if (bignum->sign == -1)
		printf_s("-");
	for (int i = bignum->size - 1; i >= 0; i--)
	{
		for (int k = 0; k < hexLen; k++)
			hex[k] = '0';
		hex[hexLen] = '\0';

		unsigned int tmp = bignum->number[i];
		int iterator = 0;
		if (tmp == 0 && bignum->size == 1)
			printf_s("0");
		else
		{
			while (tmp > 0)
			{
				int tmpDec = tmp % 16;
				tmp /= 16;
				if (tmpDec < 10)
					hex[iterator] = '0' + tmpDec;
				else
					hex[iterator] = 'A' + tmpDec - 10;
				iterator++;
			}
			if (i == bignum->size - 1)
				iterator = iterator - 1;
			else
				iterator = hexLen - 1;
			for (; iterator >= 0; iterator--)
				printf_s("%c", hex[iterator]);
		}
	}
	printf_s("\n\n");
	return;
}

int greaterBigNum(BigNum* bignum1, BigNum* bignum2)
{
	if (bignum1->size > bignum2->size)
		return 1;
	if (bignum2->size > bignum1->size)
		return -1;

	for (int i = bignum1->size - 1; i >= 0; i--)
	{
		if (bignum1->number[i] > bignum2->number[i])
			return 1;
		if (bignum2->number[i] > bignum1->number[i])
			return -1;
	}
	return 0;
}

void addBigNum(BigNum* bignum1, BigNum* bignum2, BigNum* res)
{
	int num1SignHolder = bignum1->sign;
	int num2SignHolder = bignum2->sign;
	unsigned int* resTmp;

	if (bignum1->sign != bignum2->sign)
	{
		switch (greaterBigNum(bignum1, bignum2))
		{
		case 1://a-b -a+b
			bignum2->sign = bignum1->sign;
			res->sign = bignum1->sign;
			subBigNum(bignum1, bignum2, res);
			bignum1->sign = num1SignHolder;
			bignum2->sign = num2SignHolder;
			return;

		case -1:
			bignum1->sign = bignum2->sign;
			res->sign = bignum2->sign;
			subBigNum(bignum2, bignum1, res);
			bignum1->sign = num1SignHolder;
			bignum2->sign = num2SignHolder;
			return;
		default:
			res->size = 1;
			res->sign = 1;
			resTmp = (unsigned int*)malloc(sizeof(unsigned int));
			resTmp[0] = 0;
			res->number = resTmp;
			return;
		}
	}
	else
		res->sign = bignum1->sign;

	if (bignum1->size > bignum2->size)
	{
		res->size = bignum1->size + 1;
		resTmp = (unsigned int*)malloc(sizeof(unsigned int) * (res->size));
	}
	else
	{
		res->size = bignum2->size + 1;
		resTmp = (unsigned int*)malloc(sizeof(unsigned int) * (res->size));
	}

	for (int i = 0; i < res->size; i++)
		resTmp[i] = 0;

	int over = 0;

	for (int i = 0; i < res->size; i++)
	{
		unsigned int tmpDec1;
		unsigned int tmpDec2;

		if (i >= bignum1->size)
			tmpDec1 = 0;
		else
			tmpDec1 = bignum1->number[i];
		if (i >= bignum2->size)
			tmpDec2 = 0;
		else
			tmpDec2 = bignum2->number[i];

		unsigned int pow = 1;

		for (int index = 7; index >= 0; index--)
		{
			int tmp1 = tmpDec1 % 16;
			int tmp2 = tmpDec2 % 16;
			tmpDec1 /= 16;
			tmpDec2 /= 16;
			int sum = tmp1 + tmp2 + over;
			if (sum >= 16)
			{
				sum -= 16;
				over = 1;
			}
			else
				over = 0;
			resTmp[i] += pow * sum;
			pow *= 16;
		}
	}
	if (resTmp[res->size - 1] == 0)
		res->size -= 1;
	res->number = resTmp;
	return;
}

void subBigNum(BigNum* bignum1, BigNum* bignum2, BigNum* res)
{
	int num1SignHolder = bignum1->sign;
	int num2SignHolder = bignum2->sign;
	unsigned int* resTmp;
	unsigned int* tmpNum;
	size_t tmpSize;

	if (bignum1->sign != bignum2->sign)
	{
		switch (greaterBigNum(bignum1, bignum2))
		{
		case 1:// a-(-b)=+  -a-b=-
			bignum2->sign = bignum1->sign;
			res->sign = bignum1->sign;
			addBigNum(bignum1, bignum2, res);
			bignum1->sign = num1SignHolder;
			bignum2->sign = num2SignHolder;
			return;
		case -1:// a-(-b) = + -a-b=-
			bignum2->sign = bignum1->sign;
			res->sign = bignum1->sign;
			addBigNum(bignum2, bignum1, res);
			bignum1->sign = num1SignHolder;
			bignum2->sign = num2SignHolder;
			return;
		case 0:// a-(-b)=+   -a-b=-
			bignum2->sign = bignum1->sign;
			res->sign = bignum1->sign;
			addBigNum(bignum1, bignum2, res);
			bignum1->sign = num1SignHolder;
			bignum2->sign = num2SignHolder;
			return;
		}
	}

	switch (greaterBigNum(bignum1, bignum2))
	{
	case 1://-a-(-b)=-    a-b=+
		res->sign = bignum1->sign;
		res->size = bignum1->size;
		break;
	case -1://-a-(-b)=+ a-b=-
		res->sign = bignum2->sign * (-1);
		res->size = bignum2->size;

		tmpNum = bignum1->number;
		bignum1->number = bignum2->number;
		bignum2->number = tmpNum;

		tmpSize = bignum1->size;
		bignum1->size = bignum2->size;
		bignum2->size = tmpSize;
		break;
	case 0://a==b
		res->sign = 1;
		res->size = 1;
		resTmp = (unsigned int*)malloc(sizeof(unsigned int) * (res->size));
		resTmp[0] = 0;
		res->number = resTmp;
		return;
	}

	int borrow = 0;
	resTmp = (unsigned int*)malloc(sizeof(unsigned int) * (res->size));

	for (int i = 0; i < res->size; i++)
		resTmp[i] = 0;

	for (int i = 0; i < res->size; i++)
	{
		unsigned int tmpDec1;
		unsigned int tmpDec2;

		if (i >= bignum1->size)
			tmpDec1 = 0;
		else
			tmpDec1 = bignum1->number[i];

		if (i >= bignum2->size)
			tmpDec2 = 0;
		else
			tmpDec2 = bignum2->number[i];


		unsigned int pow = 1;

		for (int iterator = 7; iterator >= 0; iterator--)
		{
			int tmp1 = tmpDec1 % 16;
			int tmp2 = tmpDec2 % 16;
			tmpDec1 /= 16;
			tmpDec2 /= 16;
			int sub = tmp1 - tmp2 - borrow;
			if (sub < 0)
			{
				borrow = 1;
				sub += 16;
			}
			else
				borrow = 0;
			resTmp[i] += pow * sub;
			pow *= 16;
		}
	}

	int realLen = res->size - 1;
	while (realLen > 0 && resTmp[realLen] == 0)
	{
		realLen--;
		res->size--;
	}
	res->number = resTmp;
	return;
}