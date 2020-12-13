// userlib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

// TODO: Это пример библиотечной функции.
extern "C"
{
	void _pause()
	{
		system("pause");
		return;
	}

	void outint(int a)
	{
		setlocale(LC_ALL, "rus");
		std::cout << a << " [ Десятичная система счисления ]" << std::endl;
		return;
	}

	int _AND(int a, int b)
	{
		return b & a; // b % a тк берем из стека первым a, но это короче b, так шо местами меняем оп оп 
	}

	int _OR(int a, int b)
	{
		return b | a;
	}

	int _NOT(int a)
	{
		return ~a;
	}

	int powN(int num, int degree)
	{
		int r = pow(num, degree);
		return r;
	}

	int rootN(int num, int degree)
	{
		int r = pow(num, 1.0/degree);
		return r;
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
		{
			return strcpy(str1, str2);
		}
	}

	char* tostr(int number) {

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

	void outstr(char* str)
	{
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
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

	int strequal(char* str1, char* str2)
	{
		if (!str1 || !str2)
		{
			std::cout << "ERROR: Null string in strcon function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}

		if (strcmp(str1, str2) == 0) return 1;
		else return 0;
	}
}
