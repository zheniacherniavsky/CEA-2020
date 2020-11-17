#include "SemAnalyzer.h"

namespace SemAnalyzer
{
	struct flag
	{
		bool declare = false;
		bool checkDivideByZero = false;
	};

	bool semAnalyzer(LT::LexTable lt, IT::IdTable it)
	{
		LT::Entry *lexem = lt.head; // таблица лексем
		int line = 0;				// для вывода анализа
		int errorCount = 0;			

		flag f;		// флажки

		enum Type	
		{
			INT = 1,
			STR = 2
		};

		short expressionType = 0;

		SEM_START
		while (lexem->next)
		{
			SEM_TRACE1
			while (lexem) // разбор строки
			{
				switch (lexem->lexema[0])
				{
				case LEX_ID:
					std::cout << getType(IT::GetEntry(it, lexem->idxTI));
					break;
				case LEX_DECLARE:
					// flag declare for line
					f.declare = true;
					break;
				case LEX_DIRSLASH:
					// null check
					f.checkDivideByZero = true;
					break;
				case LEX_IS:
					f.declare = false;
					break;
				}

				std::cout << lexem->lexema[0];
				if (lexem->next && lexem->sn == lexem->next->sn) lexem = lexem->next;
				else break;
			} 
			expressionType = 0;
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
}