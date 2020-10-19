#include "LT.h"
#include "IT.h"

#include <iostream>
#include <stack>
#include "PolishNotation.h"

/*
	ISSUES:
		- Пустые строки брикают процесс, решить 
		- Утечка памяти при изменении таблы лексем
*/

namespace PN
{
	struct polishNotation
	{
		LT::Entry* result;
		LT::Entry* resultHead;
		std::stack <LT::Entry> symbols;

		polishNotation()
		{
			result = nullptr;
			resultHead = nullptr;
		}
	};

	bool isSymbol(LT::Entry* element);
	void addRusult(polishNotation& p, LT::Entry* element);

	bool PolishNotation( // создание польской записи
		int lextable_pos, // позиция выражения в таблице лексем
		LT::LexTable& lexTable, // таблица лексем
		IT::IdTable& idTable, // таблица идентификаторов
		bool debug // вывод польской записи
	)
	{
		/*
		Алгоритм построения:
		1 - исходная строка: выражение;
		- результирующая строка: польская запись;
		- стек: пустой;
		- исходная строка просматривается слева направо;
		- операнды переносятся в результирующую строку в порядке их следования;
		- операция записывается в стек, если стек пуст или в вершине стека лежит отрывающая скобка;
		- операция выталкивает все операции с большим или равным приоритетом в результирующую строку;
		- открывающая скобка помещается в стек;
		- закрывающая скобка выталкивает все операции до открывающей скобки, после чего обе скобки уничтожаются;
		- по концу разбора исходной строки все операции, оставшиеся в стеке, выталкиваются в результирующую строку
		*/

		// 1 - получение исходной строки выражения

		// i=a*(b+c)
		// 
		// a*(b+c)     
		// *(b+c)       a	
		// (b+c)		a		*
		// b+c)			a		*(
		// +c)			ab		*(
		// c)			ab		*(+
		// )			abc		*(+
		//				abc+	*
		//				abc+*
		
		polishNotation pol;
		LT::Entry* sourceStr = LT::GetEntry(lexTable, lextable_pos);
		LT::Entry* nextLine = LT::GetEntry(lexTable, lextable_pos + 1);
		LT::Entry* head = sourceStr;
		
		LT::Entry* beforePN = sourceStr;

		if (debug)
		{
			std::cout << "До Польской записи:" << std::endl;
			while (beforePN->sn == lextable_pos)
			{
				std::cout << beforePN->lexema[0];
				beforePN = beforePN->next;
			}
			std::cout << std::endl;
		}
		
		// algorithm
		/*
		
		while(строка совпадает с выбранной)
		{
			if(НЕ символ операции)
			{
				добавление в результирующую строку, возврат false и доп. провека
			}
			else // У нас символ операции
			{
				if(ПРОВЕРКА на правую скобку) { ... } // выполнения алгоритма правой скобки
				else
				{
					...  заполнения стека или добавление в результирующую строку
				}
			}
		}

		*/


		while (sourceStr != nullptr && sourceStr->sn == lextable_pos)
		{
			char s = sourceStr->lexema[0]; // lex symbol
			if (!isSymbol(sourceStr))
			{
				if (s == LEX_SEMICOLON)
				{
					while (!pol.symbols.empty())
					{
						addRusult(pol, &pol.symbols.top()); // добавляем в резалт наши знаки
						pol.symbols.pop(); // убираем эти знаки из стека
					}
				}
				else if (s != LEX_ID && s != LEX_IS)
				{
					if (debug) {
						std::cout << "Польская запись для этой строчки не построена!" << std::endl;
					}
					return false;
				}
				addRusult(pol, sourceStr); // если идентификатор, то кидаем в резалт
			}
			else
			{
				if (s == LEX_RIGHTHESIS) // если встречаем ), то
				{
					while (pol.symbols.top().lexema[0] != LEX_LEFTHESIS) // пока не встретим (
					{
						addRusult(pol, &pol.symbols.top()); // добавляем в резалт наши знаки
						pol.symbols.pop(); // убираем эти знаки из стека
					}
					pol.symbols.pop(); // удаляем (
				}
				else
				{
					if (!pol.symbols.empty())
					{
						while ((pol.symbols.top().priority >= sourceStr->priority) && sourceStr->priority != 0)
						{
							addRusult(pol, &pol.symbols.top()); // добавляем в резалт наши знаки
							pol.symbols.pop(); // убираем эти знаки из стека
							if (pol.symbols.empty()) break;
						}
					}
					pol.symbols.push(*sourceStr); // иначе добавляем символ в стек
				}
			}
			sourceStr = sourceStr->next;
		}

		if (debug)
		{
			std::cout << "\nПосле Польской записи:" << std::endl;
			LT::Entry* out = pol.resultHead;
			while (out != nullptr)
			{
				std::cout << out->lexema[0];
				out = out->next;
			}
			std::cout << std::endl;
		}

		// ISSUE: утечка памяти, удалять lextable

		head->idxTI = pol.resultHead->idxTI;
		head->lexema[0] = pol.resultHead->lexema[0];
		head->priority = pol.resultHead->priority;
		head->sn = pol.resultHead->sn;

		head->next = pol.resultHead->next;
		while (head->next != nullptr) head = head->next;
		head->next = nextLine;
		
		return true;
	}

	void addRusult(polishNotation& p, LT::Entry* element)
	{
		if (p.result == nullptr)
		{
			p.result = new LT::Entry();
			p.resultHead = p.result;
		}
		else
		{
			p.result->next = new LT::Entry();
			p.result = p.result->next;
		}

		p.result->idxTI = element->idxTI;
		p.result->lexema[0] = element->lexema[0];
		p.result->sn = element->sn;
	}
	
	bool isSymbol(LT::Entry* element)
	{
		const char* s = "(),+-*/"; // ISSUES: add []

		for (int i = 0; i < strlen(s); i++)
		{
			if (element->lexema[0] == s[i]) return true;
		}
		return false;
	}
}
