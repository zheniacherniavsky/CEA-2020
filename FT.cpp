#include "FT.h"

namespace FT
{
	struct flags
	{
		bool _declare		= false; // флаг на объ€влени€ переменной
		bool _literal		= false; // флаг на опознавание литерала
		bool _function		= false; // функци€
		bool _integer		= false; // int
		bool _string		= false; // string
		bool _hesisIsOpen	= false; // открытие закрытие скобок дл€ отличени€ параметров от функции
	};

	void fillTables(char* code, LT::LexTable &lt, IT::IdTable &it)
	{
		int* posArray = getLineNums(code);		// массив позиций строк исходного кода
		char* lexem = strtok(&code[0], " \n");	// разбиваю код на лексемы
		int pos = 0;							// номер обрабатываемой лексемы
		int idx = 0;							// id идентификатора
		flags flag;

		// добавление в таблицу лексем
		while (lexem != NULL)
		{
			char lexemID[ID_MAXSIZE];
			for (int i = 0; i < ID_MAXSIZE; i++)
				lexemID[i] = lexem[i];

			LT::Entry element;
			char lexemSymbol = compareLexems(lexem);
			if (lexemSymbol != 'i' && lexemSymbol != 'n') // если не идентификатор
			{
				element.idxTI = IT_NULL_IDX;		// нет элемента в таблице идентификаторов
				element.lexema[0] = lexemSymbol;	// символ лексемы, полученный из автомата 
				element.sn = posArray[pos];			// добавл€ю номер строки в исходном коде
				pos++;

				// установка флажков
				switch(lexemSymbol)
				{
				case('d'):
					flag._declare = true;
					break;
				case('f'):
					flag._function = true;
					break;
				case('t'):
					if (lexem[0] == 's') // дешева€ проверка на тип ыыыыыыы
					{
						flag._string = true;
						break;
					}
					else
					{
						flag._integer = true;
						break;
					}
				case('('):
					flag._hesisIsOpen = true;
					break;
				case(')'):
					flag._hesisIsOpen = false;
					flag._function = false;
					break;
				}

				LT::Add(lt, element);				// добавление в таблицу лексем
			}

			else if (lexemSymbol == ';' || lexemSymbol == '{')
			{
				flag._declare = false;
				flag._literal = false;
				flag._function = false;
				flag._integer = false;
				flag._string = false;
			}

			else // если идентификатор
			{
				element.lexema[0] = lexemSymbol;
				if (lexemSymbol != 'n' && flag._declare)
				{
					element.idxTI = idx;
				}
				else {
					element.idxTI = IT::IsId(it, lexemID);
				}
				if (lexem[0] == '\'' || lexem[0] == '\"')
				{
					flag._literal = true; // проверка на литерал
					element.lexema[0] = 'l';
					lexemSymbol = 'l';
				}
				element.sn = posArray[pos];
				pos++;

				if (lexemSymbol != 'n' && lexemSymbol != 'l') // если не число
				{
					if (flag._declare) // если идЄт объ€вление
					{
						IT::Entry idElement;
						for (int i = 0; i < ID_MAXSIZE; i++)
						{
							idElement.id[i] = lexemID[i];
						}
						idElement.idtype = IT::V; // переменна€

						if(flag._integer)
							idElement.iddatatype = IT::INT;
						else if (flag._string)
							idElement.iddatatype = IT::STR;

						idElement.idxTI = idx;
						idx++;
						idElement.idxfirstLE = posArray[pos-1]; // позици€ первого вхождени€
						idElement.value.vint = 0;
						idElement.value.vstr->len = 0;
						idElement.value.vstr->str[0] = '\0';

						IT::Add(it, idElement);
						flag._declare = false;
					}
					else if (flag._function)
					{
						IT::Entry idElement;
						for (int i = 0; i < ID_MAXSIZE; i++)
						{
							idElement.id[i] = lexemID[i];
						}
						
						if (flag._hesisIsOpen) idElement.idtype = IT::P; // переменна€
						else idElement.idtype = IT::F; // функци€

						if (flag._integer)
						{
							idElement.iddatatype = IT::INT;
							flag._integer = false;
						}
						else if (flag._string)
						{
							idElement.iddatatype = IT::STR;
							flag._string = false;
						}

						idElement.idxTI = idx;
						element.idxTI = idx;
						idx++;				
						idElement.idxfirstLE = posArray[pos - 1]; // позици€ первого вхождени€
						idElement.value.vint = 0;
						idElement.value.vstr->len = 0;
						idElement.value.vstr->str[0] = '\0';

						IT::Add(it, idElement);
					}
				}
				LT::Add(lt, element);
			}
			std::cout << lexem << std::endl;
			lexem = strtok(NULL, " \n");
		}
	}

	int* getLineNums(std::string code) // функци€ вычислени€ номера строки исходного кода дл€ его лексем
	{
		int* array = new int[MAX_LEXEMS_LENGTH];
		int size = 0;
		for (int i = 0, line = 0, pos = 0; i < code.length(); i++)
		{
			if (code[i] != ' ')
			{
				if (code[i] == '\n') {
					line++;
					continue;
				}

				array[pos] = line;
				pos++; size++;
				while (code[i] != ' ') {
					if (code[i] == '\n' || code[i] == ' ') {
						i--;
						break;
					}
					i++;
				}
			}
		}
		return array;
	}

