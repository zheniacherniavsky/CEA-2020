#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
//#include <math.h>
#include <Windows.h>
#pragma warning(disable: 4996)

extern "C"
{
	void outint(int a);
	void outstr(char* str);
	char* concat(char* str1, char* str2);
}