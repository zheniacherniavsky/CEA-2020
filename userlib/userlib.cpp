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

	char* ConvertToChar(int number) {

		char* buffer = (char*)calloc(5, sizeof(char));
		int pos = 100; // max num value is 255
		for (int i = 0; pos != 0; i++, pos /= 10)
		{
			if (number / pos == 0) { i--; continue; };
			switch ((number / pos) % 10)
			{
			case(0): buffer[i] = '0'; break;
			case(1): buffer[i] = '1'; break;
			case(2): buffer[i] = '2'; break;
			case(3): buffer[i] = '3'; break;
			case(4): buffer[i] = '4'; break;
			case(5): buffer[i] = '5'; break;
			case(6): buffer[i] = '6'; break;
			case(7): buffer[i] = '7'; break;
			case(8): buffer[i] = '8'; break;
			case(9): buffer[i] = '9'; break;
			}
		}
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
		char* buf = (char*)calloc(255, sizeof(char));
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
