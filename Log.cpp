#include "Log.h"
#include "Parm.h"
#include "Error.h"
#include <ctime>
#include <fstream>
#include <iostream>

namespace Log {
	LOG_ getlog(char* logfile) {
		LOG_ log;
		for (int i = 0; i <= strlen(logfile); i++)
			log.logfile[i] = logfile[i];
		log.stream = new std::ofstream(logfile);
		if (!log.stream->is_open())
			throw ERROR_THROW(111);
		return log;
	}

	void WriteLine(LOG_ log, char* c, ...) {
		char** pc = &c;
		while (*pc != "") {
			*log.stream << *pc;
			pc++;
		}
		*log.stream << "\n";
	}

	void WriteLog(LOG_ log) {
		time_t now = time(0);
		tm ltm;
		localtime_s(&ltm,&now);
		*log.stream << "------ ----- ---- Протокол  LOG ---- ----- ------ " << "\n";
		*log.stream << "Дата компиляции: "  << ltm.tm_mday << "."
											<< 1 + ltm.tm_mon << "."
											<< 1900 + ltm.tm_year << "\t"
											<< ltm.tm_hour << ":"
											<< ltm.tm_min << ":"
											<< ltm.tm_sec << "\n";
	}

	void WriteParm(LOG_ log, Parm::PARM parm) {
		*log.stream << "----- ----- ---- Параметры ---- ----- ----- ----- " << "\n";
		*log.stream << "-in: " << parm.in << "\n";
		*log.stream << "-out: " << parm.out << "\n";
		*log.stream << "-log: " << parm.log << "\n";
		*log.stream << "----- ----- ---- ---- ---- ---- ----- ----- ----- " << "\n";
	}

	void WriteIn(LOG_ log, In::_IN_ in) {
		*log.stream << "----- ----- ---- Исходные данные ---- ----- -----" << "\n";
		*log.stream << "Количество строк в исходном коде " << in.fullLines << "\n";
		*log.stream << "Количество символов в исходном коде " << in.fullsize << "\n";
		*log.stream << "----- ----- ----- ---- --- ---- ----- ----- ----- " << "\n";
		*log.stream << "----- ----- ---- Полученные данные --------- ----- " << "\n";
		*log.stream << "Количество строк в полученном коде " << in.lines << "\n";
		*log.stream << "Количество символов в полученном коде " << in.size << "\n";
		*log.stream << "Количество символов проигнорированных в коде " << in.ignored << "\n";
		*log.stream << "----- ----- ----- ---- ---- ---- ----- ----- ----- " << "\n";
	}

	void WriteError(LOG_ log, Error::ERROR_ e) {
		*log.stream << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << "\n";
		*log.stream << "Ошибка " << e.id << "\n";
		*log.stream << "Причина: " << e.message << "\n";
		if (e.inHandler.col > 0) {
			*log.stream << "Линия " << e.inHandler.line << "\n";
			*log.stream << "Позиция " << e.inHandler.col << "\n";
		}
		*log.stream << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << "\n";
	}

	void Close(LOG_ log) {
		log.stream->close();
		delete log.stream;
	}
}