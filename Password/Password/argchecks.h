#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>




int isConvertableToInt(const char* string)
{
	int i = 0;
	while (string[i] != '\0')
	{
		if ((string[i] < 47) || (string[i] > 58))
			return 1;
		i++;
	}
	return 0;
}


 unsigned long strToULong(const char* string)
{
	 unsigned long result = strtoul(string,NULL,0);
	 return result;
}


int lengthOptions(const char* argv[])// �������� ��������������� -m1 -m2 � n
{
	int count = 0;
	int i = 1;
	int j = 1;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i],"-m1")==0 && strcmp(argv[i-1],"-a")!=0)
			count++;
		i++;
		if (count > 1) //���� �� ���� ���� ������ 2, ������ m1 ����������
		{
			printf("ERROR:-m1 arguments should not repeat");
			return -1;
		}
	}
	i = 1;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i],"-m2")==0 && strcmp(argv[i-1],"-a")!=0)
			count++;
		i++;
		if (count > 2) //�� ���� ���� count=1 ���� 0(���� ��� ��������� ����� �� �������),
						//������� �� ������ ���������� ���� 2,���� 0, ����� ��������� �����������
		{
			printf("ERROR:-m2 arguments should not repeat");
			return -1;
		}
	}
	i = 1;
	while (argv[i] != NULL) 
	{
		if ((strcmp(argv[i],"-n")==0) && (count>0) && strcmp(argv[i-1],"-a")!=0)
		{
			printf("ERROR: -m and -n parameters are incompatible");
			return -1;
		}
		i++;
	}

	if (count == 1) 
	{
		printf("ERROR: one of the -m parameters is missing");
		return -1;
	}
	return 0;
}


int alphabetOptions(const char* argv[]) //�������� �� ������� �  ��������������� -� � -�
{
	int i = 1;
	int check = 0;

	while (argv[i] != NULL) 
	{
		if (strcmp(argv[i], "-a") == 0 && strcmp(argv[i - 1], "-a") != 0)
		{
			check++;
		}
		if (strcmp(argv[i], "-C") == 0 && strcmp(argv[i - 1], "-a") != 0)
			check++;
		i++;
	}
	if (check > 1) 
	{
		printf("ERROR: two or more incompatible -a and -C parameters found");
		return -1;
	}

	i = 1;
	return 0;
}

int otherOptions(const char* argv[]) // �������� �� ������� -n � -�
{
	int i = 1;
	int check = 0;
	while (argv[i] != NULL) 
	{
		if (strcmp(argv[i], "-n") == 0 && strcmp(argv[i-1],"-a")!=0)
			check++;
		if (check > 1) 
		{
			printf("ERROR: -n should not repeat");
			return -1;
		}
		i++;
	}
	i = 1;
	check = 0;
	while (argv[i] != NULL) 
	{
		if (strcmp(argv[i], "-c")==0 && strcmp(argv[i-1],"-a")!=0)
			check++;
		if (check > 1) 
		{
			printf("ERROR: -c should not repeat");
			return -1;
		}
		i++;
	}
	return 0;
}



int argsAreNumbers(const char* argv[]) // -m1 -m2 ������ ����� ��������� � ���� ����� �� ����
{
	int i = 1;

	while (argv[i] != NULL) 
	{
		if ( (strcmp(argv[i], "-m1") == 0 || strcmp(argv[i], "-m2")==0) )
		{
			if (argv[i + 1] == NULL && strcmp(argv[i-1],"-a")!=0)
			{
				printf("ERROR:one of the m1/m2 parameters does not contain an argument");
				return -1;
			}
			if (strcmp(argv[i - 1], "-a") == 0) 
			{
				i++;
				continue;
			}

			if (isConvertableToInt(argv[i + 1] )!= 0)
			{
				printf("ERROR: one of the parameters' arguments cannot be converted to an unsigned number, or the argument for one of the -m1/-m2 commands was not present");
				return -1;
			}

		}
		if (strcmp(argv[i], "-c") == 0 && strcmp(argv[i - 1], "-a") != 0)
		{
			if (isConvertableToInt(argv[i + 1]) != 0) 
			{
				printf("ERROR: -c argument cannot be converted to unsigned int");
				return -1;
			}
		}
		i++;
	}
	return 0;
}


int alphabetCheck(const char* argv[]) // �������� ���������� -� [aADS]
{
	int a = 0;
	int A = 0;
	int D = 0;
	int S = 0;
	int argEnd = 1;
	int endl = 0;
	int i = 1;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i], "-C") == 0)
		{
			if (strcmp(argv[i-1],"-a") == 0)
				return 0;
			if (argv[i + 1] == NULL || strcmp(argv[i+1],"-n")==0 || strcmp(argv[i+1],"-m1")==0 || strcmp(argv[i+1],"-m2")==0 || strcmp(argv[i+1],"-c")==0)
			{
				return 0;
			}
			while (argv[i + 1][endl] != '\0')
				endl++;
			if (endl < 5) // "aADS" - �� ������ 4 ��������
			{
				for (int j = 0; j < endl; j++)
				{
					if (argv[i + 1][j] != 'a' && argv[i + 1][j] != 'A' && argv[i + 1][j] != 'D' && argv[i + 1][j] != 'S')
					{
						printf("ERROR: incorrect input of -C parameters(wrong arguments)");
						return -1;
					}
					switch (argv[i + 1][j])
					{
					case 'a':
						a++;
						break;
					case 'A':
						A++;
						break;
					case 'D':
						D++;
						break;
					case 'S':
						S++;
						break;
					}
					if (a > 1 || A > 1 || D > 1 || S > 1)
					{
						printf("ERROR: several identical arguments for -C were given");
						return -1;
					}

				}
			}
			else
			{
				printf("ERROR: incorrect input of -C parameters(more than 4 parameters were given)");
				return -1;
			}
		}
		i++;
	}
	
	return 0;
}

int minMaxComparison(const char* argv[]) 
{
	int i = 0;
	int m1 = 0;
	int m2 = 0;
	while (argv[i+1] != NULL) 
	{
		if (strcmp(argv[i], "-m1") == 0)
			m1 = atoi(argv[i + 1]);
		if (strcmp(argv[i], "-m2") == 0)
			m2 = atoi(argv[i + 1]);
		i++;
	}
	if (m1 > m2) 
	{
		printf("ERROR: -m1 argument cannot be bigger than -m2 argument");
		return -1;
	}
	if (m1 > RAND_MAX || m2 > RAND_MAX) 
	{
		printf("ERROR: -m1/-m2 arguments cannot exceed 32767");
		return -1;
	}
	return 0;
}
