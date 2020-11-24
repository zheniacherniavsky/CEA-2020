#pragma once
#include "GRB.h"
#define GRB_ERROR_SERIES 600

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'),
		6,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, // ошибка в структуре
			6, // S -> m{NrE;}; | tfi(F){NrE;};S | tfi(F){NrE;}; | m{NrE;};S
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), TS(';')),
			Rule::Chain(6, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';')),
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(9, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1, // ошибка в операторах
			11, // N -> dti; | rE; | i=E; | dti;N | i=E;N | dti=E; | dti=E;N
			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), TS('i'), TS(';')),
			Rule::Chain(4, TS('s'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(3, TS('s'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2, // ошибка в выражении 
			9,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 3, // ошибка в выражении
			4,
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(2, TS('/'), NS('E'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4, // ошибка в параметрах функции
			2,		//F -> ti | ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 5, // ошибка в параметрах вызываемой функции
			4,		//W -> i | l | i,W | l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);
}