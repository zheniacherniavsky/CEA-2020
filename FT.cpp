#include "FT.h"
#include "Error.h"
#include <stack>
#include <sstream>
#include <iomanip>

#define UNCHECKED_FUNCTION true

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
		bool _unchecked		= UNCHECKED_FUNCTION;

		void toFalse()	// занулить все флажки
		{
			_declare = false;
			_literal = false;
			_number = false;
			_integer = false;
			_string = false;
			_hesisIsOpen = false;

			// flag._function я контролирую сам в switch !!!!
		}
	};

	void fillTables(char* code, LT::LexTable &lt, IT::IdTable &it) // заполнение таблицы лексем и идентификаторов
	{
		int* posArray = getLineNums(code);			// массив позиций строк исходного кода
		char* lexem = strtok(&code[0], " \n");		// разбиваю код на лексемы
		int pos = 0;								// номер обрабатываемой лексемы
		int idx = 0;								// id идентификатора
		short visibArea = 0;						// область видимости
		flags flag;									// флажки
		short hesisCount = 0;						// слежу за правильным количеством скобок
		int literalCount = 1;						// отслеживание количества литералов

		std::string ErrorMessage = "";

		int linePos = 0;	// позиция лексемы в строке

		// добавление в таблицу лексем
		while (lexem != NULL)
		{
			char lexemID[ID_MAXSIZE]; // айди лексемы
			for (int i = 0; i < ID_MAXSIZE; i++)
				lexemID[i] = lexem[i];


			/*
				создаём элемент таблицы лексем
				создаём элемент таблицы идентификаторов

				элемент = символ лексемы;
				
				switch(элемент)
				{
					установка флажков
				}

				if(элемент идентификатор или литерал)
				{
					добавляем его в таблицу идентификаторов
				}

				добавление в таблицу лексем
			*/

			IT::Entry itElement; // создаю элемент таблицы идентификаторов
			itElement.idxTI = IT_NULL_IDX;

			LT::Entry ltElement; // элемент таблицы лексем
			ltElement.idxTI = IT_NULL_IDX;
			ltElement.priority = NULL;	// приоритеты выставляются в compareLexem

			char lexemSymbol = compareLexems(lexem, &ltElement); // получаю символ лексемы

			if (flag._functionName)
			{
				stkFunc.push(lexem);
				flag._functionName = false;
			}

			switch (lexemSymbol)
			{
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
				if(!flag._string) flag._number = true;
				flag._literal = true;
				break;
			case(LEX_MAIN): // точка входа main, соотв. область видимости повышаем на единицу
				if (!flag._main) flag._main = true;
				else throw ERROR_THROW(205); // two or more mains
				stkFunc.push(lexem);
				flag._function = true;
				break;
			case(LEX_LEFTHESIS):
				flag._hesisIsOpen = true;
				hesisCount++;
				if (flag._function && flag._functionParms) visibArea++;
				break;
			case(LEX_RIGHTHESIS):
				flag._hesisIsOpen = false;
				hesisCount--;
				if (flag._function && flag._functionParms)
				{
					visibArea--;
					flag._functionParms = false;
				}
				break;
			case(LEX_SEMICOLON):
				flag.toFalse();
				break;
			case(LEX_LEFTBRACE):
				visibArea++;
				hesisCount++;
				flag._body = true;
				flag.toFalse();
				break;
			case(LEX_BRACELET):
				visibArea--;
				hesisCount--;
				flag._body = false;
				flag.toFalse();
				if (flag._function)
				{
					if(!stkFunc.empty()) stkFunc.pop();
					flag._function = false; // отслеживаю закрытие блока функции
				}
				break;
			case('u'):	// unchecked, special for stack overflow exceptions
				flag._unchecked = true;
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
				int checkIdx = IT::IsId(it, lexemID, visibArea, stkFunc.top());
				bool newElement = false;
				if (checkIdx == IT_NULL_IDX) // следовательно это новый элемент
				{
					if (!flag._declare && !flag._literal && !flag._hesisIsOpen && !flag._function) // не литерал и не параметр и не функция
					{
						ErrorMessage += "\nВнимание, переменная "; ErrorMessage += lexem;
						ErrorMessage += " не объявлена!";
					}
					newElement = true;
					itElement.idxTI = idx;
					ltElement.idxTI = idx++;
					itElement.idxfirstLE = posArray[pos];
				}
				else ltElement.idxTI = checkIdx;

				if (flag._literal)
				{
					itElement.idtype = IT::L;
					if (flag._number) { // tut problems s return 0
						itElement.iddatatype = IT::INT;
						flag._number = false;
					}
					else itElement.iddatatype = IT::STR;
				}

				if (newElement)
				{
					if (flag._body && !flag._literal && !flag._declare) 
						throw ERROR_THROW(206);

					if (!flag._literal)
					{
						for (int i = 0; i < ID_MAXSIZE; i++)
							itElement.id[i] = lexemID[i];

						itElement.value.vint = NULL;
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
							if (number > 0 && number <= 256)
								itElement.value.vint = number;
							else if (flag._unchecked)
								itElement.value.vint = number % 256;
							else throw ERROR_THROW(207);
						}
						else if (itElement.iddatatype == IT::STR)
						{
							if (!((lexem[0] == '"' || lexem[0] == '\'') && (lexem[strlen(lexem) - 1] == '"' || lexem[strlen(lexem) - 1] == '\'')))
								throw ERROR_THROW(208);
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
						itElement.visibility.functionName[strlen(itElement.visibility.functionName)] = '\0';
					}
					else itElement.visibility.functionName[0] = '\0';
					itElement.visibility.area = visibArea;
					
					itElement.idtype = _idtype; // параметр

					if (flag._integer) itElement.iddatatype = IT::INT;
					else if (flag._string) itElement.iddatatype = IT::STR;

					IT::Add(it, itElement);
					flag._declare = false;
				}
			}

			// заполнение таблицы лексем
			ltElement.sn = posArray[pos++];

			ltElement.lexema[0] = lexemSymbol;
			LT::Add(lt, ltElement);
			lexem = strtok(NULL, " \n");
		}

		if (hesisCount != 0) throw ERROR_THROW(203);
		if (!flag._main) throw ERROR_THROW(204);

		makeOutWithLT(lt, it);
		makeOutWithIT(it);
		std::cout << ErrorMessage << std::endl;
	}

	int* getLineNums(std::string code) // функция вычисления номера строки исходного кода для его лексем
	{
		int* array = new int[MAX_LEXEMS_LENGTH];
		char* p = &code[0];
		bool endLine;
		bool emptyLine;
		short _count = 0;
		for (int line = 0, pos = 0, count = 0; *p != '\0'; *p++)
		{
			endLine = false;
			emptyLine = true;
			count = 0;
			while (*p != '\n')
			{
				if (*p == ' ') while (*p == ' ') *p++; // пропускаем пробелы
				while (*p != ' ') {
					if (*p != '\n') {
						*p++; // проходим лексему
						emptyLine = false;
					}
					else
					{
						endLine = true;
						break;
					}
				}
				if (!endLine)
				{
					count++;
					array[pos] = line;
					pos++;
				} if (endLine && !count)
				{
					array[pos] = line;
					pos++;
				}
			}
			if (!emptyLine) line++;
			_count += count;
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

		FST::FST fst6(lexem, 6,		//print
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
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

		FST::FST fst20(lexem, 10,		// enter point
			FST::NODE(1, FST::RELATION('u', 1)),
			FST::NODE(1, FST::RELATION('n', 2)),
			FST::NODE(1, FST::RELATION('c', 3)),
			FST::NODE(1, FST::RELATION('h', 4)),
			FST::NODE(1, FST::RELATION('e', 5)),
			FST::NODE(1, FST::RELATION('c', 6)),
			FST::NODE(1, FST::RELATION('k', 7)),
			FST::NODE(1, FST::RELATION('e', 8)),
			FST::NODE(1, FST::RELATION('d', 9)),
			FST::NODE()
		);

		if (FST::execute(fst1) == -1) return LEX_INTEGER; // integer
		else if (FST::execute(fst2) == -1) return LEX_STRING; // string
		else if (FST::execute(fst3) == -1) return LEX_FUNCTION;
		else if (FST::execute(fst4) == -1) return LEX_DECLARE;
		else if (FST::execute(fst5) == -1) return LEX_RETURN;
		else if (FST::execute(fst6) == -1) return LEX_PRINT;
		else if (FST::execute(fst7) == -1) return LEX_SEMICOLON;
		else if (FST::execute(fst8) == -1) return LEX_COMMA;
		else if (FST::execute(fst9) == -1) return LEX_LEFTBRACE;
		else if (FST::execute(fst10) == -1) return LEX_BRACELET;
		else if (FST::execute(fst11) == -1)
		{
			ltElement->priority = 0;  
			return LEX_LEFTHESIS;
		}
		else if (FST::execute(fst12) == -1)
		{
			ltElement->priority = 0;
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
			ltElement->priority = 2;
			return LEX_DIRSLASH;
		}
		else if (FST::execute(fst17) == -1) return LEX_IS;
		else if (FST::execute(fst18) == -1) return LEX_NUMBER; // number
		else if (FST::execute(fst19) == -1) return LEX_MAIN; // main
		else if (FST::execute(fst20) == -1) return 'u'; // unchecked!
		else return LEX_ID;
	}

	// debug
	void makeOutWithLT(LT::LexTable& table, IT::IdTable &it)
	{
		LT::Entry* element = table.head;

		int i = 0;
		std::cout << "\tВЫВОД ТАБЛИЦЫ ЛЕКСЕМ:" << std::endl;
		while (element->next != nullptr)
		{
			std::cout << "\n" << i << '\t';
			while (i == element->sn && element->lexema[0] != NULL) {
				std::cout << element->lexema[0];
				if (element->idxTI != IT_NULL_IDX)
					std::cout << '<' << element->idxTI << '>';
				element = element->next;
			}
			i++;
		}
		std::cout << "\n" <<table.size << std::endl;
	}

	void makeOutWithIT(IT::IdTable& idTable)
	{
		std::cout << "\n\t\t\tIT TABLE DEBUG" << std::endl;
		IT::Entry* showTable = idTable.head;
		std::cout << std::setfill('_') << std::setw(91) << '_' << std::endl;
		std::cout << std::setfill(' ')
			<< std::setw(8) << "номер"
			<< std::setw(6) << "id"
			<< std::setw(6) << "тип"
			<< std::setw(10) << "DATA"
			<< std::setw(20) << "обл. видимости"
			<< std::setw(12) << "функция"
			<< std::setw(20) << "содержимое"
			<< std::setw(10) << '|' << std::endl;
		std::cout << std::setfill('_') << std::setw(91) << '_' << std::endl;
		while (showTable)
		{
			std::cout << std::setfill(' ')
				<< std::setw(6) << showTable->idxTI
				<< std::setw(8) << showTable->id;
			if (showTable->iddatatype == IT::INT)
				std::cout << std::setw(6) << "INT";
			else if (showTable->iddatatype == IT::STR)
				std::cout << std::setw(6) << "STR";
			if(showTable->idtype == IT::F)
				std::cout << std::setw(12) << "функция";
			else if (showTable->idtype == IT::P)
				std::cout << std::setw(12) << "параметр";
			else if (showTable->idtype == IT::L)
				std::cout << std::setw(12) << "литерал";
			else if (showTable->idtype == IT::V)
				std::cout << std::setw(12) << "переменная";
			std::cout << std::setw(10) << showTable->visibility.area;
			std::cout << std::setw(19) << showTable->visibility.functionName;

			if (showTable->iddatatype == IT::INT)
				std::cout << std::setw(17) << showTable->value.vint;
			else if (showTable->iddatatype == IT::STR)
				if(showTable->value.vstr->len == NULL) std::cout << std::setw(19) << "NULL";
				else
				{
					std::cout << std::setw(10);
					for (int i = 0; i < showTable->value.vstr->len; i++)
					{
						std::cout << showTable->value.vstr->str[i];
						if (i == 8)
						{
							std::cout << "...";
							break;
						}
					}
					std::cout << "[len: " << showTable->value.vstr->len << ']';
				}
			std::cout << std::endl;
			showTable = showTable->next;
		}
		std::cout << std::setfill('_') << std::setw(91) << '_' << std::endl;
	}
}