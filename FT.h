#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#include <iostream>
#include <string>

#include "IT.h"
#include "LT.h"
#include "FST.h"

#define MAX_LEXEMS_LENGTH 2048		// максимальный размер для массива лексем

namespace FT
{
	void makeOutWithLT(LT::LexTable& lt, IT::IdTable &id);
	void makeOutWithIT(IT::IdTable& id);
	char compareLexems(char* lexem, LT::Entry* ltElement); // автомат для лексем
	int* getLineNums(std::string code);
	void fillTables(char* code, LT::LexTable& lt, IT::IdTable& it);
}