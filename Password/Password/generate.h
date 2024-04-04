#pragma once

#include<time.h>
#include<stdlib.h>
#include<stdio.h>




int generate(int* min_pwd_len, int* max_pwd_len, unsigned long pwd_count, char* alphabet)
{
	srand(time(NULL));

	char* password = (char*)malloc((*max_pwd_len + 1) * sizeof(char));
	if (!password)
	{
		printf("ERROR: memory allocation failure");
		return -1;
	}
	else {
		for (int i = 0; i < *max_pwd_len + 1; i++)
			password[i] = '\0';

		for (unsigned long j = 0; j < pwd_count; j++)
		{
			int real_pwd_len = rand() % (*max_pwd_len - *min_pwd_len + 1) + *min_pwd_len;
			int alphabet_len = strlen(alphabet);

			for (int i = 0; i < real_pwd_len; i++)
			{
				password[i] = alphabet[rand() % (alphabet_len)];
			}
			printf("%s\n", password);
			for (int i = 0; i < *max_pwd_len + 1; i++)
				password[i] = '\0';
		}
		free(password);
		return 0;
	}
}