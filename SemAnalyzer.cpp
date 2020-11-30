﻿#include "SemAnalyzer.h"

namespace SemAnalyzer
{

	// ++ non declared variables should be tracked!

	struct Flag
	{
		bool declare = false;
		bool function = false;
		bool expression = false;
		bool returnExpression = false;
		bool main = false;
		bool repeat = false;
		bool itos = false;
	};


	bool semAnalyzer(LT::LexTable lt, IT::IdTable it)
	{
		LT::Entry *lexem = lt.head; // таблица лексем
		int line = 0;				// для вывода анализа	
		int errorCount = 0;
		std::string errorMessage = "";

		Flag f;		// флажки

		IT::IDDATATYPE expressionType = IT::EMPTY, functionType = IT::EMPTY;
		IT::Entry* element;

		SEM_START
		while (lexem->next)
		{
			SEM_TRACE1
			while (lexem) // разбор строки
			{
				switch (lexem->lexema[0])
				{
				case LEX_LITERAL:
				case LEX_ID:
				case('@'):
					element = IT::GetEntry(it, lexem->idxTI);
					std::cout << getType(element);


					if (!element->declared && !f.declare && element->idtype == IT::V)
					{
						errorCount++;
						errorMessage = "ВЫЗОВ НЕОБЪЯВЛЕННОЙ ПЕРЕМЕННОЙ";
					}
					
					if (expressionType == IT::EMPTY) expressionType = element->iddatatype;
					if (f.returnExpression)
					{
						if (element->iddatatype != functionType)
						{
							errorCount++;
							if (!f.main) errorMessage = "ВОЗРАЩАЕМОЕ ЗНАЧЕНИЕ НЕ СОВПАДАЕТ С ТИПОМ ФУНКЦИИ";
							else errorMessage = "ВОЗРАЩАЕМОЕ ЗНАЧЕНИЕ НЕ СОВПАДАЕТ С ТИПОМ ФУНКЦИИ (ТИП ФУНКЦИИ MAIN -> INT)";
						}
					}
					else if (f.function)
					{
						if (!f.main) functionType = element->iddatatype;
						f.function = false;
					}
					else if (f.expression)
					{
						if (lexem->lexema[0] == '@') // function check types
						{
							std::cout << "[";
							for (int i = 0; i < lexem->func.memoryCount; i++)
							{
								IT::Entry* checkType = IT::GetEntry(it, lexem->func.idx[i]);
								if (lexem->func.count != lexem->func.memoryCount)
								{
									errorCount++;
									errorMessage = "НЕДОСТАТОЧНО ВЫЗЫВАЕМЫХ ПАРАМЕТРОВ";
								}
								std::cout << getType(lexem->func.memoryType[i]) << "=" << getType(checkType) << " ";

								if (lexem->func.memoryType[i] != checkType->iddatatype)
								{
									errorCount++;
									errorMessage = "ОШИБКА В ПАРАМЕТРАХ ВЫЗЫВАЕМОЙ ФУНКЦИИ. НЕ СОВПАДАЮТ ТИПЫ";
								}
							}
							std::cout << ']';
						}

						if(element->iddatatype != expressionType && !f.itos)
						{
							errorCount++;
							errorMessage = "НЕ СОВПАДАЮТ ТИПЫ ВЫРАЖЕНИЯ";
						}
					}
					break;
				case LEX_DECLARE:
					// flag declare for line
					f.declare = true;
					break;
				case LEX_IS:
					f.declare = false;
					f.expression = true;
					break; 
				case LEX_MAIN:
					functionType = IT::INT;
					f.main = true;
					break;
				case LEX_FUNCTION:
					f.function = true; // function body
					break;
				case LEX_RETURN:
					f.returnExpression = true;
					break;
				case LEX_BRACELET:
					if (f.repeat) f.repeat = false;
					else {
						f.returnExpression = false;
						f.expression = false;
						functionType = IT::EMPTY;
						expressionType = IT::EMPTY;
						f.main = false;
					}
					break;
				case('w'):
					f.repeat = true;
					break;
				case('c'):
					f.itos = true;
					break;
				}

				std::cout << lexem->lexema[0];

				if (lexem->next && lexem->sn == lexem->next->sn) lexem = lexem->next;
				else break;
			} 
			if (errorMessage != "")
			{
				SEM_ERROR(errorMessage)
				errorMessage = "";
			}
				
			expressionType = IT::EMPTY;
			line++;
			lexem = lexem->next;
		}

		SEM_END;
	}

	const char* getType(IT::Entry* element)
	{
		switch (element->iddatatype)
		{
		case IT::INT: return " INT:"; break;
		case IT::STR: return " STR:"; break;
		}
	}

	const char* getType(IT::IDDATATYPE type)
	{
		switch (type)
		{
		case IT::INT: return " INT:"; break;
		case IT::STR: return " STR:"; break;
		}
	}
}