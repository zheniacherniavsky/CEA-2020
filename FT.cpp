﻿#include "FT.h"
#include "Error.h"
#include <stack>
#include <sstream>
#include <iomanip>

namespace FT
{

	std::stack <char*> stkFunc;

	struct flags
	{
		bool _declare		= false; // объявление переменной
		bool _literal		= false; // флаг на опознавание литерала
		bool _number		= false;
		bool _function		= false; // функция
		bool _functionName	= false; // запоминание названия функции
		bool _body			= false; // тело
		bool _main			= false;
		bool _integer		= false; // int
		bool _string		= false; // string
		bool _hesisIsOpen	= false; // открытие закрытие скобок для отличения параметров от функции
		bool _functionParms = false;
		bool _getFuncIdx	= false;
		bool _return		= false;
		bool _repeat		= false;
		bool _condition		= false;

		void toFalse()	// занулить все флажки
		{
			_declare = false;
			_literal = false;
			_number = false;
			_integer = false;
			_string = false;
			_hesisIsOpen = false;
			_return = false;
			// flag._function я контролирую сам в switch !!!!
		}
	};

	void fillTables(char* code, LT::LexTable &lt, IT::IdTable &it) // заполнение таблицы лексем и идентификаторов
	{
		int* posArray = getLineNums(code);			// массив позиций строк обработанного кода

		char lexArr[512][256];
		int pos = 0;
		int spos = 0;
		while (*code) // разбитие кода на отдельные компоненты
		{
			while (*code != ' ' && *code != '\n')
			{
				lexArr[pos][spos++] = *code;
				*code++;
			}
			lexArr[pos][spos] = 0x00;
			pos++; spos = 0;
			while (*code == ' ' || *code == '\n')
				*code++;

		}
		lexArr[pos][0] = 0x00;

		int lexPos = 0;								// номер лексемы
		char *lexem = lexArr[lexPos++];

		pos = 0;								// номер обрабатываемой лексемы
		int idx = 0;								// id идентификатора
		short visibArea = 0;						// область видимости
		flags flag;									// флажки
		int literalCount = 1;						// отслеживание количества литералов
		int functionIdxMemory = -1;
		short firstRepeatOrCondition = 0;		// Кто первый, цикл или условие (цикл == 1, условие == 2)

		int linePos = 0;	// позиция лексемы в строке

		// добавление в таблицу лексем
		while (lexem[0] != 0x00)
		{
			// if (posArray[pos] != posArray[pos - 1] && !flag._repeat) flag.toFalse();
			
			char lexemID[ID_MAXSIZE]; // айди лексемы
			for (int i = 0; i < ID_MAXSIZE; i++)
				lexemID[i] = lexem[i];
			lexemID[ID_MAXSIZE - 1] = 0x00;

			IT::Entry itElement; // создаю элемент таблицы идентификаторов
			itElement.idxTI = IT_NULL_IDX;

			LT::Entry ltElement; // элемент таблицы лексем
			ltElement.idxTI = IT_NULL_IDX;
			ltElement.priority = NULL;	// приоритеты выставляются в compareLexem

			char lexemSymbol = compareLexems(lexem, &ltElement); // получаю символ лексемы

			if (flag._functionName)
			{
				lexem[5] = 0x00;
				stkFunc.push(lexem);
				flag._functionName = false;
				flag._getFuncIdx = true;
			}

			switch (lexemSymbol)
			{
			case(LEX_CONDITION):
				if (flag._condition) throw Error::geterrorin(203, posArray[pos] + 1, 0);
				else flag._condition = true;
				if (firstRepeatOrCondition == 0) firstRepeatOrCondition = 2;
				break;
			case(LEX_CYCLE): // repeat
				if (flag._repeat) throw Error::geterrorin(204, posArray[pos] + 1, 0);
				else flag._repeat = true;
				if (firstRepeatOrCondition == 0) firstRepeatOrCondition = 1;
				break;
			case(LEX_DECLARE):
				flag._declare = true;
				break;
			case(LEX_FUNCTION):
				if (!flag._function)
				{
					flag._function = true;
					flag._functionParms = true;
					flag._functionName = true;
				}
				break;
				
			case(LEX_INTEGER): // or LEX_STRING doesnt matter, they are equals
				if (lexem[0] == 's')
				{
					flag._string = true;
					break;
				}
				else
				{
					flag._integer = true;
					break;
				}
			case(LEX_NUMBER):
				lexemSymbol = LEX_ID;
				flag._number = true;
				flag._literal = true;
				break;
			case(LEX_MAIN): // точка входа main, соотв. область видимости повышаем на единицу
				stkFunc.push(lexem);
				flag._function = true;
				break;
			case(LEX_LEFTHESIS):
				flag._hesisIsOpen = true;
				if (flag._function && flag._functionParms) visibArea++;
				break;
			case(LEX_RIGHTHESIS):
				flag._hesisIsOpen = false;
				if (flag._function && flag._functionParms)
				{
					visibArea--;
					flag._functionParms = false;
					functionIdxMemory = -1;
				}
				break;
			case(LEX_SEMICOLON):
				flag.toFalse();
				break;
			case(LEX_LEFTBRACE):
				if (!flag._repeat)
				{
					visibArea++;
					flag._body = true;
					flag.toFalse();
				}
				break;
			case(LEX_BRACELET):
				if (flag._repeat && !flag._condition)
				{
					flag._repeat = false;
					firstRepeatOrCondition = 0;
				}
				else if (!flag._repeat && flag._condition)
				{
					flag._condition = false;
					firstRepeatOrCondition = 0;
				}
				else if (flag._repeat && flag._condition)
				{
					if (firstRepeatOrCondition == 1) flag._condition = false;
					else if (firstRepeatOrCondition == 2) flag._repeat = false;
				}
				else
				{
					visibArea--;
					flag._body = false;
					flag.toFalse();
					if (flag._function)
					{
						if (!stkFunc.empty()) stkFunc.pop();
						flag._function = false; // отслеживаю закрытие блока функции
					}
				}
				break;
			case(LEX_RETURN):
				flag._return = true;
				break;
			case(LEX_COMMA):
				if (flag._functionParms)
				{
					flag._integer = false;
					flag._string = false;
				}
				break;
			}

			if (lexemSymbol == LEX_ID)
			{
				if (lexem[0] == '\'' || lexem[0] == '\"') // строковый и числовой литерал
				{
					flag._literal = true;
					lexemSymbol = LEX_LITERAL;
				}
				else if (flag._number) lexemSymbol = LEX_LITERAL;

				// определение типа
				IT::IDTYPE _idtype;
				if (flag._function && !flag._literal)
				{
					if (flag._hesisIsOpen) _idtype = IT::P; // параметр
					else if (!flag._body) _idtype = IT::F; // функция
					else _idtype = IT::V;
				}
				else if (!flag._literal) _idtype = IT::V; // переменная
				else _idtype = IT::L; // литерал

				// проверка на наличие в табллице
				int checkIdx = NULL;

				if (!stkFunc.empty())
				{
					checkIdx = IT::IsId(it, lexemID, visibArea, stkFunc.top());
				}
				else throw ERROR_THROW(208);

				bool newElement = false;
				if (checkIdx == IT_NULL_IDX) // следовательно это новый элемент
				{
					newElement = true;
					if (_idtype == IT::P ||
						(_idtype == IT::F && !flag._body))
					{
						itElement.declared = true;
					}
					else itElement.declared = false;
					itElement.idxTI = idx;
					ltElement.idxTI = idx++;
					itElement.idxfirstLE = posArray[pos];
				}
				else ltElement.idxTI = checkIdx;

				if (flag._literal)
				{
					itElement.idtype = IT::L;
					if (flag._number) {
						itElement.iddatatype = IT::INT;
						flag._number = false;
					}
					else itElement.iddatatype = IT::STR;
				}

				if (newElement)
				{
					if (!flag._literal)
					{
						for (int i = 0; i < ID_MAXSIZE; i++)
							itElement.id[i] = lexemID[i];

						itElement.value.vint8 = NULL;
						itElement.value.vstr->len = NULL;
						itElement.value.vstr->str[0] = NULL;
					}
					else // литерал дожен получить уникальный ID и присвоить себе значение, будь то число или строка.
					{
						itElement.id[0] = 'c';
						std::stringstream a;
						a << literalCount;
						std::string sliteralCount;
						a >> sliteralCount;
						for (int i = 1;; i++)
						{
							if (sliteralCount[i - 1] != NULL) itElement.id[i] = sliteralCount[i - 1];
							else
							{
								itElement.id[i] = NULL;
								break;
							}

						}
						if (itElement.iddatatype == IT::INT)
						{
							std::stringstream b;
							b << lexem;
							int number;
							b >> number;
							itElement.value.vint8 = number;
						}
						else if (itElement.iddatatype == IT::STR)
						{
							itElement.value.vstr->len = strlen(lexem) - 2;
							for (int i = 1; i < strlen(lexem) - 1; i++)
								itElement.value.vstr->str[i - 1] = lexem[i];
							itElement.value.vstr->str[strlen(lexem) - 2] = NULL;
						}
						literalCount++;
					}

					if (!stkFunc.empty())
					{
						for (int i = 0; i < ID_MAXSIZE; i++)
							itElement.visibility.functionName[i] = stkFunc.top()[i];
						itElement.visibility.functionName[5] = 0x00;
					}
					else itElement.visibility.functionName[0] = '\0';
					itElement.visibility.area = visibArea;
					
					itElement.idtype = _idtype; // параметр

					if (!flag._literal) 
					{
						if (flag._integer) itElement.iddatatype = IT::INT;
						else if (flag._string) itElement.iddatatype = IT::STR;
					}


					IT::Add(it, itElement);
					flag._declare = false;
				}
			}

			// заполнение таблицы лексем
			ltElement.sn = posArray[pos++] + 1;

			if (flag._functionParms && lexemSymbol == LEX_ID && functionIdxMemory != -1)
			{
				LT::Entry* ptrFunc = LT::GetEntryByIdx(lt, functionIdxMemory);
				ptrFunc->func.memoryType[ptrFunc->func.memoryCount] = itElement.iddatatype;
				ptrFunc->func.memoryCount++;
			}

			if (flag._getFuncIdx)
			{
				functionIdxMemory = ltElement.idxTI;
				flag._getFuncIdx = false;
			}

			ltElement.lexema[0] = lexemSymbol;
			LT::Add(lt, ltElement);
			lexem = lexArr[lexPos++];
		}
	}

