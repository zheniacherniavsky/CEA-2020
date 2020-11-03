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

void makeOutWithLT(LT::LexTable& table, IT::IdTable& it);

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

		bool f = PN::PolishNotation(-1, lexTable, idTable, true); // last arg is debug
		makeOutWithLT(lexTable, idTable);

		Log::WriteIn(log, in);
		Log::Close(log);
		std::cout << "\nReady. Check " << parms.in << " DIR" << std::endl;
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

void makeOutWithLT(LT::LexTable& table, IT::IdTable& it)
{
	LT::Entry* element = table.head;

	int i = 0;
	std::cout << "\tВЫВОД ТАБЛИЦЫ ЛЕКСЕМ:" << std::endl;
	while (element && element->next != nullptr)
	{
		std::cout << "\n" << i << '\t';
		while (element && i == element->sn) {
			std::cout << element->lexema[0];
			//if (element->idxTI != IT_NULL_IDX)
				//std::cout << '<' << element->idxTI << '>';
			element = element->next;
		}
		i++;
	}
}