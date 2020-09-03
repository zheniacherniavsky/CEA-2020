#pragma once

#define PARM_IN		"-in:"
#define PARM_OUT	"-out:"
#define PARM_LOG	"-log:"
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT ".out"
#define PARM_LOG_DEFAULT ".log"
#define PARMS_COUNT 4				// include .exe parm
#define KEY_LENGTH 10
#define IN 1
#define OUT 2
#define LOG 3

namespace Parm {
	struct PARM
	{
		char in[PARM_MAX_SIZE];
		char out[PARM_MAX_SIZE];
		char log[PARM_MAX_SIZE];
	};

	PARM getparm(int argc, char* argv[]);
}