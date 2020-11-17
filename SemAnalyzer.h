#include "LT.h"
#include "IT.h"
#include <iostream>

#include <iomanip>
#define SEM_START		{std::cout << "\n\n" << std::left << std::setfill('_') << std::setw(60) << "ÑÅÌÀÍÒÈ×ÅÑÊÈÉ ÀÍÀËÈÇÀÒÎÐ" << std::endl;}

#define SEM_TRACE1		{std::cout << '\n' << std::setw(4) << line << ":\t";}

#define SEM_END		{	std::cout << '\n' << std::setw(60);																				\
						if(errorCount == 0) {																							\
							std::cout << "\nÑÅÌÀÍÒÈ×ÅÑÊÈÉ ÀÍÀËÈÇ ÇÀÂÅÐØ¨Í ÁÅÇ ÎØÈÁÎÊ" << std::endl;										\
							return true;																								\
						}																												\
						else {																											\
							std::cout << "\nÑÅÌÀÍÒÈ×ÅÑÊÈÉ ÀÍÀËÈÇ ÇÀÂÅÐØ¨Í C ÎØÈÁÊÎÉ. ÊÎË-ÂÎ ÎØÈÁÎÊ: " << errorCount << std::endl;		\
							return false;																								\
						}																												\
					}

namespace SemAnalyzer
{
	bool semAnalyzer(LT::LexTable lt, IT::IdTable it);
	const char* getType(IT::Entry* element);
}