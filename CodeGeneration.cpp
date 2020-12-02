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
		codeAsm << "EXTRN powN : PROC\n";
		codeAsm << "EXTRN rootN : PROC\n";
		codeAsm << "EXTRN _AND : PROC\n";
		codeAsm << "EXTRN _OR : PROC\n";
		codeAsm << "EXTRN _NOT : PROC\n";
		codeAsm << "EXTRN tostr : PROC\n";

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
			if (element->idtype == IT::V)
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
		bool enterPoint = false;
		bool function = false;
	};

	void CreateCodeSegment(IT::IdTable& it, LT::LexTable lt, std::ofstream& codeAsm)
	{
		flags f;

		codeAsm << ".CODE\n";

		codeAsm << "\nstart:\nmain PROC\n\tcall cea2020\nmain ENDP\n\n"; // enter point to assembly

		codeAsm << "; ---------- - Function definitions--------------------\n\n\n";

		IT::Entry* itElement = new IT::Entry();
		IT::Entry* firstArg = new IT::Entry();
		IT::Entry* secondArg = new IT::Entry();
		LT::Entry* element = lt.head;

		char* functionName = NULL;

		while (element->next)
		// this cycle is lined cycle, it mean we get line and think that line is expression.
		// by this way we can remember our variables, and it is simple to understand... for me.
		// this algorythm like at SemAnalyzer.h 
		{
			if (element->lexema[0] == 'm')
			{
				f.enterPoint = true;
				codeAsm << "\n\ncea2020 PROC\n";
			}

			char* id_of_first_var = NULL;

			IT::IDDATATYPE first_var_type = IT::EMPTY;
			while (element)
			{
				switch (element->lexema[0])
				{

				case('&'):
					codeAsm << "\n\tcall _AND\n\tpush\teax";
					break;

				case('|'):
					codeAsm << "\n\tcall _OR\n\tpush\teax";
					break;

				case('~'):
					element = element->next; // ~ -> i
					itElement = IT::GetEntry(it, element->idxTI);
					CODE_PUSH
					codeAsm << "\n\tcall _NOT";
					break;

				case('$'):	// pow(Q, Q) function with 2 agrs
					element = element->next; // ^ -> (
					element = element->next; // ( -> Q
					firstArg = IT::GetEntry(it, element->idxTI);
					element = element->next; // i -> ,
					element = element->next; // , -> i
					secondArg = IT::GetEntry(it, element->idxTI);
					// stdcall -> we push second arg, then first arg!!
					codeAsm << "\n\tpush\t" << secondArg->visibility.functionName
						<< '_' << secondArg->id;
					codeAsm << "\n\tpush\t" << firstArg->visibility.functionName
						<< '_' << firstArg->id;
					codeAsm << "\n\tcall\tpowN";
					codeAsm << "\n\tpush\teax";
					element = element->next; // i -> )
					break;
				case('q'):	// root(Q, Q) function with 2 agrs
					element = element->next; // q -> (
					element = element->next; // ( -> Q
					firstArg = IT::GetEntry(it, element->idxTI);
					element = element->next; // i -> ,
					element = element->next; // , -> i
					secondArg = IT::GetEntry(it, element->idxTI);
					// stdcall -> we push second arg, then first arg!!
					codeAsm << "\n\tpush\t" << secondArg->visibility.functionName
						<< '_' << secondArg->id;
					codeAsm << "\n\tpush\t" << firstArg->visibility.functionName
						<< '_' << firstArg->id;
					codeAsm << "\n\tcall\trootN";
					codeAsm << "\n\tpush\teax";
					element = element->next; // i -> )
					break;
				case('f'):
					element = element->next; // function -> i
					itElement = IT::GetEntry(it, element->idxTI);

					functionName = new char();
					for (int i = 0; i < strlen(itElement->visibility.functionName); i++)
						functionName[i] = itElement->visibility.functionName[i];
					functionName[strlen(itElement->visibility.functionName)] = 0x00;
					element = element->next; // i -> (
					f.function = true;
					codeAsm << functionName << " PROC ";
					while (element->lexema[0] != ')')
					{
						if (element->lexema[0] == LEX_ID)
						{
							itElement = IT::GetEntry(it, element->idxTI);
							codeAsm << itElement->visibility.functionName << "_" << itElement->id << " :";
							if (itElement->iddatatype == IT::INT) codeAsm << "SDWORD";
							else if (itElement->iddatatype == IT::STR) codeAsm << "DWORD";
							if (element->next->lexema[0] != ')') codeAsm << ", ";
						}
						element = element->next;
					}
					codeAsm << "\n";
					break;
				case('c'):
					element = element->next; // itos -> (
					element = element->next; // ( -> INT var
					itElement = IT::GetEntry(it, element->idxTI);
					CODE_PUSH // push INT
					codeAsm << "\n\tcall\ttostr\n\tpush\teax";
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
					if (f.function)
					{
						element = element->next; // r -> i
						itElement = IT::GetEntry(it, element->idxTI);
						if (itElement->iddatatype == IT::INT) codeAsm << "\n\tpush " << itElement->visibility.functionName << '_' << itElement->id;
						else if (itElement->iddatatype == IT::STR) codeAsm << "\n\tmov eax, offset " << itElement->visibility.functionName << '_' << itElement->id << "\n\tpush\teax";
						codeAsm << "\n\tret";
					}
					else if (f.enterPoint)
					{
						element = element->next; // r -> i
						itElement = IT::GetEntry(it, element->idxTI);
						if (itElement->iddatatype == IT::INT) CODE_PUSH // push i
						else if (itElement->iddatatype == IT::STR) CODE_PUSH_OFFSET // push i
					}
					codeAsm << "\t; // this is return of function: " << itElement->visibility.functionName << '\n';
					break;
				case(LEX_ID):
				case(LEX_LITERAL):
				case('@'):
					itElement = IT::GetEntry(it, element->idxTI);
					if (id_of_first_var == NULL)
					{
						if (element->next->lexema[0] == ';' && itElement->iddatatype == IT::STR) break;

						codeAsm << "\n\n\t; // this is " << itElement->visibility.functionName << '_' << itElement->id << " expression!";
						if (itElement->iddatatype == IT::INT)
							codeAsm << " int";
						else if (itElement->iddatatype == IT::STR) codeAsm << " str";

						id_of_first_var = new char();
						for (int i = 0; i < strlen(itElement->id); i++)
							id_of_first_var[i] = itElement->id[i];
						id_of_first_var[strlen(itElement->id)] = 0x00;

						first_var_type = itElement->iddatatype;

						// if (element->next->lexema[0] == LEX_SEMICOLON) codeAsm << "\n\tpush 0";
						break;
					}
					else if (element->lexema[0] != '@')
					{
						if (first_var_type == IT::INT) CODE_PUSH
						else if (f.function && first_var_type == IT::STR) CODE_PUSH_OFFSET_FUNC
						else if (first_var_type == IT::STR) CODE_PUSH_OFFSET
						break;
					}
					else if (element->lexema[0] == '@')
					{
						char* fname = new char();
						itElement = IT::GetEntry(it, element->idxTI);
						for (int i = 0; i < strlen(itElement->visibility.functionName); i++)
							fname[i] = itElement->visibility.functionName[i];
						fname[strlen(itElement->visibility.functionName)] = 0x00;

						for (int i = element->func.count - 1; i >= 0; i--)
						{
							itElement = IT::GetEntry(it, element->func.idx[i]);
							if (first_var_type == IT::INT) CODE_PUSH
							else if (first_var_type == IT::STR) CODE_PUSH_OFFSET
						}

						codeAsm << "\n\tcall\t" << fname;
						codeAsm << "\n\tpush\teax";
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
				case('%'):
					CODE_RDIV
					break;
				case(LEX_SEMICOLON):
					if (id_of_first_var != NULL)
					{
						if (first_var_type == IT::INT) CODE_POP
						else if (first_var_type == IT::STR) CODE_POP_STR
						break;
					}
					else break;
				case(LEX_BRACELET):
					if (f.repeat)
					{
						f.repeat = false;
						codeAsm << "\nENDM\n";
					}
					else if (f.function)
					{
						f.function = false;
						codeAsm << functionName << " ENDP\n\n";
						functionName = NULL;
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

		codeAsm << "\n\tjmp EXIT\ncea2020 ENDP\n\nEXIT_DIV_ON_NULL:\n\tpush offset null_division\n\tcall outstr\n\tpush - 1\n\tcall ExitProcess";
		codeAsm << "\n\nEXIT_OVERFLOW:\n\tpush offset overflow\n\tcall outstr\n\tpush - 2\n\tcall ExitProcess\n\nEXIT:";
		codeAsm << "\tcall ExitProcess\n\nEND start";
	}
}