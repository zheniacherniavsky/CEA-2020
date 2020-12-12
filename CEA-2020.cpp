#include <iostream>
#include <Windows.h>


//			                               https://github.com/zheniacherniavsky
//░░░░░░░░░░▄					           programming language - course project
//░░░░░░░░▄▐░▄▄█████▄▄			                  
//░░░░░░▄█████████████▄▀▄▄░▄▄▄
//░░░░████▐█████▌████████▌█████▌		    - - - - - - - how it work - - - - - - -
//░░░████▌█████▌█░████████▐▀██▀				1 stage: get in.txt file with code
//░▄█████░█████▌░█░▀██████▌█▄▄▀▄			2 stage: lex analyz (In.cpp)
//░▌███▌█░▐███▌▌░░▄▄░▌█▌███▐███░▀			3 stage: fill lex and id tables (FT.cpp)
//▐░▐██░░▄▄▐▀█░░░▐▄█▀▌█▐███▐█				4 stage: syntax analyz (MFST.cpp)
//░░███░▌▄█▌░░▀░░▀██░░▀██████▌				5 stage: semantic analyz (SemAnalyzer.cpp)
//░░░▀█▌▀██▀░▄░░░░░░░░░███▐███				6 stage: generating assembler code (CodeGeneration.cpp)
//░░░░██▌░░░░░░░░░░░░░▐███████▌				-	-	-	-	-	-	-	-	-	-	-	-	-	-
//░░░░███░░░░░▀█▀░░░░░▐██▐███▀▌
//░░░░▌█▌█▄░░░░░░░░░▄▄████▀░▀
//░░░░░░█▀██▄▄▄░▄▄▀▀▒█▀█░▀
//░░░░░░░░░▀░▀█▀▌▒▒▒░▐▄▄
//░░░░░░░░▄▄▀▀▄░░░░░░▄▀░▀▀▄▄
//░░░░░░▄▀░▄▀▄░▌░░░▄▀░░░░░░▄▀▀▄
//░░░░░▐▒▄▀▄▀░▌▀▄▄▀░░░░░░▄▀▒▒▒▐				      https://vk.com/geshkaaaaaa
//░░░░▐▒▒▌▀▄░░░░░▌░░░░▄▄▀▒▐▒▒▒▒▌
//░░░▌▄▀░░░▄▀░█▀▒▒▒▒▀▄▒▌▐▒▒▒▒▒▌▌
//░░▄▀▒▐░▄▀░░░▌▒▐▒▐▒▒▒▌▀▒▒▒▒▒▐▒▌

#include "Error.h"	// обработка ошибок
#include "Parm.h"	// обработка параметров
#include "Log.h"	// работа с протоколом
#include "In.h"		// ввод исходного кода
#include "IT.h"		// таблица идентификаторов
#include "LT.h"		// таблица лексем
#include "PolishNotation.h" // Польская запись
#include "MFST.h"	// автомат 
#include "SemAnalyzer.h"
#include "CodeGeneration.h"

#define TBL_LENGTH 2048 // размеры создаваемых таблиц лексем и идентификаторов

void makeOutWithLT(LT::LexTable& table, IT::IdTable& it, bool showIDX, bool functionINFO, Log::LOG_ log);
void makeOutWithIT(IT::IdTable& idTable, Log::LOG_ log);

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
		if(parms.lex) makeOutWithLT(lexTable, idTable, true, true, log);
		if(parms.id) makeOutWithIT(idTable, log);

		*log.stream << "\n\n\tСинтаксический анализатор\n";
		MFST_TRACE_START;								// оладка
		MFST::Mfst mfst(lexTable, GRB::getGreibach()); 	// автомат
		
		if (mfst.start(log)) // синтаксическийй анализ
		{
			mfst.savededucation();
			if(parms.tree) mfst.printrules(log);

			PN::PolishNotation(lexTable, idTable, true); // last arg is debug
			*log.stream << "\n\tПольская запись:\n";
			makeOutWithLT(lexTable, idTable, false, false, log);

			if (SemAnalyzer::semAnalyzer(lexTable, idTable, log)) // семантический анализ
			{
				Log::WriteIn(log, in);
				Log::Close(log);
				if (CG::CodeGeneration(idTable, lexTable, parms.out))
				{
					std::cout << "\tCEA-2020: Компиляция завершена."
						<< "\nСгенерированный ассемблерный код: " << parms.out
						<< "\nПодробная информация находится в протоколе: " << parms.log << "\n\n";
					system("pause");
					return 0;
				}
			}
		}
	}
	catch (Error::ERROR_ e) {
		std::cout << "\nОшибка " << e.id << ": " << e.message << "\n";
		if (e.inHandler.line >= 0) {
			if(e.inHandler.col > 0) std::cout << "позиция: " << e.inHandler.col << std::endl;
			std::cout << "строка: " << e.inHandler.line  << std::endl;
			Log::WriteError(log, e);
			Log::Close(log);
		}
		system("pause");
		return -1;
	}
}

