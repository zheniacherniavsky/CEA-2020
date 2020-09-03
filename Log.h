#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"

namespace Log {
	struct LOG_ {
		char* logfile = new char[PARM_MAX_SIZE];
		std::ofstream* stream;
	};

	static const LOG_ INITLOG = { (char*)"", NULL};
	LOG_ getlog(char* logfile);
	void WriteLine(LOG_ log, char* c, ...);
	void WriteLog(LOG_ log);
	void WriteParm(LOG_ log, Parm::PARM parm);
	void WriteIn(LOG_ log, In::_IN_ in);
	void WriteError(LOG_ log, Error::ERROR_ e);
	void Close(LOG_ log);
}