#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include "IT.h"
#include "LT.h"

#define DATA_WRITE_INT {codeAsm << '\t' << std::setw(5) << element->id << '\t' << "DD\t" << element->value.vint << '\n'; }
#define DATA_WRITE_STR {codeAsm << '\t' << std::setw(5) << element->id << '\t' << "DB\t\"" << element->value.vint << "\",0\n"; }

#define CODE_PUSH {codeAsm << "\n\tpush\t" << itElement->id;}
#define CODE_PLUS {codeAsm << "\n\tpop\teax\n\tpop\tebx\n\tadd\teax,\tebx\n\tpush\teax";}
#define CODE_MUL {codeAsm << "\n\tpop\teax\n\tpop\tebx\n\timul\tebx\n\tpush\teax";}
#define CODE_DIV {codeAsm << "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tidiv ebx\n\tpush eax";}
#define CODE_POP {codeAsm << "\n\tpop\t" << id_of_first_var << '\n';}

namespace CG
{
	bool CodeGeneration(IT::IdTable& it, LT::LexTable& lt);
	void CreateDataSegment(IT::IdTable& it, std::ofstream& codeAsm);
	void CreateConstSegment(IT::IdTable& it, std::ofstream& codeAsm);
	void CreateCodeSegment(IT::IdTable& it, LT::LexTable lt, std::ofstream& codeAsm);
}