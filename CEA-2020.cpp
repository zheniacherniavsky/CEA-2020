#include <iostream>
#include <Windows.h>

#include "Error.h"	// обработка ошибок
#include "Parm.h"	// обработка параметров
#include "Log.h"	// работа с протоколом
#include "In.h"		// ввод исходного кода

#include "FST/FST.h"

void checkParms(Parm::PARM);

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Log::LOG_ log = Log::INITLOG;

	try {
		std::cout << "Loading..." << std::endl;
		Parm::PARM parms = Parm::getparm(argc, argv);

		checkParms(parms);

		std::cout << "---RESULT---" << std::endl;
		std::cout << "IN: " << parms.in << std::endl;
		std::cout << "OUT: " << parms.out << std::endl;
		std::cout << "LOG: " << parms.log << std::endl;
		std::cout << "------------" << std::endl;
		log = Log::getlog(parms.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parms);
		In::_IN_ in = In::getin(parms.in, parms.out);
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
	return 0;
}

// Debug functions

void checkParms(Parm::PARM p) {
	std::cout << p.in << std::endl;
	std::cout << p.log << std::endl;
	std::cout << p.out << std::endl;
}