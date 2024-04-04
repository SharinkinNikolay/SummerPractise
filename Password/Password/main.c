#pragma once
#include"mastercheck.h"
#include"argparsing.h"
#include"generate.h"
#include<locale.h>





int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, "ru-RU");
	if (argc <= 1) 
	{
		messageBox();
		return -1;
	}

	if (masterCheck(argv) != 0)
		return 1;
	int MAX_PWD_LEN;
	int MIN_PWD_LEN;
	unsigned long PWD_COUNT = getPwdCount(argv);
	char* result = getPwdAlphabet(argv);
	getPwdLen(argv, &MIN_PWD_LEN, &MAX_PWD_LEN);
	generate(&MIN_PWD_LEN, &MAX_PWD_LEN, PWD_COUNT, result);

	free(result);
	return 0;
}