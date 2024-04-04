#pragma once
#include"argchecks.h"




int masterCheck(const char* argv[])
{
	if (alphabetOptions(argv) != 0)//�������� ��������������� -� � -�
		return 1;
	if (alphabetCheck(argv) != 0) // �������� ���������� -� � -�
		return 1;
	if (lengthOptions(argv) != 0)// �������� ��������������� -m1 -m2 � n
		return 1;
	if (otherOptions(argv) != 0)// �������� �� ������� -n � -�
		return 1;
	if (argsAreNumbers(argv) != 0)// �������� �������� ���������� �����  -m1 -m2 -n -c 
		return 1;
	if (minMaxComparison(argv) != 0)//�������� ������������ �������� -m1 -m2 (m1<m2)
		return 1;

	return 0;
}


void messageBox() 
{
	printf("COULD NOT DEFINE PASSWORD CHARACTERISTICS\n\nPlease specify at least one of the available options:\n\n -m1/-m2 | min/max length of a password\n -n      | password length\n -a abc  | password alphabet\n -c      | number of passwords\n -C aADS | choose one or more of the preset alphabets\n");
	printf("\nNote that:\n\n -m1/-m2 is incompatible with -n\n -C is incompatible with -a\n -m1 argument cannot be bigger than -m2 argument\n -C and -a parameters cannot be present more than once\n -C argument can only contain one or several of the following letters: aADS , but not more then one for each\n");
	printf("\nThese parameters have default argument values in case they are not found: \n [-n (10)] \n [-c (5)] \n [-a & -C (ILOVEROCKNROLL)] \n\nEvery parameter is optional on its own, but at least one should be specified\n");
}