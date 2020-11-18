#include "LT.h"
#include "IT.h"
#include <iostream>

#include <iomanip>
#define SEM_OUTTABLE_SIZE 100

#define SEM_START		{std::cout << "\n\n" << std::left << std::setfill('_') \
<< std::setw(SEM_OUTTABLE_SIZE) << "СЕМАНТИЧЕСКИЙ АНАЛИЗАТОР" << std::endl;}

#define SEM_TRACE1		{std::cout << '\n' << std::setw(4) << line << ":\t";}

#define SEM_ERROR(a)	{std::cout << "\n\n" << std::setw(20) << "ОШИБКА В ЛИНИИ " << std::setw(4) << line << '\n' << std::setw(SEM_OUTTABLE_SIZE); \
						std::cout << a << std::endl;}

#define SEM_END		{	std::cout << '\n' << std::setw(SEM_OUTTABLE_SIZE);																\
						if(errorCount == 0) {																							\
							std::cout << "\nСЕМАНТИЧЕСКИЙ АНАЛИЗ ЗАВЕРШЁН БЕЗ ОШИБОК" << std::endl;										\
							return true;																								\
						}																												\
						else {																											\
							std::cout << "\nСЕМАНТИЧЕСКИЙ АНАЛИЗ ЗАВЕРШЁН C ОШИБКОЙ. КОЛ-ВО ОШИБОК: " << errorCount << std::endl;		\
							return false;																								\
						}																												\
					}

namespace SemAnalyzer
{
	bool semAnalyzer(LT::LexTable lt, IT::IdTable it);
	const char* getType(IT::Entry* element);
}