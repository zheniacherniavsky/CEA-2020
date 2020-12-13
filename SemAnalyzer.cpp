#include "SemAnalyzer.h"

namespace SemAnalyzer
{

	struct Flag
	{
		bool declare = false;
		bool function = false;
		bool expression = false;
		bool returnExpression = false;
		bool main = false;
		bool repeat = false;
		bool condition = false;
		bool itos = false;
	};

	bool semAnalyzer(LT::LexTable lt, IT::IdTable it, Log::LOG_ log)
	{
		LT::Entry *lexem = lt.head; // таблица лексем
		int line = 0;				// для вывода анализа
		char condition;				// для проверки условий
		int errorCount = 0;
		std::string errorMessage = "";
		Error::ERROR_ errors[3];
		short firstRepeatOrCondition = 0;  // Кто первый, цикл или условие (цикл == 1, условие == 2)

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
				case LEX_DOG:
					element = IT::GetEntry(it, lexem->idxTI);
					*log.stream << getType(element);

					if (element->declared && f.declare && element->idtype == IT::V)
					{
						errors[errorCount++] = Error::geterrorin(700, lexem->sn, 0);
						errorMessage = "ЭТА ПЕРЕМЕННА УЖЕ БЫЛА ОБЪЯВЛЕНА";
						break;
					}
					else if (!element->declared && !f.declare && element->idtype == IT::V)
					{
						errors[errorCount++] = Error::geterrorin(701, lexem->sn, 0);
						errorMessage = "ВЫЗОВ НЕОБЪЯВЛЕННОЙ ПЕРЕМЕННОЙ";
						break;
					}
					else if (!element->declared && f.declare && element->idtype == IT::V)
						element->declared = true;
					
					if (expressionType == IT::EMPTY) expressionType = element->iddatatype;
					if (f.returnExpression)
					{
						if (element->iddatatype != functionType)
						{
							if (!f.main)
							{
								errors[errorCount++] = Error::geterrorin(702, lexem->sn, 0);
								errorMessage = "ВОЗРАЩАЕМОЕ ЗНАЧЕНИЕ НЕ СОВПАДАЕТ С ТИПОМ ФУНКЦИИ";
								break;
							}
							else
							{
								errors[errorCount++] = Error::geterrorin(703, lexem->sn, 0);
								errorMessage = "ВОЗРАЩАЕМОЕ ЗНАЧЕНИЕ НЕ СОВПАДАЕТ С ТИПОМ ФУНКЦИИ (ТИП ФУНКЦИИ MAIN -> INT)";
								break;
							}
						}
					}
					else if (f.function)
					{
						if (!f.main) functionType = element->iddatatype;
						f.function = false;
					}
					else if (f.expression)
					{
						if (lexem->lexema[0] == LEX_DOG) // function check types
						{
							*log.stream << "[";
							for (int i = 0; i < lexem->func.memoryCount; i++)
							{
								IT::Entry* checkType = IT::GetEntry(it, lexem->func.idx[i]);
								if (lexem->func.count != lexem->func.memoryCount)
								{
									errors[errorCount++] = Error::geterrorin(704, lexem->sn, 0);
									errorMessage = "НЕДОСТАТОЧНО ВЫЗЫВАЕМЫХ ПАРАМЕТРОВ";
								}
								*log.stream << getType(lexem->func.memoryType[i]) << "=" << getType(checkType) << " ";

								if (lexem->func.memoryType[i] != checkType->iddatatype)
								{
									errors[errorCount++] = Error::geterrorin(705, lexem->sn, 0);
									errorMessage = "ОШИБКА В ПАРАМЕТРАХ ВЫЗЫВАЕМОЙ ФУНКЦИИ. НЕ СОВПАДАЮТ ТИПЫ";
								}
							}
							*log.stream << ']';
						}

						if(element->iddatatype != expressionType && !f.itos)
						{
							errors[errorCount++] = Error::geterrorin(706, lexem->sn, 0);
							errorMessage = "НЕ СОВПАДАЮТ ТИПЫ ВЫРАЖЕНИЯ";
						}
					}
					break;
				case(LEX_CONDITION):
					*log.stream << lexem->lexema[0];
					f.condition = true;
					if (firstRepeatOrCondition == 0) firstRepeatOrCondition = 2;
					lexem = lexem->next; // : -> (
					*log.stream << lexem->lexema[0];
					lexem = lexem->next; // ( -> Q
					
					element = IT::GetEntry(it, lexem->idxTI);
					*log.stream << getType(element);
					*log.stream << lexem->lexema[0];

					expressionType = element->iddatatype; // запоминаю первый член

					lexem = lexem->next; // Q -> C
					*log.stream << ' ' << lexem->lexema[0] << ' ';
					condition = lexem->lexema[0]; // запоминаю условие

					lexem = lexem->next; // C -> Q
					element = IT::GetEntry(it, lexem->idxTI);
					*log.stream << getType(element);

					switch (condition)
					{
					case(LEX_LESS):
					case(LEX_OVER):
						if (expressionType == IT::STR || element->iddatatype == IT::STR)
						{
							errors[errorCount++] = Error::geterrorin(707, lexem->sn, 0);
							errorMessage = "ДЛЯ СТРОК МОЖНО ПРОВЕРЯТЬ ТОЛЬКО ЭКВИВАЛЕНТНОСТЬ!";
						}
						break;
					case(LEX_EQUAL):
						if ((expressionType == IT::STR && element->iddatatype == IT::INT) ||
							(expressionType == IT::INT && element->iddatatype == IT::STR))
						{
							errors[errorCount++] = Error::geterrorin(708, lexem->sn, 0);
							errorMessage = "НЕЛЬЗЯ СРАВНИВАТЬ СТРОКУ С ЧИСЛОМ!";
						}
						break;
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
				case LEX_SEMICOLON:
					f.declare = false;
					break;
				case LEX_MAIN:
					functionType = IT::INT;
					f.main = true;
					break;
				case LEX_PRINT_INT:
					if (IT::GetEntry(it, lexem->next->idxTI)->iddatatype != IT::INT)
					{
						errors[errorCount++] = Error::geterrorin(705, lexem->sn, 0);
						errorMessage = "ОШИБКА В ПАРАМЕТРАХ ВЫЗЫВАЕМОЙ ФУНКЦИИ. НЕ СОВПАДАЮТ ТИПЫ";
					}
					break;
				case LEX_PRINT_STR:
					if (IT::GetEntry(it, lexem->next->idxTI)->iddatatype != IT::STR)
					{
						errors[errorCount++] = Error::geterrorin(705, lexem->sn, 0);
						errorMessage = "ОШИБКА В ПАРАМЕТРАХ ВЫЗЫВАЕМОЙ ФУНКЦИИ. НЕ СОВПАДАЮТ ТИПЫ";
					}
					break;
				case LEX_FUNCTION:
					f.function = true; // function body
					break;
				case LEX_RETURN:
					f.returnExpression = true;
					break;
				case LEX_BRACELET:
					if (f.repeat && !f.condition)
					{
						f.repeat = false;
						firstRepeatOrCondition = 0;
					}
					else if (!f.repeat && f.condition)
					{
						f.condition = false;
						firstRepeatOrCondition = 0;
					}
					else if (f.repeat && f.condition)
					{
						if (firstRepeatOrCondition == 1) f.condition = false;
						else if (firstRepeatOrCondition == 2) f.repeat = false;
					}
					else {
						f.returnExpression = false;
						f.expression = false;
						functionType = IT::EMPTY;
						expressionType = IT::EMPTY;
						f.main = false;
					}
					break;
				case(LEX_CYCLE):
					f.repeat = true;
					if (firstRepeatOrCondition == 0) firstRepeatOrCondition = 1;
					break;
				case(LEX_CONVERT):
					f.itos = true;
					break;
				}

				*log.stream << lexem->lexema[0];

				if (lexem->next && lexem->sn == lexem->next->sn) lexem = lexem->next;
				else break;
			} 
			if (errorMessage != "")
			{
				SEM_ERROR(errorMessage)
				errorMessage = "";
				if (errorCount == 3)
				{
					*log.stream << "\n\t\tОшибки семантики:";
					for (int i = 0; i < 3; i++)
					{
						*log.stream << "\n - Ошибка " << errors[i].id 
							<< ": " << errors[i].message 
							<< " Строка в исходном коде: " 
							<< errors[i].inHandler.line;
					}
					throw errors[0];
				}
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