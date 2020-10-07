#include <iostream>
#include <Windows.h>

#include "Error.h"	// обработка ошибок
#include "Parm.h"	// обработка параметров
#include "Log.h"	// работа с протоколом
#include "In.h"		// ввод исходного кода
#include "IT.h"		// таблица идентификаторов
#include "LT.h"		// таблица лексем
#include "PolishNotation.h" // Польская запись

#define TBL_LENGTH 2048 // размеры создаваемых таблиц лексем и идентификаторов

// debug functions
void makeOutWithLT(LT::LexTable& table, IT::IdTable it);
void makeOutWithIT(IT::IdTable& idTable);

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Log::LOG_ log = Log::INITLOG; // log file

	try {
		Parm::PARM parms = Parm::getparm(argc, argv);	// получение параметров
		log = Log::getlog(parms.log);					// инициализация протокола	
		Log::WriteLog(log);								// запись начальных данных в лог файлы
		Log::WriteParm(log, parms);						// запись параметров
 
		LT::LexTable lexTable = LT::Create(TBL_LENGTH); // создаю таблицу лексем размером 2048
		IT::IdTable idTable = IT::Create(TBL_LENGTH); // создаю таблицу идентификаторов размеров 2048

		In::_IN_ in = In::getin(lexTable, idTable, parms.in, parms.out);	// получение in структуры

		makeOutWithLT(lexTable, idTable);
		makeOutWithIT(idTable);

		Log::WriteIn(log, in);
		Log::Close(log);
		std::cout << "Ready. Check " << parms.in << " DIR" << std::endl;
	}
	catch (Error::ERROR_ e) {
		std::cout << "\nОшибка " << e.id << ": " << e.message << "\n";
		if (e.inHandler.col >= 0) {
			std::cout << "строка: " << e.inHandler.line << std::endl;
			std::cout << "позиция: " << e.inHandler.col << std::endl;
			Log::WriteError(log, e);
			Log::Close(log);
		}
	}
	system("pause");
	return 0;
}

// debug
void makeOutWithLT(LT::LexTable& table, IT::IdTable it)
{
	LT::Entry* element = table.head;

	int i = 0;
	std::cout << "\tВЫВОД ТАБЛИЦЫ ЛЕКСЕМ:" << std::endl;
	while (element->next != nullptr)
	{
		std::cout << "\n" << i << '\t';
		while (i == element->sn) {
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
	std::cout << "_________________________________________________________________" << std::endl;
	std::cout << "                                   тип данных    1-перем  2-ф-ция" << std::endl;
	std::cout << "       номер         уник.id      1-INT  2-STR   3-парам  4-литер" << std::endl;
	std::cout << "_________________________________________________________________" << std::endl;
	std::cout << "|\t" << "IDX" << "\t|\t" << "ID" << "\t|\t" << "DATA" << "\t|\t" << "TYPE" << "\t|\t" << std::endl;
	while (showTable)
	{
		std::cout << "|\t" << showTable->idxTI << "\t";
		std::cout << "|\t" << showTable->id << "\t";
		std::cout << "|\t" << showTable->iddatatype << "\t";
		std::cout << "|\t" << showTable->idtype << "\t|" << std::endl;
		showTable = showTable->next;
	}
	std::cout << "_________________________________________________________________" << std::endl;
}