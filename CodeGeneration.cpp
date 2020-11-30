#include "CodeGeneration.h"

namespace CG
{
	bool CodeGeneration(IT::IdTable& it, LT::LexTable& lt)
	{
		const char* asmFile_path = "ASM.asm";
		std::ofstream codeAsm;
		codeAsm.open(asmFile_path);
		if (!codeAsm.is_open())
			return false;

		const char* header = ".486\n.MODEL FLAT, STDCALL\nincludelib kernel32.lib\nincludelib libucrt.lib\nincludelib E:\\lib\\userlib.lib\n\nExitProcess PROTO, :DWORD\n";

		codeAsm << header;
		codeAsm << "EXTRN outint : PROC\n";
		codeAsm << "EXTRN outstr : PROC\n";
		codeAsm << "EXTRN copystr : PROC\n";
		codeAsm << "EXTRN strcon : PROC\n";
		codeAsm << "EXTRN ConvertToChar : PROC\n";
		
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
		codeAsm << "\toverflow db 'ERROR: VARIABLE OVERFLOW', 0\n";
		codeAsm << "\tnull_division db 'ERROR: DIVISION BY ZERO', 0\n";

		IT::Entry* element = it.head;
		while (element)
		{
			if (element->idtype == IT::L)
				switch (element->iddatatype)
				{
				case(IT::INT): CONST_WRITE_INT; break;
				case(IT::STR): CONST_WRITE_STR; break;
				}
			element = element->next;
		}
	}

	struct flags
	{
		bool repeat = false;
	};

	void CreateCodeSegment(IT::IdTable& it, LT::LexTable lt, std::ofstream& codeAsm)
	{
		flags f;

		codeAsm << ".CODE\n";

		codeAsm << "\nstart:\nmain PROC\n\tcall cea2020\nmain ENDP\n\n"; // enter point to assembly

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
			IT::IDDATATYPE first_var_type = IT::EMPTY;
			while (element)
			{


				switch (element->lexema[0])
				{
				case('c'):
					element = element->next; // itos -> (
					element = element->next; // ( -> INT var
					itElement = IT::GetEntry(it, element->idxTI);
					CODE_PUSH // push INT
					codeAsm << "\n\tcall\tConvertToChar\n\tpush\teax";
					element = element->next; // INT -> )
					break;
				case('w'):
					f.repeat = true;
					element = element->next; // repeat -> (
					element = element->next; // ( -> CYCLE COUNT
					itElement = IT::GetEntry(it, element->idxTI);
					codeAsm << "\nREPEAT " << itElement->value.vint;
					break;
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
						if (element->next->lexema[0] == ';' && itElement->iddatatype == IT::STR) break;

						codeAsm << "\n\n\t; // this is _" << itElement->id << " expression!";
						if (itElement->iddatatype == IT::INT)
							codeAsm << " int";
						else if (itElement->iddatatype == IT::STR) codeAsm << " str";

						id_of_first_var = new char();
						for (int i = 0; i < strlen(itElement->id); i++)
							id_of_first_var[i] = itElement->id[i];
						id_of_first_var[strlen(itElement->id)] = 0x00;

						first_var_type = itElement->iddatatype;

						if (element->next->lexema[0] == LEX_SEMICOLON) codeAsm << "\n\tpush 0";
						break;
					}
					else
					{
						if (first_var_type == IT::INT) CODE_PUSH
						else if (first_var_type == IT::STR) CODE_PUSH_OFFSET
						break;
					}

				case(LEX_PLUS):
					if (first_var_type == IT::INT) CODE_PLUS
					else if (first_var_type == IT::STR) CODE_PLUS_STR
					break;
				case(LEX_MINUS):
					if (first_var_type == IT::INT) CODE_DIFF
					break;
				case(LEX_STAR):
					if(first_var_type == IT::INT) CODE_MUL
					break;

				case(LEX_PRINT_INT):
					element = element->next; // p -> i
					itElement = IT::GetEntry(it, element->idxTI);
					if (itElement->iddatatype == IT::INT)
					{
						CODE_PUSH // push i
							codeAsm << "\n\tcall\toutint ; // at console\n";
					}
					element = element->next; // i -> ; (then this semicolon go to next lexem)
					break;
				case(LEX_PRINT_STR):
					element = element->next; // p -> i
					itElement = IT::GetEntry(it, element->idxTI);
					if (itElement->iddatatype == IT::STR)
					{
						CODE_PUSH_OFFSET // push i
							codeAsm << "\n\tcall\toutstr ; // at console\n";
					}
					element = element->next; // i -> ; (then this semicolon go to next lexem)
					break;
				case(LEX_DIRSLASH):
					CODE_DIV
					break;

				case(LEX_SEMICOLON):
					if (id_of_first_var != NULL)
					{
						if (first_var_type == IT::INT) CODE_POP
						else if (first_var_type == IT::STR) CODE_POP_STR
					}
					else break;
				case(LEX_BRACELET):
					if (f.repeat)
					{
						f.repeat = false;
						codeAsm << "\nENDM\n";
					}
					break;
				}
				if (element->next && element->sn == element->next->sn) element = element->next;
				else break;
			}

			id_of_first_var = NULL;
			first_var_type = IT::EMPTY;
			element = element->next;
		}

		codeAsm << "\n\tjmp EXIT\n\nEXIT_DIV_ON_NULL:\n\tpush offset null_division\n\tcall outstr\n\tpush - 1\n\tcall ExitProcess";
		codeAsm << "\n\nEXIT_OVERFLOW:\n\tpush offset overflow\n\tcall outstr\n\tpush - 2\n\tcall ExitProcess\n\n\tEXIT:";
		codeAsm << "\tcall ExitProcess\ncea2020 ENDP\nEND start";
	}
}