void makeOutWithLT(LT::LexTable& table, IT::IdTable& it, bool showIDX, bool functionINFO, Log::LOG_ log)
{
	LT::Entry* element = table.head;

	int i = 0;
	*log.stream << "\n\t\tВЫВОД ТАБЛИЦЫ ЛЕКСЕМ:" << std::endl;
	if (showIDX) *log.stream << "<idx> - в треугольник скобках содержится id таблицы идентификаторов." << std::endl;
	if (functionINFO) *log.stream << "@<idx>[ n1 n2 ... nn ] - вызов функции c id <idx>, которая имеет параметры [ p1 p2 ... pn ]\n\n";
	while (element->next != nullptr)
	{
		*log.stream << std::setfill(' ') << std::setw(4) << std::right << i << ": ";
		int memory = element->sn;
		while (memory == element->sn && element->lexema[0] != NULL) {
			*log.stream << element->lexema[0];
			if (showIDX && element->idxTI != IT_NULL_IDX)
				*log.stream << '<' << element->idxTI << '>';
			if (functionINFO && element->lexema[0] == '@')
			{
				*log.stream << "[ ";
				for (int i = 0; i < element->func.count; i++)
				{
					*log.stream << element->func.idx[i] << ' ';
				}
				*log.stream << ']';
			}

			element = element->next;
		}
		*log.stream << '\n';
		i++;
	}
	*log.stream << "\nTABLE SIZE: " << table.size << std::endl;
}

void makeOutWithIT(IT::IdTable& idTable, Log::LOG_ log)
{
	*log.stream << "\n\t\t\tIT TABLE DEBUG" << std::endl;
	IT::Entry* showTable = idTable.head;
	*log.stream << std::setfill('_') << std::setw(101) << '_' << std::endl;
	*log.stream << std::setfill(' ')
		<< std::setw(8) << "номер"
		<< std::setw(6) << "id"
		<< std::setw(6) << "тип"
		<< std::setw(10) << "DATA"
		<< std::setw(20) << "обл. видимости"
		<< std::setw(12) << "функция"
		<< std::setw(10) << "declared"
		<< std::setw(20) << "содержимое"
		<< std::setw(10) << '|' << std::endl;
	*log.stream << std::setfill('_') << std::setw(101) << '_' << std::endl;
	while (showTable)
	{
		*log.stream << std::setfill(' ')
			<< std::setw(6) << showTable->idxTI
			<< std::setw(8) << showTable->id;
		if (showTable->iddatatype == IT::INT)
			*log.stream << std::setw(6) << "INT";
		else if (showTable->iddatatype == IT::STR)
			*log.stream << std::setw(6) << "STR";
		if (showTable->idtype == IT::F)
			*log.stream << std::setw(12) << "функция";
		else if (showTable->idtype == IT::P)
			*log.stream << std::setw(12) << "параметр";
		else if (showTable->idtype == IT::L)
			*log.stream << std::setw(12) << "литерал";
		else if (showTable->idtype == IT::V)
			*log.stream << std::setw(12) << "переменная";
		*log.stream << std::setw(10) << showTable->visibility.area;
		*log.stream << std::setw(19) << showTable->visibility.functionName;

		if (showTable->declared == true) *log.stream << std::setw(10) << "YES";
		else *log.stream << std::setw(10) << "NO";

		if (showTable->iddatatype == IT::INT)
			*log.stream << std::setw(17) << showTable->value.vint;
		else if (showTable->iddatatype == IT::STR)
			if (showTable->value.vstr->len == NULL) *log.stream << std::setw(19) << "NULL";
			else
			{
				*log.stream << std::setw(10);
				for (int i = 0; i < showTable->value.vstr->len; i++)
				{
					*log.stream << showTable->value.vstr->str[i];
					if (i == 8)
					{
						*log.stream << "...";
						break;
					}
				}
				*log.stream << "[len: " << showTable->value.vstr->len << ']';
			}


		*log.stream << std::setfill(' ') << std::endl;
		showTable = showTable->next;
	}
	*log.stream << std::setfill('_') << std::setw(101) << '_' << std::endl;
}