#pragma once

#include<string.h>
#include"argchecks.h"
#include<malloc.h>

#define MAX_ALPHABET_LEN 92
#define DEFAULT_PWD_LEN 10
#define DEFAULT_PWD_COUNT 5




int getPwdLen(char* argv[], int *minPwdLen, int *maxPwdLen) 
{
	int i = 1;
	int check = 0;




	while (argv[i] != NULL) 
	{
		if (strcmp(argv[i], "-m1") == 0 && strcmp(argv[i-1],"-a")!=0)
		{
			if (argv[i + 1] != NULL)
			{
				*minPwdLen = atoi(argv[i + 1]);
				check++;
			}
		}

		if (strcmp(argv[i], "-m2") == 0 && strcmp(argv[i-1],"-a")!=0)
		{
			if (argv[i + 1] != NULL) 
			{
				*maxPwdLen = atoi(argv[i + 1]);
				check++;
			}
		}
		if (strcmp(argv[i], "-n") == 0 && strcmp(argv[i-1],"-a")!=0)
		{
			if (argv[i + 1] == NULL)
			{
				printf("Inappropriate parameter for -n, set default value of 10\n\n");
				*maxPwdLen = DEFAULT_PWD_LEN;
				*minPwdLen = DEFAULT_PWD_LEN;
			}
			else 
				if(isConvertableToInt(argv[i+1])==0)
				{
					*maxPwdLen = atoi(argv[i + 1]);
					*minPwdLen = atoi(argv[i + 1]);
				}
				else
				{
					printf("Inappropriate parameter for -n , set default value of 10\n\n");
					*maxPwdLen = DEFAULT_PWD_LEN;
					*minPwdLen = DEFAULT_PWD_LEN;
				}
				check++;
		}
		i++;
	}

	if (*maxPwdLen > RAND_MAX)
	{
		printf("ERROR: maximal length of a password should not exceed 32767\n\n");
		return -1;
	}
	if (*minPwdLen > RAND_MAX)
	{
		printf("ERROR: minimal length of a password should not exceed 32767\n\n");
		return -1;
	}

	if (check == 0) 
	{
		printf("No length parameters found:assuming the default value of 10\n\n");
		*maxPwdLen = DEFAULT_PWD_LEN;
		*minPwdLen = DEFAULT_PWD_LEN;
	}
	return 0;
}


char* getPwdAlphabet(const char* argv[])
{
	int i = 1;
	int tmp;
	int check = 0;
	int RESULT_ALPHABET_LEN = 1;
	

	char* RESULT_ALPHABET;
	RESULT_ALPHABET = (char*)malloc(MAX_ALPHABET_LEN * sizeof(char));
	if (!RESULT_ALPHABET) 
	{
		printf("ERROR: memory allocation failure\n");
		return "ERROR";
	}


	for (int i = 0; i < MAX_ALPHABET_LEN; i++)
		RESULT_ALPHABET[i] = '\0';
	const char DEFAULT_ALPHABET[] = "ILOVEROCKNROLL";
	const char CAPITAL_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char LOWERCASE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";
	const char DIGITS_ALPHABET[] = "1234567890";
	const char SPECIAL_ALPHABET[] = "!#$%&'()*+,-./:;<=>?@[]^_{|}~";


	while (argv[i] != NULL) 
	{
		if (strcmp(argv[i], "-C") == 0 || strcmp(argv[i], "-a") == 0)
			check++;
			i++;
	}

	if (check == 0) 
	{
		printf("Couldn't find a correct alphabet parameter: assuming 'ILOVEROCKNROLL' as base alphabet\n\n");
		strcat_s(RESULT_ALPHABET,strlen( DEFAULT_ALPHABET)+1,DEFAULT_ALPHABET);
		return RESULT_ALPHABET;
	}

	i = 1;

	while (argv[i] != NULL) 
	{
		if (strcmp(argv[i], "-a")==0)
		{
			if (argv[i + 1] == NULL)
			{
				strcat_s(RESULT_ALPHABET, strlen(DEFAULT_ALPHABET) + 1, DEFAULT_ALPHABET);
				return RESULT_ALPHABET;
			}
			strcat_s(RESULT_ALPHABET, strlen(argv[i+1]) + 1, argv[i+1]);
			return RESULT_ALPHABET;
		}
		if (strcmp(argv[i], "-C") == 0 && strcmp(argv[i-1],"-a")!=0)
		{
			if (argv[i + 1] == NULL || strcmp(argv[i + 1], "-m1") == 0 || strcmp(argv[i + 1], "-m2") == 0 || strcmp(argv[i + 1], "-n") == 0 || strcmp(argv[i + 1], "-c") == 0)
			{
				strcat_s(RESULT_ALPHABET, strlen(DEFAULT_ALPHABET) + 1, DEFAULT_ALPHABET);
				return RESULT_ALPHABET;
			}

			tmp = strlen(argv[i + 1]);
			for (int j = 0; j < tmp; j++) 
			{
				if (argv[i+1][j] == 'a') 
				{
					RESULT_ALPHABET_LEN =RESULT_ALPHABET_LEN + strlen(LOWERCASE_ALPHABET);
					strcat_s(RESULT_ALPHABET, RESULT_ALPHABET_LEN, LOWERCASE_ALPHABET);
				}
				if (argv[i+1][j] == 'A')
				{
					RESULT_ALPHABET_LEN += strlen(CAPITAL_ALPHABET);
					strcat_s(RESULT_ALPHABET, RESULT_ALPHABET_LEN, CAPITAL_ALPHABET);
				}
				if (argv[i+1][j] == 'D')
				{
					RESULT_ALPHABET_LEN += strlen(DIGITS_ALPHABET);
					strcat_s(RESULT_ALPHABET,RESULT_ALPHABET_LEN, DIGITS_ALPHABET);
				}
				if (argv[i+1][j] == 'S') 
				{
					RESULT_ALPHABET_LEN += strlen(SPECIAL_ALPHABET);
					strcat_s(RESULT_ALPHABET,RESULT_ALPHABET_LEN, SPECIAL_ALPHABET);
				}
			}
		}
		i++;
	}



	return RESULT_ALPHABET;
}

unsigned long getPwdCount(const char* argv[]) 
{
	int i = 1;
	unsigned long count = 0;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i], "-c")==0)
		{
			if (strcmp(argv[i + 1], "-a") == 0 || strcmp(argv[i + 1], "-m1") == 0 || strcmp(argv[i + 1], "-m2")==0 || strcmp(argv[i + 1], "-C")==0 || strcmp(argv[i + 1], "-n") == 0 || argv[i + 1] == NULL)
			{
				printf("Couldn't find the passwords number:assuming default value of 5\n\n");
				return DEFAULT_PWD_COUNT;
			}
			count = strToULong(argv[i + 1]);
			return count;
		}
		i++;
	}
	printf("Couldn't find the passwords number:assuming default value of 5\n\n");
	return DEFAULT_PWD_COUNT;
}