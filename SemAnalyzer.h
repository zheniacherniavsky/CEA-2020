#include "LT.h"
#include "IT.h"
#include "Log.h"
#include <iostream>

#include <iomanip>
#define SEM_OUTTABLE_SIZE 100

#define SEM_START		{*log.stream << "\n\n" << std::left << std::setfill('_') \
<< std::setw(SEM_OUTTABLE_SIZE) << "СЕМАНТИЧЕСКИЙ АНАЛИЗАТОР" << std::endl;}

#define SEM_TRACE1		{*log.stream << '\n' << std::setw(4) << line << ":\t";}

#define SEM_ERROR(a)	{*log.stream << "\n\n" << std::setw(20) << "ОШИБКА В ЛИНИИ " << std::setw(4) << line << '\n' << std::setw(SEM_OUTTABLE_SIZE); \
						*log.stream << a << std::endl;}

#define SEM_END		{	*log.stream << '\n' << std::setw(SEM_OUTTABLE_SIZE);																\
						if(errorCount == 0) {																							\
							*log.stream << "\nСЕМАНТИЧЕСКИЙ АНАЛИЗ ЗАВЕРШЁН БЕЗ ОШИБОК" << std::endl;										\
							return true;																								\
						}																												\
						else {																											\
							*log.stream << "\nСЕМАНТИЧЕСКИЙ АНАЛИЗ ЗАВЕРШЁН C ОШИБКОЙ. КОЛ-ВО ОШИБОК: " << errorCount << std::endl;		\
							throw errors[0];																								\
						}																												\
					}

namespace SemAnalyzer
{
	bool semAnalyzer(LT::LexTable lt, IT::IdTable it, Log::LOG_ log);
	const char* getType(IT::Entry* element);
	const char* getType(IT::IDDATATYPE type);
}