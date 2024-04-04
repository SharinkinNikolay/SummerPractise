#include"func.h"
#include<malloc.h>


void tests_strtoi() 
{

	/////////////////////////////////
	char* stringIncorrect = "aboba";


	char* stringDec = "2147483647";//INT_MAX
	char* stringDecOv = "b1000000000";//Overflow



	char* stringHex = "0x7FFFFFFF";//INT_MAX
	char* stringHExOv = "0xDDDDDDDDDD";//Overflow


	char* stringOct = "000051";
	char* stringOctOv = "-0777777777";//Overflow




}

int main(void) 
{
	const char* string = "07";
	char* p = NULL;

	//int ret=1;
	//	printf("%d %d %s", str_to_int(string, &p, &ret), ret, p);


	char* buf = (char*)malloc(sizeof(char) * 10);
	printf("%d:%s", my_itoa(buf, 11, -INT_MAX-1, 10),buf);

	//tests_strtoi();


	return 0;

}