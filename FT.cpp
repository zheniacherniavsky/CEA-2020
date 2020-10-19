#include "FT.h"

/*

	ISSUES:
				определение области видимости:
		Область видимости отслеживается, но нужно добавить стек, в который будет
	помещаться название функции в данный момент, и убираться соответственно когда
	функция будет закончена. Это нужно для дополнительной проверки на наличие переменной
	в таблице идентификаторов. 
		todo:
		 1- стек с функциями
		 2- добавить проверку на название функции в IT::IsID с доп аргументом.

	После этого я думаю всё будет работать хорошо. А, и не забыть добавить проверку
	на функцию входа main, у неё лексема m !!!!!


	++ ещё один баг:
		точка входа имеет область видимости на единицу больше, такого не должно быть
		Это исправится само, при добавлении стека функций

	++ добавить проверку на кол-во скобок в коде
*/

namespace FT
{

	struct flags
	{
		bool _declare		= false; // объявление переменной
		bool _literal		= false; // флаг на опознавание литерала
		bool _number		= false;
		bool _function		= false; // функция
		bool _integer		= false; // int
		bool _string		= false; // string
		bool _hesisIsOpen	= false; // открытие закрытие скобок для отличения параметров от функции

		void toFalse()	// занулить все флажки
		{
			_declare = false;
			_literal = false;
			_number = false;
			_function = false;
			_integer = false;
			_string = false;
			_hesisIsOpen = false;
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

			switch (lexemSymbol)
			{
			case(LEX_DECLARE):
				flag._declare = true;
				break;
			case(LEX_FUNCTION):
				flag._function = true;
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
				visibArea++;
				break;
			case(LEX_LEFTHESIS):
				flag._hesisIsOpen = true;
				if (flag._function) visibArea++;
				break;
			case(LEX_RIGHTHESIS):
				flag._hesisIsOpen = false;
				if (flag._function) visibArea--;
				flag._function = false;
				break;
			case(LEX_SEMICOLON):
				flag.toFalse();
				break;
			case(LEX_LEFTBRACE):
				visibArea++;
				flag.toFalse();
				break;
			case(LEX_BRACELET):
				visibArea--;
				flag.toFalse();
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

				int checkIdx = IT::IsId(it, lexemID, visibArea);
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
					if (flag._number) itElement.iddatatype = IT::INT;
					else itElement.iddatatype = IT::STR;
				}

				if (newElement)
				{
					for (int i = 0; i < ID_MAXSIZE; i++)
						itElement.id[i] = lexemID[i];

					itElement.visibility.area = visibArea;

					if (flag._function && !flag._literal)
					{
						if (flag._hesisIsOpen) itElement.idtype = IT::P; // параметр
						else itElement.idtype = IT::F; // функция
					}
					else if (!flag._literal) itElement.idtype = IT::V; // переменная
					else itElement.idtype = IT::L; // литерал

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

			std::cout << lexem << std::endl;

			lexem = strtok(NULL, " \n");
		}

		makeOutWithLT(lt, it);
		makeOutWithIT(it);
		std::cout << ErrorMessage << std::endl;
	}

	int* getLineNums(std::string code) // функция вычисления номера строки исходного кода для его лексем
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
	}

	void makeOutWithIT(IT::IdTable& idTable)
	{
		std::cout << "\n\t\t\tIT TABLE DEBUG" << std::endl;
		IT::Entry* showTable = idTable.head;
		std::cout << "___________________________________________________________________________________" << std::endl;
		std::cout << "                                   тип данных    1-перем  2-ф-ция      область     " << std::endl;
		std::cout << "       номер         уник.id      1-INT  2-STR   3-парам  4-литер     видимости    "<< std::endl;
		std::cout << "___________________________________________________________________________________" << std::endl;
		std::cout << "|\t" << "IDX" << "\t|\t" << "ID" << "\t|\t" << "DATA" << "\t|\t" << "TYPE" << "\t|\t" << "VIS" << "\t|\t" << std::endl;
		while (showTable)
		{
			std::cout << "|\t" << showTable->idxTI << "\t";
			std::cout << "|\t" << showTable->id << "\t";
			std::cout << "|\t" << showTable->iddatatype << "\t";
			std::cout << "|\t" << showTable->idtype << "\t";
			std::cout << "|\t" << showTable->visibility.area << "\t|" << std::endl;
			showTable = showTable->next;
		}
		std::cout << "_________________________________________________________________" << std::endl;
	}
}