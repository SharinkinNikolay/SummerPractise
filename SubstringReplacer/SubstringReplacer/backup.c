#include<stdio.h>

#define MAX_BUFFER_SIZE 512

int myStrlen(char* string) 
{
	int i = 0;
	while (string[i] != '\0')
		i++;
	return i;
}
void myCharCopy(char* srcBuf, char* dstBuf, int count,int srcIndex) 
{
	int i = 0;
	for (int j = srcIndex;count>0; j++) 
	{
		dstBuf[i] = srcBuf[j+1];
		i++;
		count--;
	}
	return;
}

void moveRemainder(char* remainder) 
{
	int len = myStrlen(remainder) - 1;
	for (int i = 0; i < len; i++) 
	{
		remainder[i] = remainder[i + 1];
	}
}

int main(int argc,char* argv[])
{
	if (argc != 5)
	{
		printf("ERROR:\t INCORRECT NUMBER OF ARGUMENTS");
		return -1;
	}
	char* inputName = argv[1];
	char* outputName = argv[2];
	char* stringtoReplace = argv[3];
	char* stringToPlace = argv[4];

	FILE* inputFile = fopen(inputName, "r");
	FILE* outputFile = fopen(outputName, "w");
	if (inputFile == NULL || outputFile == NULL)
	{
		printf("ERROR: COULD NOT OPEN THE FILES");
		return -1;
	}
	char buffer[MAX_BUFFER_SIZE];
	char remainder[MAX_BUFFER_SIZE];

	int bytesRead = 0;
	int found = 0;
	int blockStartIndex = 0;
	int remainderIsEmpty = 1;
	while ((bytesRead = fread(buffer,1,MAX_BUFFER_SIZE,inputFile))>0)//читаем по 512 до конца(может и меньше)
	{
		//Нашли found элементов в прошлом блоке, значит нужно проверить ещё myStrlen(stringtoReplace)-found элементов
		//Если все совпадают,вывести замененную строку и сдвинуть указатель на myStrlen(stringtoReplace)-found,
		// 
		//A если нашли несовпадение на каком-то индексе, вывести в файл found байт из remainder + i проитерированных, которые не подошли
		//и сдвинуть указатель на i
		if (found > 0) 
		{
			int elementsToCheck = myStrlen(stringtoReplace) - found;
			int checkSum = 0;
			int strIndex = found;
			//Если блок оказался меньшей длины,чем нужно для совпадения
			if (bytesRead < elementsToCheck) 
			{
				fwrite(remainder, 1, found, outputFile);
				fwrite(buffer, 1, bytesRead, outputFile);
				found = 0;
				remainderIsEmpty = 1;
				break;
			}
			for (int i = 0; i < elementsToCheck; i++)
			{
				//Нашли несовпадение в i-ом, записали первый символ из остатка, сдвинули проверку на 1 вправо и повторяем
				//пока остаток не кончится
				if (buffer[i] != stringtoReplace[strIndex])
				{
					fwrite(remainder, 1, 1, outputFile);
					moveRemainder(remainder);
					found--;
					strIndex = found;
					elementsToCheck++;
					checkSum = 0;
					i = -1;
					if (found == 0)
					{
						blockStartIndex = 0;
						break;
					}
					//fwrite(remainder, 1, found, outputFile);
					//fwrite(buffer, 1, i, outputFile);
					//found = 0;
					//blockStartIndex = i;
					//remainderIsEmpty = 1;
					//break;
				}
				//нашли совпадение, проверяем, нашлась ли строка: если да,то записываем и сдвигаем стартовый индекс на 
				else 
				{
					checkSum++;
					strIndex++;
				}
				//Если все элементы найдены, то выводим замену и сдвигаем указатель на elementsToCheck+1
			}
			if (checkSum == elementsToCheck)
			{
				fwrite(stringToPlace, 1, myStrlen(stringToPlace), outputFile);
				found = 0;
				blockStartIndex = elementsToCheck;
				remainderIsEmpty = 1;
			}
		}
		for (int i = blockStartIndex; i < bytesRead; i++) 
		{
			if (buffer[i] != stringtoReplace[0] && found == 0)//если первый символ не совпадает, можно сразу делать ++
			{
				fwrite(buffer+i, 1, 1, outputFile);
				continue;
			}

			if (buffer[i] == stringtoReplace[0] && found == 0)//если нашли первый совпавший, начинаем обработку
			{
				{
					int stringPos = 0;
					int initPos = i;//сохраняем позицию,с которой начали проверку подстроки,чтобы пройти полную длину строки
					for (int pos = i; pos<bytesRead && pos<initPos+myStrlen(stringtoReplace); pos++)
					{

						//Если наткнулись на элемент не из строки,выводим всю неполную подстроку и обнуляем счетчик
						if ((buffer[pos] != stringtoReplace[stringPos] && found > 0))
						{
							fwrite(stringtoReplace, 1, found, outputFile);
							i += found-1;
							found = 0;
							break;
						}
						//от индекса,где нашли до длины заменяемой строки смотрим вхождения и фиксируем их количество в found
						if (buffer[pos] == stringtoReplace[stringPos])
						{
							found++;
							stringPos++;
						}

						//если строка полная, то выводим заменяющую последовательность,устанавливаем указатель в файле
						// на новое место и обнуляем счетчик
						if (found == myStrlen(stringtoReplace))
						{
							fwrite(stringToPlace, 1, myStrlen(stringToPlace), outputFile);
							i += found-1;
							found = 0;
							break;
						}
						//Если found>0, но блок подошел к концу, сохраняем found предыдущих элементов в remainder,чтобы в главном цикле 
						//проверить начало следующего цикла
						if (found > 0 && pos + 1 == bytesRead) 
						{
							myCharCopy(buffer, remainder, found, pos - found);
							remainderIsEmpty = 0;
							break;
						}
					}
				}
			}

		}

	}
	if (remainderIsEmpty == 0)
		fwrite(remainder, 1, found, outputFile);
	fclose(inputFile);
	fclose(outputFile);
}