	int* getLineNums(std::string code) // функция вычисления номера строки исходного кода для его лексем
	{
		int* array = new int[MAX_LEXEMS_LENGTH];
		char* p = &code[0];
		for (int line = 0, pos = 0; *p != '\0'; *p++)
		{
			if (*p == '\n')
			{
				line++;
				continue;
			}
			if (*p != ' ')
			{
				array[pos++] = line;
				while (*p != ' ')
				{
					*p++;
					if (*p == '\n') break;
				}
			}
			if (*p == '\n') line++;
		}
		return array;
	}

	char compareLexems(char* lexem, LT::Entry* ltElement) {

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

		FST::FST fst6(lexem, 5,		//print
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE(1, FST::RELATION('u', 3)),
			FST::NODE(1, FST::RELATION('t', 4)),
			FST::NODE()
		);

		FST::FST fst7(lexem, 2,	FST::NODE(1, FST::RELATION(';', 1)), FST::NODE());
		FST::FST fst8(lexem, 2,	FST::NODE(1, FST::RELATION(',', 1)), FST::NODE());
		FST::FST fst9(lexem, 2, FST::NODE(1, FST::RELATION('{', 1)), FST::NODE());
		FST::FST fst10(lexem, 2, FST::NODE(1, FST::RELATION('}', 1)), FST::NODE());
		FST::FST fst11(lexem, 2, FST::NODE(1, FST::RELATION('(', 1)), FST::NODE());
		FST::FST fst12(lexem, 2, FST::NODE(1, FST::RELATION(')', 1)), FST::NODE());
		FST::FST fst13(lexem, 2, FST::NODE(1, FST::RELATION('+', 1)), FST::NODE());
		FST::FST fst14(lexem, 2, FST::NODE(1, FST::RELATION('-', 1)), FST::NODE());
		FST::FST fst15(lexem, 2, FST::NODE(1, FST::RELATION('*', 1)), FST::NODE());
		FST::FST fst16(lexem, 2, FST::NODE(1, FST::RELATION('/', 1)), FST::NODE());
		FST::FST fst17(lexem, 2, FST::NODE(1, FST::RELATION('=', 1)), FST::NODE());

		FST::FST fst18(lexem, 2,
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

		FST::FST fst19(lexem, 5,		// enter point
			FST::NODE(1, FST::RELATION('m', 1)),
			FST::NODE(1, FST::RELATION('a', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE()
		);

		FST::FST fst20(lexem, 5,		// enter point
			FST::NODE(1, FST::RELATION('s', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE(1, FST::RELATION('u', 3)),
			FST::NODE(1, FST::RELATION('t', 4)),
			FST::NODE()
		);

		FST::FST fst21(lexem, 7,		// repeat cycle
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('p', 3)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE(1, FST::RELATION('a', 5)),
			FST::NODE(1, FST::RELATION('t', 6)),
			FST::NODE()
		);

		FST::FST fst22(lexem, 5,		// convert into to string
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('t', 2)),
			FST::NODE(1, FST::RELATION('o', 3)),
			FST::NODE(1, FST::RELATION('s', 4)),
			FST::NODE()
		);

		FST::FST fst23(lexem, 4,		// convert into to string
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE(1, FST::RELATION('w', 3)),
			FST::NODE()
		);

		FST::FST fst24(lexem, 5,		// convert into to string
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE(1, FST::RELATION('o', 3)),
			FST::NODE(1, FST::RELATION('t', 4)),
			FST::NODE()
		);

		FST::FST fst25(lexem, 2, FST::NODE(1, FST::RELATION('%', 1)), FST::NODE());
		FST::FST fst26(lexem, 2, FST::NODE(1, FST::RELATION('&', 1)), FST::NODE());
		FST::FST fst27(lexem, 2, FST::NODE(1, FST::RELATION('|', 1)), FST::NODE());
		FST::FST fst28(lexem, 2, FST::NODE(1, FST::RELATION('~', 1)), FST::NODE());

		FST::FST fst29(lexem, 5,		// if condition (a less b)
			FST::NODE(1, FST::RELATION('l', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('s', 3)),
			FST::NODE(1, FST::RELATION('s', 4)),
			FST::NODE()
		);

		FST::FST fst30(lexem, 5,		// if condition (a over b)
			FST::NODE(1, FST::RELATION('o', 1)),
			FST::NODE(1, FST::RELATION('v', 2)),
			FST::NODE(1, FST::RELATION('e', 3)),
			FST::NODE(1, FST::RELATION('r', 4)),
			FST::NODE()
		);

		FST::FST fst31(lexem, 6,		// if condition (a equal b)
			FST::NODE(1, FST::RELATION('e', 1)),
			FST::NODE(1, FST::RELATION('q', 2)),
			FST::NODE(1, FST::RELATION('u', 3)),
			FST::NODE(1, FST::RELATION('a', 4)),
			FST::NODE(1, FST::RELATION('l', 5)),
			FST::NODE()
		);

		FST::FST fst32(lexem, 3,		// if condition
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('f', 2)),
			FST::NODE()
		);

		if (FST::execute(fst1) == -1) return LEX_INTEGER; // integer
		else if (FST::execute(fst2) == -1) return LEX_STRING; // string
		else if (FST::execute(fst3) == -1) return LEX_FUNCTION;
		else if (FST::execute(fst4) == -1) return LEX_DECLARE;
		else if (FST::execute(fst5) == -1) return LEX_RETURN;
		else if (FST::execute(fst6) == -1) return LEX_PRINT_INT;
		else if (FST::execute(fst7) == -1) return LEX_SEMICOLON;
		else if (FST::execute(fst8) == -1) return LEX_COMMA;
		else if (FST::execute(fst9) == -1) return LEX_LEFTBRACE;
		else if (FST::execute(fst10) == -1) return LEX_BRACELET;
		else if (FST::execute(fst11) == -1)
		{
			ltElement->priority = 1;  
			return LEX_LEFTHESIS;
		}
		else if (FST::execute(fst12) == -1)
		{
			ltElement->priority = 1;
			return LEX_RIGHTHESIS;
		}
		else if (FST::execute(fst13) == -1)
		{
			ltElement->priority = 2;
			return LEX_PLUS;
		}
		else if (FST::execute(fst14) == -1)
		{
			ltElement->priority = 2;
			return LEX_MINUS;
		}
		else if (FST::execute(fst15) == -1)
		{
			ltElement->priority = 3;
			return LEX_STAR;
		}
		else if (FST::execute(fst16) == -1)
		{
			ltElement->priority = 3;
			return LEX_DIRSLASH;
		}
		else if (FST::execute(fst17) == -1) return LEX_IS;
		else if (FST::execute(fst18) == -1) return LEX_NUMBER; // number
		else if (FST::execute(fst19) == -1) return LEX_MAIN; // main
		else if (FST::execute(fst20) == -1) return LEX_PRINT_STR; // main
		else if (FST::execute(fst21) == -1) return LEX_CYCLE; // repeat cycle
		else if (FST::execute(fst22) == -1) return LEX_CONVERT; // convert int to string
		else if (FST::execute(fst23) == -1) return LEX_POW; // pow function
		else if (FST::execute(fst24) == -1) return LEX_ROOT; // root function
		else if (FST::execute(fst25) == -1)
		{
			ltElement->priority = 3;
			return LEX_PERCENT;
		}
		else if (FST::execute(fst26) == -1)
		{
			ltElement->priority = 3;
			return LEX_AND;
		}
		else if (FST::execute(fst27) == -1)
		{
			ltElement->priority = 3;
			return LEX_OR;
		}
		else if (FST::execute(fst28) == -1)
		{
			ltElement->priority = 3;
			return LEX_INVERT;
		}
		else if (FST::execute(fst29) == -1) return LEX_LESS; // less
		else if (FST::execute(fst30) == -1) return LEX_OVER; // over
		else if (FST::execute(fst31) == -1) return LEX_EQUAL; // equal
		else if (FST::execute(fst32) == -1) return LEX_CONDITION; // if condition
		else return LEX_ID;
	}
}