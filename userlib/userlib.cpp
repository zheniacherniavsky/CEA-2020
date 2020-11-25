// userlib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

// TODO: Это пример библиотечной функции.
extern "C"
{
	void __stdcall outint(int a)
	{
		std::cout << a << std::endl;
		return;
	}
}
