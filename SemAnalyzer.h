#include "LT.h"
#include "IT.h"
#include <iostream>

#include <iomanip>
#define SEM_START		{std::cout << "\n\n" << std::left << std::setfill('_') << std::setw(60) << "������������� ����������" << std::endl;}

#define SEM_TRACE1		{std::cout << '\n' << std::setw(4) << line << ":\t";}

#define SEM_END		{	std::cout << '\n' << std::setw(60);																				\
						if(errorCount == 0) {																							\
							std::cout << "\n������������� ������ �����ب� ��� ������" << std::endl;										\
							return true;																								\
						}																												\
						else {																											\
							std::cout << "\n������������� ������ �����ب� C �������. ���-�� ������: " << errorCount << std::endl;		\
							return false;																								\
						}																												\
					}

namespace SemAnalyzer
{
	bool semAnalyzer(LT::LexTable lt, IT::IdTable it);
	const char* getType(IT::Entry* element);
}