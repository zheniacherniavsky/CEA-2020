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
		bool itos = false;
	};

	bool semAnalyzer(LT::LexTable lt, IT::IdTable it, Log::LOG_ log)
	{
		LT::Entry *lexem = lt.head; // таблица лексем
		int line = 0;				// для вывода анализа	
		int errorCount = 0;
		std::string errorMessage = "";
		Error::ERROR_ errors[3];

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
					*log.stream << getType(element);

					if (element->declared && f.declare && element->idtype == IT::V)
					{
						errors[errorCount++] = Error::geterrorin(700, lexem->sn, 0);
						errorMessage = "ЭТА ПЕРЕМЕННА УЖЕ БЫЛА ОБЪЯВЛЕНА";
					}
					else if (!element->declared && !f.declare && element->idtype == IT::V)
					{
						errors[errorCount++] = Error::geterrorin(701, lexem->sn, 0);
						errorMessage = "ВЫЗОВ НЕОБЪЯВЛЕННОЙ ПЕРЕМЕННОЙ";
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
							}
							else
							{
								errors[errorCount++] = Error::geterrorin(703, lexem->sn, 0);
								errorMessage = "ВОЗРАЩАЕМОЕ ЗНАЧЕНИЕ НЕ СОВПАДАЕТ С ТИПОМ ФУНКЦИИ (ТИП ФУНКЦИИ MAIN -> INT)";
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
						if (lexem->lexema[0] == '@') // function check types
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