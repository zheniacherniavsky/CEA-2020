#include "SemAnalyzer.h"

namespace SemAnalyzer
{
	bool semAnalyzer(LT::LexTable lt, IT::IdTable it)
	{
		LT::Entry *lexem = lt.head;

		while (lexem)
		{

			lexem = lexem->next;
		}

		return false;
	}
}