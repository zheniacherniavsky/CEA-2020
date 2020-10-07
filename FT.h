#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <istream>
#include <string>

#include "IT.h"
#include "LT.h"
#include "FST.h"

#define MAX_LEXEMS_LENGTH 2048		// ������������ ������ ��� ������� ������

namespace FT
{
	char compareLexems(char* lexem); // ������� ��� ������
	int* getLineNums(std::string code);
	void fillTables(char* code, LT::LexTable& lt, IT::IdTable& it);
}