	char compareLexems(char* lexem) {

		FST::FST fst1(lexem, 8,		//integer
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('n', 2)),
			FST::NODE(2, FST::RELATION('t', 3), FST::RELATION('t', 7)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE(1, FST::RELATION('g', 5)),
			FST::NODE(1, FST::RELATION('e', 6)),
			FST::NODE(1, FST::RELATION('r', 7)),
			FST::NODE()
		);

		FST::FST fst2(lexem, 7,		//string
			FST::NODE(1, FST::RELATION('s', 1)),
			FST::NODE(1, FST::RELATION('t', 2)),
			FST::NODE(1, FST::RELATION('r', 3)),
			FST::NODE(1, FST::RELATION('i', 4)),
			FST::NODE(1, FST::RELATION('n', 5)),
			FST::NODE(1, FST::RELATION('g', 6)),
			FST::NODE()
		);

		FST::FST fst3(lexem, 9,		//function
			FST::NODE(1, FST::RELATION('f', 1)),
			FST::NODE(1, FST::RELATION('u', 2)),
			FST::NODE(1, FST::RELATION('n', 3)),
			FST::NODE(1, FST::RELATION('c', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE(1, FST::RELATION('i', 6)),
			FST::NODE(1, FST::RELATION('o', 7)),
			FST::NODE(1, FST::RELATION('n', 8)),
			FST::NODE()
		);

		FST::FST fst4(lexem, 8,		//declare
			FST::NODE(1, FST::RELATION('d', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('c', 3)),
			FST::NODE(1, FST::RELATION('l', 4)),
			FST::NODE(1, FST::RELATION('a', 5)),
			FST::NODE(1, FST::RELATION('r', 6)),
			FST::NODE(1, FST::RELATION('e', 7)),
			FST::NODE()
		);

		FST::FST fst5(lexem, 7,		//return
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('u', 4)),
			FST::NODE(1, FST::RELATION('r', 5)),
			FST::NODE(1, FST::RELATION('n', 6)),
			FST::NODE()
		);

		FST::FST fst6(lexem, 6,		//print
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE()
		);

		FST::FST fst7(lexem, 2,		//;
			FST::NODE(1, FST::RELATION(';', 1)),
			FST::NODE()
		);

		FST::FST fst8(lexem, 2,		//,
			FST::NODE(1, FST::RELATION(',', 1)),
			FST::NODE()
		);

		FST::FST fst9(lexem, 2,		//{
			FST::NODE(1, FST::RELATION('{', 1)),
			FST::NODE()
		);

		FST::FST fst10(lexem, 2,		//}
			FST::NODE(1, FST::RELATION('}', 1)),
			FST::NODE()
		);

		FST::FST fst11(lexem, 2,		//(
			FST::NODE(1, FST::RELATION('(', 1)),
			FST::NODE()
		);

		FST::FST fst12(lexem, 2,		//)
			FST::NODE(1, FST::RELATION(')', 1)),
			FST::NODE()
		);

		FST::FST fst13(lexem, 2,		//+
			FST::NODE(1, FST::RELATION('+', 1)),
			FST::NODE()
		);

		FST::FST fst14(lexem, 2,		//-
			FST::NODE(1, FST::RELATION('-', 1)),
			FST::NODE()
		);

		FST::FST fst15(lexem, 2,		//*
			FST::NODE(1, FST::RELATION('*', 1)),
			FST::NODE()
		);

		FST::FST fst16(lexem, 2,		//
			FST::NODE(1, FST::RELATION('/', 1)),
			FST::NODE()
		);

		FST::FST fst17(lexem, 2,		//=
			FST::NODE(1, FST::RELATION('=', 1)),
			FST::NODE()
		);

		FST::FST fst18(lexem, 2,		//=
			FST::NODE(20, 
				FST::RELATION('0', 1),
				FST::RELATION('1', 1),
				FST::RELATION('2', 1),
				FST::RELATION('3', 1),
				FST::RELATION('4', 1),
				FST::RELATION('5', 1),
				FST::RELATION('6', 1),
				FST::RELATION('7', 1),
				FST::RELATION('8', 1),
				FST::RELATION('9', 1),
				FST::RELATION('0', 0),
				FST::RELATION('1', 0),
				FST::RELATION('2', 0),
				FST::RELATION('3', 0),
				FST::RELATION('4', 0),
				FST::RELATION('5', 0),
				FST::RELATION('6', 0),
				FST::RELATION('7', 0),
				FST::RELATION('8', 0),
				FST::RELATION('9', 0)),
			FST::NODE()
		);

		if (FST::execute(fst1) == -1) // integer
		{
			return 't';
		}
		else if (FST::execute(fst2) == -1) // string
		{
			return 't';
		}
		else if (FST::execute(fst3) == -1) // function 
		{
			return 'f';
		}
		else if (FST::execute(fst4) == -1) // declare
		{
			return 'd';
		}
		else if (FST::execute(fst5) == -1) // return
		{
			return 'r';
		}
		else if (FST::execute(fst6) == -1) // print
		{
			return 'p';
		}
		else if (FST::execute(fst7) == -1) return ';';
		else if (FST::execute(fst8) == -1) return ',';
		else if (FST::execute(fst9) == -1) return '{';
		else if (FST::execute(fst10) == -1) return '}';
		else if (FST::execute(fst11) == -1) { return '('; }
		else if (FST::execute(fst12) == -1) { return ')'; }
		else if (FST::execute(fst13) == -1) return '+';
		else if (FST::execute(fst14) == -1) return '-';
		else if (FST::execute(fst15) == -1) return '*';
		else if (FST::execute(fst16) == -1) return '/';
		else if (FST::execute(fst17) == -1) return '=';
		else if (FST::execute(fst18) == -1) return 'n'; // number
		else
		{
			return 'i';
		}
	}
}
