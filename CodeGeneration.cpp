#include "CodeGeneration.h"

namespace CG
{
	bool CodeGeneration(IT::IdTable& it, LT::LexTable& lt)
	{
		const char* asmFile_path = "code.asm";
		std::ofstream codeAsm;
		codeAsm.open(asmFile_path);
		if (!codeAsm.is_open())
			return false;

		const char* header = ".686\n.MODEL FLAT, C\n.STACK 4096\nExitProcess PROTO, :DWORD\n\n";
		codeAsm << header;

		CreateDataSegment(it, codeAsm);
		CreateConstSegment(it, codeAsm);
		CreateCodeSegment(it, lt, codeAsm);

		codeAsm.close();
		return true;
	}

	void CreateDataSegment(IT::IdTable& it, std::ofstream& codeAsm)
	{
		codeAsm << ".DATA\n";
		IT::Entry* element = it.head;
		while (element)
		{
			if (element->idtype == IT::V || element->idtype == IT::P)
				switch (element->iddatatype)
				{
				case(IT::INT): DATA_WRITE_INT; break;
				case(IT::STR): DATA_WRITE_STR; break;
				}
			element = element->next;
		}
	}

	void CreateConstSegment(IT::IdTable& it, std::ofstream& codeAsm)
	{
		codeAsm << ".CONST\n";
		IT::Entry* element = it.head;
		while (element)
		{
			if (element->idtype == IT::L)
				switch (element->iddatatype)
				{
				case(IT::INT): DATA_WRITE_INT; break;
				case(IT::STR): DATA_WRITE_STR; break;
				}
			element = element->next;
		}
	}

	void CreateCodeSegment(IT::IdTable& it, LT::LexTable lt, std::ofstream& codeAsm)
	{
		codeAsm << ".CONST\n";
		// EXTRN FUNCTIONS
		codeAsm << "\t; // ----------- EXTRN functions declarations -----------\n\n\n";
		// FUNCTIONS ...
		codeAsm << "\t; // ----------- codefunctions declaration -----------\n\n\n";
		codeAsm << "cea2020:\n"; // when we found enter point
		// CODE GENERATION
		codeAsm << "\t; // this special plase for code\n\tINVOKE ExitProcess, 0\n";

		codeAsm << "start:\n\tjmp cea2020\n"; // enter point to assembly
	}
}