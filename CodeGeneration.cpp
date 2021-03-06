#include "CodeGeneration.h"

namespace CG
{
	bool CodeGeneration(IT::IdTable& it, LT::LexTable& lt, char* outPath)
	{
		std::ofstream codeAsm;
		codeAsm.open(outPath);
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
		codeAsm << "EXTRN strequal : PROC\n";
		codeAsm << "EXTRN _pause : PROC\n";

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
		codeAsm << "\toverflow_str db 'ERROR: STRING VARIABLE OVERFLOW', 0\n";
		codeAsm << "\toverflow_int db 'ERROR: INTEGER VARIABLE OVERFLOW. INTEGER INTERVAL : [ 0 - 255 ]', 0\n";
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
		bool condition = false;
		bool is = false;
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

		char cond;			// запоминать условие
		int conditionsCount = 0;

		IT::IDTYPE var_type = IT::F;
		IT::IDTYPE first_var_type = IT::F;
		IT::IDDATATYPE first_var_dtype = IT::EMPTY;
		char* id_of_first_var = NULL;

		char* functionName = NULL;

		while (element->next)
		{
			switch (element->lexema[0])
			{
			case(LEX_IS):
				f.is = true;
				break;
			case(LEX_MAIN):
				f.enterPoint = true;
				codeAsm << "\n\ncea2020 PROC\n";
				break;
			case(LEX_CONDITION):
				conditionsCount++;

				codeAsm << "\n; // condition\n";
				f.condition = true;
				element = element->next; // : -> (
				element = element->next; // ( -> Q
				firstArg = IT::GetEntry(it, element->idxTI);
				if (firstArg->iddatatype == IT::INT)
				{
					element = element->next; // Q -> C
					cond = element->lexema[0];
					element = element->next; // C -> Q
					secondArg = IT::GetEntry(it, element->idxTI);
					codeAsm << "\n\tmov eax, " << firstArg->visibility.functionName << '_' << firstArg->id;
					codeAsm << "\n\tcmp eax, ";
					codeAsm << secondArg->visibility.functionName << '_' << secondArg->id;
					switch (cond)
					{
					case(LEX_LESS):
						codeAsm << "\n\tjg EXIT_CONDITION" << conditionsCount;
						break;
					case(LEX_OVER):
						codeAsm << "\n\tjl EXIT_CONDITION" << conditionsCount;
						break;
					case(LEX_EQUAL):
						codeAsm << "\n\tjne EXIT_CONDITION" << conditionsCount;
						break;
					}
				}
				// else if STR, it's mean we want check equal 
				// (because we have only one rule in semAnalyz for str)
				else if (firstArg->iddatatype == IT::STR)
				{
					element = element->next; // Q -> C
					element = element->next; // C -> Q
					secondArg = IT::GetEntry(it, element->idxTI);
					codeAsm << "\n\tpush offset " << firstArg->visibility.functionName << '_' << firstArg->id;
					codeAsm << "\n\tpush offset " << secondArg->visibility.functionName << '_' << secondArg->id;
					codeAsm << "\n\tcall strequal";
					codeAsm << "\n\tcmp eax, 0";
					codeAsm << "\n\tje EXIT_CONDITION" << conditionsCount;
				}
				break;
			case(LEX_AND):
				codeAsm << "\n\tcall _AND\n\tpush\teax";
				break;

			case(LEX_OR):
				codeAsm << "\n\tcall _OR\n\tpush\teax";
				break;

			case(LEX_INVERT):
				element = element->next; // ~ -> i
				itElement = IT::GetEntry(it, element->idxTI);
				CODE_PUSH
				codeAsm << "\n\tcall _NOT";
				break;

			case(LEX_POW):	// pow(Q, Q) function with 2 agrs
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
			case(LEX_ROOT):	// root(Q, Q) function with 2 agrs
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
			case(LEX_FUNCTION):
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
			case(LEX_CONVERT):
				element = element->next; // itos -> (
				element = element->next; // ( -> INT var
				itElement = IT::GetEntry(it, element->idxTI);
				CODE_PUSH // push INT
				codeAsm << "\n\tcall\ttostr\n\tpush\teax";
				element = element->next; // INT -> )
				break;
			case(LEX_CYCLE):
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
					if (itElement->iddatatype == IT::INT) codeAsm << "\n\tmov eax, " << itElement->visibility.functionName << '_' << itElement->id;
					else if (itElement->iddatatype == IT::STR && itElement->idtype == IT::P) codeAsm << "\n\tmov eax, [" << itElement->visibility.functionName << '_' << itElement->id;
					else if (itElement->iddatatype == IT::STR) codeAsm << "\n\tmov eax, offset " << itElement->visibility.functionName << '_' << itElement->id;
					codeAsm << "\n\tret";
				}
				else if (f.enterPoint)
				{
					element = element->next; // r -> i
					itElement = IT::GetEntry(it, element->idxTI);
					codeAsm << "\n\tcall _pause";
					if (itElement->iddatatype == IT::INT) CODE_PUSH // push i
					else if (itElement->iddatatype == IT::STR) CODE_PUSH_OFFSET // push i
				}
				codeAsm << "\t; // this is return of function: " << itElement->visibility.functionName << '\n';
				break;
			case(LEX_ID):
			case(LEX_LITERAL):
			case(LEX_DOG):
				itElement = IT::GetEntry(it, element->idxTI);
				var_type = itElement->idtype;
				if (id_of_first_var == NULL)
				{
					if (element->next->lexema[0] != LEX_SEMICOLON)
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

						first_var_dtype = itElement->iddatatype;
						first_var_type = itElement->idtype;
					}
					break;
				}
				else if (element->lexema[0] != LEX_DOG)
				{
					if (first_var_dtype == IT::INT) CODE_PUSH
					else if (var_type == IT::P && f.function && first_var_dtype == IT::STR) CODE_PUSH_OFFSET_FUNC
					else if (first_var_dtype == IT::STR) CODE_PUSH_OFFSET
					break;
				}
				else if (element->lexema[0] == LEX_DOG)
				{
					char* fname = new char();
					itElement = IT::GetEntry(it, element->idxTI);
					IT::IDDATATYPE t = itElement->iddatatype;
					for (int i = 0; i < strlen(itElement->visibility.functionName); i++)
						fname[i] = itElement->visibility.functionName[i];
					fname[strlen(itElement->visibility.functionName)] = 0x00;

					for (int i = element->func.count - 1; i >= 0; i--)
					{
						itElement = IT::GetEntry(it, element->func.idx[i]);
						if (itElement->iddatatype == IT::INT) CODE_PUSH
						else if (itElement->iddatatype == IT::STR) CODE_PUSH_OFFSET
					}

					codeAsm << "\n\tcall\t" << fname;
					codeAsm << "\n\tpush\teax";
					break;
				}
			case(LEX_PLUS):
				if (first_var_dtype == IT::INT) CODE_PLUS
				else if (first_var_dtype == IT::STR) CODE_PLUS_STR
				break;
			case(LEX_MINUS):
				if (first_var_dtype == IT::INT) CODE_DIFF
				break;
			case(LEX_STAR):
				if(first_var_dtype == IT::INT) CODE_MUL
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
					if (f.function && itElement->idtype == IT::P)
					{
						CODE_PUSH_OFFSET_FUNC
							codeAsm << "\n\tcall\toutstr ; // at console\n";
					}
					else
					{
						CODE_PUSH_OFFSET // push i
							codeAsm << "\n\tcall\toutstr ; // at console\n";
					}
				}
				element = element->next; // i -> ; (then this semicolon go to next lexem)
				break;
			case(LEX_DIRSLASH):
				CODE_DIV
				break;
			case(LEX_PERCENT):
				CODE_RDIV
				break;
			case(LEX_SEMICOLON):
				if (f.is && id_of_first_var != NULL)
				{
					f.is = false;
					if (first_var_dtype == IT::INT) CODE_POP
					else if (first_var_dtype == IT::STR)
					{
						if (first_var_type == IT::P)
							CODE_POP_STR_FUNC
						else
						{
							CODE_POP_STR
						}
					}
				}
				id_of_first_var = NULL;
				first_var_dtype = IT::EMPTY;
				break;
			case(LEX_BRACELET):
				if (f.condition && !f.repeat)
				{
					f.condition = false;
					codeAsm << "EXIT_CONDITION" << conditionsCount << ":\n";
				}
				else if (f.repeat)
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
			element = element->next;
		}

		codeAsm << "\n\tjmp EXIT\ncea2020 ENDP\n\nEXIT_DIV_ON_NULL:\n\tpush offset null_division\n\tcall outstr\n\tpush - 1\n\tcall ExitProcess";
		codeAsm << "\n\nEXIT_OVERFLOW_STR:\n\tpush offset overflow_str\n\tcall outstr\n\tpush - 2\n\tcall ExitProcess\n";
		codeAsm << "\n\nEXIT_OVERFLOW_INT:\n\tpush offset overflow_int\n\tcall outstr\n\tpush - 2\n\tcall ExitProcess\n";
		codeAsm << "\n\nEXIT:\n\tcall ExitProcess\n\nEND start";
	}
}