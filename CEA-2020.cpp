#include <iostream>
#include <Windows.h>

#include "Error.h"	// обработка ошибок
#include "Parm.h"	// обработка параметров
#include "Log.h"	// работа с протоколом
#include "In.h"		// ввод исходного кода
#include "IT.h"		// таблица идентификаторов
#include "LT.h"		// таблица лексем
#include "PolishNotation.h" // Польская запись
#include "MFST.h"	// автомат 

#define TBL_LENGTH 2048 // размеры создаваемых таблиц лексем и идентификаторов

void makeOutWithLT(LT::LexTable& table, IT::IdTable& it, bool showIDX, bool functionINFO);

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

		MFST_TRACE_START;								// оладка
		MFST::Mfst mfst(lexTable, GRB::getGreibach()); 	// автомат
		
		mfst.start();
		mfst.savededucation();
		mfst.printrules();

		system("pause");

		bool f = PN::PolishNotation(lexTable, idTable, true); // last arg is debug
		makeOutWithLT(lexTable, idTable, true, true);

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

void makeOutWithLT(LT::LexTable& table, IT::IdTable& it, bool showIDX, bool functionINFO)
{
	LT::Entry* element = table.head;

	int i = 0;
	std::cout << "\n\t\tВЫВОД ТАБЛИЦЫ ЛЕКСЕМ (ПОЛЬСКАЯ ЗАПИСЬ):" << std::endl;
	if (showIDX) std::cout << "<idx> - в треугольник скобках содержится id таблицы идентификаторов." << std::endl;
	if (functionINFO) std::cout << "@<idx>[ n1 n2 ... nn ] - вызов функции c id <idx>, которая имеет параметры [ p1 p2 ... pn ]\n\n";
	while (element->next != nullptr)
	{
		std::cout << std::setfill(' ') << std::setw(4) << std::right << i << ": ";
		int memory = element->sn;
		while (memory == element->sn && element->lexema[0] != NULL) {
			std::cout << element->lexema[0];
			if (showIDX && element->idxTI != IT_NULL_IDX)
				std::cout << '<' << element->idxTI << '>';
			if (functionINFO && element->lexema[0] == '@')
			{
				std::cout << "[ ";
				for (int i = 0; i < element->func.count; i++)
				{
					std::cout << element->func.idx[i] << ' ';
				}
				std::cout << ']';
			}

			element = element->next;
		}
		std::cout << '\n';
		i++;
	}
	std::cout << "\nTABLE SIZE: " << table.size << std::endl;
}