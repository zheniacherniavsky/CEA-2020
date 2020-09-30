#include "Parm.h"
#include "Error.h"
#include <cstring>
#include <iostream> // del
namespace Parm {

	PARM fillEmpty(PARM p);

	PARM getparm(int argc, char* argv[]) {
		PARM parms = { "null", "out.txt", "log.txt" };
		if (argc > 4) throw ERROR_THROW(100);

		char keys[PARMS_COUNT][KEY_LENGTH]; // get keys
		for (int arg = 1; arg < argc; arg++) {
			for (int i = 0;; i++) {
				keys[arg][i] = argv[arg][i];
				if (argv[arg][i] == ':') {
					keys[arg][++i] = '\0';
					break;
				}
			}
		}

		bool checkIN = false; // check IN tag
		for (int i = 1; i < argc; i++)
			if (strcmp(keys[i], PARM_IN) == 0) checkIN = true;
		if (!checkIN) throw ERROR_THROW(101);

		for (int PARM = 1; PARM < argc; PARM++) { // fill parms
			if (strcmp(PARM_IN, keys[PARM]) == 0)
				for (int i = 0, q = strlen(keys[PARM]); q <= strlen(argv[PARM]); q++, i++)
					parms.in[i] = argv[PARM][q];
			if (strcmp(PARM_OUT, keys[PARM]) == 0)
				for (int i = 0, q = strlen(keys[PARM]); q <= strlen(argv[PARM]); q++, i++)
					parms.out[i] = argv[PARM][q];
			if (strcmp(PARM_LOG, keys[PARM]) == 0)
				for (int i = 0, q = strlen(keys[PARM]); q <= strlen(argv[PARM]); q++, i++)
					parms.log[i] = argv[PARM][q];
		}
		return parms;
	}

	PARM fillEmpty(PARM p) {
		char log[PARM_MAX_SIZE], out[PARM_MAX_SIZE];
		for (int i = 0; i <= strlen(p.log); i++) log[i] = p.log[i];
		for (int i = 0; i <= strlen(p.out); i++) out[i] = p.out[i];

		if (strcmp(log, "log.txt") == 0) {
			for (int i = 0; i < strlen(p.in); i++)
				p.log[i] = p.in[i];
			for (int i = strlen(p.in), j = 0; j < 4; i++, j++)
				p.log[i] = PARM_LOG_DEFAULT[j];
			p.log[strlen(p.log)] = '\0';
		}
		
		if (strcmp(out, "out.txt") == 0) {
			for (int i = 0; i < strlen(p.in); i++)
				p.out[i] = p.in[i];
			for (int i = strlen(p.in), j = 0; j < 4; i++, j++)
				p.out[i] = PARM_OUT_DEFAULT[j];
			p.out[strlen(p.out)] = '\0';
		}
		
		return p;
	}
}