#pragma once
#include "Error.h"
typedef short GRBALPHABET; // терминалы > 0, нетерминалы < 0

#define NS(s) (GRB::Rule::Chain::N(s))
#define TS(s) (GRB::Rule::Chain::T(s))
#define ISNS(n) (GRB::Rule::Chain::isN(n))

namespace GRB
{
	struct Rule
	{
		GRBALPHABET nn;	// нетерминал
		int iderror; // идентификатор диагностического сообщения
		short size; // кол-во цепочек ( правых частей правила)
		struct Chain
		{
			short size; // length
			GRBALPHABET* nt; // цепочка нетерминалов
			Chain()
			{
				size = 0;
				nt = 0;
			}
			Chain(
				short psize, // кол-во символов в цепочке
				GRBALPHABET s, ... // символы
			);
			// delete statik !
			char* getCChain(char* b); // получить правую сторону правила
			static GRBALPHABET T(char t) { return GRBALPHABET(t); }; // вернет терминал > 0
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }; // возращает нетерминал < 0
			static bool isT(GRBALPHABET s) { return s > 0; };
			static bool isN(GRBALPHABET s) { return !isT(s); }; // ????????????
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };
		}*chains;
		Rule() { nn = 0x00; size = 0; };

		Rule(
			GRBALPHABET pnn, // нетерминал
			int piderror,
			short psize,
			Chain c, ...
		);

		char* getCRule( // выводит для распечатки в виде N->цепочка
			char* b, // буффер
			short nchain // номер цепочки
		);

		short getNextChain( // получить следующую ща j цепочку и вернуть ее или -1
			GRBALPHABET t, // ервый символ цепочки
			Chain& pchain, // возращаемая цепочка
			short j // номер цепочки
		);
	};

	struct Greibach
	{
		short size; // кол-во правил
		GRBALPHABET startN; // стартовый символ
		GRBALPHABET stbottomT; // дно стека
		Rule* rules; // множество правил
		Greibach() { short size = 0; startN = 0; stbottomT = 0; rules = 0; };

		Greibach(
			GRBALPHABET pstartN, // стартовый символ
			GRBALPHABET pstbottomT, // дно стека
			short psize, // кол-во правил
			Rule r, ... // правила
		);

		short getRule( // получить правило, вернет его номер или -1
			GRBALPHABET pnn, // левый символ правила
			Rule& prule // возращаемое правило грамматики 
		);
		Rule getRule(short n); // получить правило
	};

	Greibach getGreibach(); // получить грамматику
}