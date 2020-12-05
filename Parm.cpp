#include "Parm.h"
#include "Error.h"
#include <cstring>
#include <iostream> // del
namespace Parm {

	PARM getparm(int argc, char* argv[]) {
		PARM parms = { "", "code.asm", "log.txt", false, false, false };
		if (argc > 7) throw ERROR_THROW(100);
		char* buf = (char*)calloc(255, sizeof(char));

		for (int i = 1; i < argc; i++)
		{
			for (int pos = 0; pos <= strlen(argv[i]); pos++)
			{
				buf[pos] = argv[i][pos];
				if (buf[pos] == ':')
				{
					if (strcmp(buf, PARM_IN) == 0)
					{
						for (int j = 0;; j++)
						{
							parms.in[j] = argv[i][++pos];
							if (parms.in[j] == '\0') break;
						}
					}
					else if (strcmp(buf, PARM_LOG) == 0)
					{
						for (int j = 0;;)
						{
							parms.log[j] = argv[i][++pos];
							if (parms.log[j] == '\0') break;
						}
					}
					else if (strcmp(buf, PARM_OUT) == 0)
					{
						for (int j = 0;;)
						{
							parms.out[j] = argv[i][++pos];
							if (parms.out[j] == '\0') break;
						}
					}
				}
				else if (buf[pos] == '\0')
				{
					if (strcmp(buf, "-lex") == 0) parms.lex = true;
					else if (strcmp(buf, "-id") == 0) parms.id = true;
					else if (strcmp(buf, "-tree") == 0) parms.tree = true;
				}
			}
		}

		return parms;
	}
}