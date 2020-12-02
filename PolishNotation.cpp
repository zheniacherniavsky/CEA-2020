#include "LT.h"
#include "IT.h"

#include <iostream>
#include <stack>
#include "PolishNotation.h"

namespace PN
{
	void addResult(LT::Entry& result, LT::Entry& lentaElement, IT::IdTable& idTable, LT::LexTable& ltTable);
	void addResult(LT::Entry& result, LT::Entry& lentaElement, bool memory = false);
	void addToStack(std::stack<LT::Entry*>& stack, LT::Entry* result, LT::Entry* element);

	void PolishNotation( // создание польской записи
		LT::LexTable& lexTable, // таблица лексем
		IT::IdTable& idTable, // таблица идентификаторов
		bool debug // вывод польской записи
	)
	{
		LT::Entry *lenta = LT::GetEntry(lexTable, 0);
		std::stack<LT::Entry*> stack;

		while (lenta->next != NULL)
		{
			if (lenta->lexema[0] == LEX_IS)
			{
				// обработка польской записи
				LT::Entry* result = new LT::Entry();
				LT::Entry* resultHead = new LT::Entry();
				LT::Entry* unionChain = lenta->next;	// символ после равно, который мы потом заменим на resultHead 
														// чтобы сцепить цепочки

				bool saved = false;
				bool specialFunction = false;

				while(lenta->lexema[0] != LEX_SEMICOLON)
				{
					if (result->lexema[0] != NULL && saved) result = result->next;
					lenta = lenta->next; // = -> expression

					if (lenta->lexema[0] == 'q' ||
						lenta->lexema[0] == '$' ||
						lenta->lexema[0] == 'c' ||
						lenta->lexema[0] == '~')
					{
						specialFunction = true;
						break;
					}

					switch (lenta->lexema[0])
					{
					case LEX_ID:
					case LEX_LITERAL:
						addResult(*result, *lenta, idTable, lexTable);
						if (!saved)
						{
							resultHead = result;	// сохраняю начало цепочки
							saved = true;
						}
						result = result->next;
						break;
					case LEX_RIGHTHESIS:
					case LEX_LEFTHESIS:
					case LEX_PLUS:
					case LEX_MINUS:
					case LEX_STAR:
					case LEX_DIRSLASH:
					case LEX_SEMICOLON:
					case('%'):
					case('^'):
					case('|'):
						addToStack(stack, result, lenta);
						if (result->lexema[0] != NULL) result = result->next;
						break;
					}
				}
				if (!specialFunction)
				{
					*unionChain = *resultHead;
					*lenta = *unionChain;
				}
			}
			lenta = lenta->next; // следующее выражение
		}
	}

	void addResult(LT::Entry& result, LT::Entry& lentaElement, IT::IdTable& idTable, LT::LexTable& ltTable)
	{
		switch (lentaElement.lexema[0])
		{
		case(LEX_ID):
			if (IT::GetEntry(idTable, lentaElement.idxTI)->idtype == IT::F)
			{
				LT::Entry* function = LT::GetEntryByIdx(ltTable, lentaElement.idxTI);
				int parms = 0;
				result.idxTI = lentaElement.idxTI;
				result.sn = lentaElement.sn;
				while (lentaElement.lexema[0] != LEX_RIGHTHESIS)
				{
					lentaElement = *lentaElement.next;
					if (lentaElement.lexema[0] == LEX_ID || lentaElement.lexema[0] == LEX_LITERAL)
					{
						result.func.idx[parms] = lentaElement.idxTI;
						parms++;
					}
				}
				result.func.count = parms;
				result.lexema[0] = '@';
				result.func.memoryCount = function->func.memoryCount;
				for (int i = 0; i < function->func.memoryCount; i++)
					result.func.memoryType[i] = function->func.memoryType[i];
				result.next = new LT::Entry();
				result.priority = 0;
				break;
			}
		case(LEX_LITERAL):
			result.idxTI = lentaElement.idxTI;
			result.lexema[0] = lentaElement.lexema[0];
			result.priority = lentaElement.priority;
			result.sn = lentaElement.sn;
			result.next = new LT::Entry();
			break;
		}
	}

	void addResult(LT::Entry& result, LT::Entry& lentaElement, bool memory)
	{
		result.idxTI = lentaElement.idxTI;
		result.lexema[0] = lentaElement.lexema[0];
		result.priority = lentaElement.priority;
		result.sn = lentaElement.sn;
		if (!memory) result.next = new LT::Entry();
		else result.next = lentaElement.next;
	}

	void addToStack(std::stack<LT::Entry*>& stack, LT::Entry* result, LT::Entry* element)
	{
		switch (element->lexema[0])
		{
		case LEX_LEFTHESIS:
			stack.push(element);
			break;
		case LEX_RIGHTHESIS:
			while (!stack.empty())
			{
				if (stack.top()->lexema[0] == LEX_LEFTHESIS)
				{
					stack.pop();
					break;
				}
				else
				{
					addResult(*result, *stack.top());
					result = result->next;
					stack.pop();
				}
			}
			break;
		case LEX_PLUS:
		case LEX_MINUS:
			if (stack.empty() || element->priority > stack.top()->priority)
				stack.push(element);
			else
			{
				addResult(*result, *stack.top());
				stack.pop();
				stack.push(element);
			}
			break;
		case LEX_STAR:
		case LEX_DIRSLASH:
		case('%'):
		case('^'):
		case('|'):
			if (stack.empty() || element->priority > stack.top()->priority)
				stack.push(element);
			else
			{
				addResult(*result, *stack.top());
				stack.pop();
				stack.push(element);
			}
			break;
		case LEX_SEMICOLON:
			while (!stack.empty())
			{
				addResult(*result, *stack.top());
				result = result->next;
				stack.pop();
			}
			addResult(*result, *element, true); // связываем ; с цепочкой. Остаётся связать только голову
			break;
		}
	}
}
