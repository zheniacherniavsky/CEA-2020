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

		const char* header = ".486\n.MODEL FLAT, STDCALL\nincludelib kernel32.lib\nincludelib E:\\lib\\userlib.lib\n\nExitProcess PROTO, :DWORD\n";

		codeAsm << header;
		codeAsm << "outint PROTO, :DWORD ; 1 arg [int] // out int on console\n";
		
		codeAsm << "\n.STACK 4096\n"; // stack


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
		codeAsm << ".CODE\n";
		codeAsm << "; ---------- - Function definitions--------------------\n\n\n";

		IT::Entry* itElement = new IT::Entry();
		LT::Entry* element = lt.head;
		
		while (element->lexema[0] != 'm')
		{
			// functionsssss, idk how it do now lol
			element = element->next;
		}

		codeAsm << "cea2020 PROC\n";	// when we found enter point !!!
										// we start making expressions !!!

		

		while (element->next)
		// this cycle is lined cycle, it mean we get line and think that line is expression.
		// by this way we can remember our variables, and it is simple to understand... for me.
		// this algorythm like at SemAnalyzer.h 
		{
			char* id_of_first_var = NULL;
			while (element)
			{


				switch (element->lexema[0])
				{
				case(LEX_RETURN): // r i ; return exception
					element = element->next; // r -> i
					itElement = IT::GetEntry(it, element->idxTI);
					CODE_PUSH // push i
					codeAsm << "\t; // this is return of function: " << itElement->visibility.functionName << '\n';
					element = element->next; // i -> ; (then this semicolon go to next lexem)
					break;

				case(LEX_ID):
				case(LEX_LITERAL):
					itElement = IT::GetEntry(it, element->idxTI);
					if (id_of_first_var == NULL)
					{
						codeAsm << "\n\n\t; // this is " << itElement->id << " expression!";
						id_of_first_var = new char();
						for (int i = 0; i < strlen(itElement->id); i++)
							id_of_first_var[i] = itElement->id[i];
						id_of_first_var[strlen(itElement->id)] = 0x00;
						break;
					}
					else
					{
						CODE_PUSH
						break;
					}

				case(LEX_PLUS):
					CODE_PLUS
					break;

				case(LEX_STAR):
					CODE_MUL
					break;

				case(LEX_PRINT_INT):
					element = element->next; // p -> i
					itElement = IT::GetEntry(it, element->idxTI);
					CODE_PUSH // push i
					codeAsm << "\n\tcall\toutint ; // at console\n";
					element = element->next; // i -> ; (then this semicolon go to next lexem)
					break;
				case(LEX_DIRSLASH):
					CODE_DIV
					break;

				case(LEX_SEMICOLON):
					if (id_of_first_var != NULL)
					{
						CODE_POP
					}
					else break;

				}
				if (element->next && element->sn == element->next->sn) element = element->next;
				else break;
			}
			id_of_first_var = NULL;
			element = element->next;
		}

		codeAsm << "\tcall ExitProcess\n\nEXIT_DIV_ON_NULL:\n; // here is console output with error\ncea2020 ENDP\n";

		codeAsm << "\nmain PROC\n\tcall cea2020\nmain ENDP\n\nend main"; // enter point to assembly
	}
}