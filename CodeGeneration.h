#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include "IT.h"
#include "LT.h"

#define DATA_WRITE_INT {codeAsm << '\t' << std::setw(5) << element->id << '\t' << "DD\t" << element->value.vint << '\n'; }
#define DATA_WRITE_STR {codeAsm << '\t' << std::setw(5) << element->id << '\t' << "DD\t\"" << element->value.vint << "\",0\n"; }

namespace CG
{
	bool CodeGeneration(IT::IdTable& it, LT::LexTable& lt);
	void CreateDataSegment(IT::IdTable& it, std::ofstream& codeAsm);
	void CreateConstSegment(IT::IdTable& it, std::ofstream& codeAsm);
	void CreateCodeSegment(IT::IdTable& it, LT::LexTable lt, std::ofstream& codeAsm);
	
}