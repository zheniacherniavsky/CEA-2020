#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include "IT.h"
#include "LT.h"

#define DATA_WRITE_INT {codeAsm << '\t' << std::setw(11) << element->visibility.functionName << '_' << element->id << '\t' << "SDWORD\t" << element->value.vint << '\n'; }
#define DATA_WRITE_STR {codeAsm << '\t' << std::setw(11) << element->visibility.functionName << '_' << element->id << '\t' << "BYTE 255 DUP(0)" << "\n"; }
#define CONST_WRITE_INT {codeAsm << '\t' << std::setw(11) << element->visibility.functionName << '_' << element->id << '\t' << "SDWORD\t" << element->value.vint << '\n'; }
#define CONST_WRITE_STR {codeAsm << '\t' << std::setw(11) << element->visibility.functionName << '_' << element->id << '\t' << "BYTE\t\"" << element->value.vstr->str << "\",0\n"; }

#define CODE_PUSH {codeAsm << "\n\tpush\t" << itElement->visibility.functionName << '_' << itElement->id;}
#define CODE_PUSH_OFFSET {codeAsm << "\n\tpush \toffset " << itElement->visibility.functionName << '_' << itElement->id;}
#define CODE_PUSH_OFFSET_FUNC {codeAsm << "\n\tpush\t[" << itElement->visibility.functionName << '_' << itElement->id << "]";}
#define CODE_PLUS {codeAsm << "\n\tpop\teax\n\tpop\tebx\n\tadd\teax,\tebx\n\tpush\teax";}
#define CODE_PLUS_STR {codeAsm << "\n\tcall strcon\n\tjo EXIT_OVERFLOW_STR\n\tpush\teax";}
#define CODE_MUL {codeAsm << "\n\tpop\teax\n\tpop\tebx\n\timul\tebx\n\tpush\teax";}
#define CODE_DIV {codeAsm << "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tidiv ebx\n\tpush eax";}
#define CODE_RDIV {codeAsm << "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tmov\tedx,\t0\n\tidiv ebx\n\tpush edx";}
#define CODE_DIFF {codeAsm << "\n\tpop eax\n\tneg eax\n\tpop ebx\n\tadd eax, ebx\n\tjo EXIT_OVERFLOW_INT\n\tpush eax";}

#define CODE_POP {codeAsm << "\n\tpop\t" << itElement->visibility.functionName << '_' << id_of_first_var; \
					codeAsm << "\n\tcmp " << itElement->visibility.functionName << '_' << id_of_first_var << ", 255"; \
					codeAsm << "\n\tjg EXIT_OVERFLOW_INT\n"; \
					codeAsm << "\n\tcmp " << itElement->visibility.functionName << '_' << id_of_first_var << ", 0"; \
					codeAsm << "\n\tjl EXIT_OVERFLOW_INT\n"; \
}
#define CODE_POP_STR {codeAsm << "\n\tpush\toffset " << itElement->visibility.functionName << '_' << id_of_first_var << "\n\tcall copystr" << "\n\t"; }
#define CODE_POP_STR_FUNC {codeAsm << "\n\tpush\t[" << itElement->visibility.functionName << '_' << id_of_first_var << "]\n\tcall copystr" << "\n\t"; }

namespace CG
{
	bool CodeGeneration(IT::IdTable& it, LT::LexTable& lt, char* outAsm);
	void CreateDataSegment(IT::IdTable& it, std::ofstream& codeAsm);
	void CreateConstSegment(IT::IdTable& it, std::ofstream& codeAsm);
	void CreateCodeSegment(IT::IdTable& it, LT::LexTable lt, std::ofstream& codeAsm);
}