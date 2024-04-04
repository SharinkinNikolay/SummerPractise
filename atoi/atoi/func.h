#include<stdio.h>
#include<malloc.h>
#include<limits.h>
char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";


int pow(int number, int power)
{
	if (power == 0)
		return 1;
	int result = 1;
	for (int i = 0; i < power; i++)
	{
		result = result * number;
	}
	return result;
}
int getIntLen(int number)
{
	if (number < 0)
		number = number * (-1);
	int result = 1;
	while (number > 9)
	{
		result++;
		number /= 10;
	}
	return result;
}
int myStrlen(char* str)
{
	int len = 0;
	while (str[len] != '\0')
		len++;
	return len;
}
int isMultiplyOverflow(int op1, int op2, int res)
{
	if (op1 == 0 || op2 == 0)
		return 0;
	if (op1 != res / op2)
		return -1;
	return 0;
}
int isAddOverflow(int op1, int op2, int res) {
	int c = 0;
	if (op1 > 0 && op2 > 0 && (INT_MAX - op2 < op1) ||
		op1 < 0 && op2 < 0 && (INT_MIN - op2 > op1))
		return 1;
	return 0;
}

int checkNumSys(char* str, int radix, int sign)
{

	int number;
	if (sign == -1)
		number = INT_MIN;
	else
		number = INT_MAX;


	int notationMax = 1;
	if (number == INT_MAX)
	{
		for (int i = 0; number > radix; i++)
		{
			number /= radix;
			notationMax++;
		}
	}
	else
	{
		for (int i = 0; number < -radix; i++)
		{
			number /= radix;
			notationMax++;
		}
	}

	if (myStrlen(str) > notationMax)
		return -1;
	return 0;
}

int str_to_int(const char* str, char** p, int* ret)
{
	*ret = 0;
	if (str[0] == '\0')
	{
		if (p != NULL)
			*p = str;
		return 0;
	}
	char* dupstr = str;
	char sign = 1;
	if (dupstr[0] == '-')
	{
		sign = -1;
		dupstr++;
	}
	int radix = 10;
	if (dupstr[0] == '0' && dupstr[1] == 'x')
	{
		radix = 16;
		dupstr += 2;
	}
	else if (dupstr[0] == '0')
	{
		radix = 8;
		dupstr++;
	}
	else if (dupstr[0] == 'b')
	{
		radix = 2;
		dupstr++;
	}
	int numberStartingIndex = 0;
	for (int i = 0; dupstr[i] == '0'; i++)
		numberStartingIndex++;
	dupstr += numberStartingIndex;

	int numLen = myStrlen(dupstr);
	for (int i = 0; i < numLen; i++)
	{
		char found = 0;
		for (int q = 0; q < radix; q++)
		{
			if (dupstr[i] == alphabet[q])
			{
				found = 1;
				break;
			}
		}

		if (!found)
		{
			if (p != NULL)
				*p =dupstr + i;
			return 0;
		}
	}
	if (checkNumSys(dupstr, radix, sign) == -1)
	{
		if (p != NULL)
			*p = dupstr;
		return 0;
	}
	int numPow = 0;
	int checkOverflow = 0;

	for (int i = numLen - 1; i >= 0; i--)
	{
		int tmp = *ret;
		for (int q = 0; q < radix; q++)
			if (dupstr[i] == alphabet[q])
			{
				int currentPow = (sign)*pow(radix, numPow);
				checkOverflow = q * currentPow;
				if (isMultiplyOverflow(q, currentPow, checkOverflow) != 0)
				{
					*ret = 0;
					if (p != NULL)
						*p = dupstr + i;
					return 0;
				}
				int tmpRet = *ret;
				if (isAddOverflow(tmpRet, q * currentPow, *ret + q * currentPow))
				{
					*ret = 0;
					if (p != NULL)
						*p = dupstr + i;
					return 0;
				}

				*ret += q * currentPow;
				numPow++;
				break;
			}
	}
	return 1;
}


int my_itoa(char* buf, int bufSize, int value, int p)
{
	if (buf == NULL)
		return -1;
	if (p > 62 || p < 2)
		return -1;


	if (bufSize == 0)
	{
		bufSize = getIntLen(value);
		if (value < 0)
			bufSize++;
	}

	int iterator = 0;
	int sign = 1;
	int charsWritten = 0;
	if (value < 0)
	{
		sign = -1;
		value *= -1;
		iterator++;
	}

	char* tempString = (char*)malloc(sizeof(char) * 32);
	for (int i = 0; i < 32; i++)
		tempString[i] = '\0';


	while (value >= 1)
	{
		tempString[iterator] = alphabet[value % p];
		value /= p;
		iterator++;
	}
	while (value <= -1)
	{
		tempString[iterator] = alphabet[-(value % p)];
		value /= p;
		iterator++;
	}
	if (sign < 0)
	{
		tempString[0] = '-';
		for (int a = 1, b = iterator - 1; a < b; a++, b--)
		{
			char ch = tempString[a];
			tempString[a] = tempString[b];
			tempString[b] = ch;
		}
	}
	else
		for (int a = 0, b = iterator - 1; a < b; a++, b--)
		{
			char ch = tempString[a];
			tempString[a] = tempString[b];
			tempString[b] = ch;
		}
	for (int a = 0; a < bufSize && tempString[a] != '\0'; a++)
	{
		buf[a] = tempString[a];
		charsWritten++;

	}
	free(tempString);
	return charsWritten;
}