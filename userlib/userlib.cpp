// userlib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

// TODO: Это пример библиотечной функции.
extern "C"
{
	void outint(int a)
	{
		std::cout << a << std::endl;
		return;
	}

	char* copystr(char* str1, char* str2)
	{
		if (!str1 || !str2)
		{
			std::cout << "ERROR: Null string pointer in copystr function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}
		else
			return strcpy(str1, str2);
	}

	char* ConvertToChar(int number) { // ty KBroomy, ily
		if (number == 0)
			return (char*)"0";
		char* temp = (char*)calloc(255, sizeof(char));
		char* buffer = (char*)calloc(255, sizeof(char));
		if (temp == NULL)
			return NULL;
		if (buffer == NULL)
			return NULL;

		int bin = 0, i = 1;
		while (number) {
			bin += (number % 2) * i;
			i *= 10;
			number /= 2;
		}
		i = 0;
		while (bin) {
			temp[i++] = (bin % 10) + '0';
			bin /= 10;
		}
		temp[i] = '\0';

		for (size_t i = 0, j = strlen(temp) - 1; i < strlen(temp); i++, j--)
			buffer[i] = temp[j];
		return buffer;
	}

	int intpow(int a, int b)
	{
		return pow(a, b);
	}

	void outstr(char* str)
	{
		if (!str)
		{
			std::cout << "ERROR: Null string in strcon function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}

		std::cout << str << std::endl;
		return;
	}

	char* strcon(char* str1, char* str2)
	{
		char* buf = new char[255];
		if (!str1 || !str2)
		{
			std::cout << "ERROR: Null string in strcon function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}
		strcpy(buf, str2);
		strcat(buf, str1);

		return  buf;
	}
}
