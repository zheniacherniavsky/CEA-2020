#pragma once
#define IN_MAX_LEN_TEXT 1024*1024
#define IN_MAX_LEN_LINE 256
#define IN_CODE_ENDL '\n'

#include "LT.h"
#include "IT.h"

namespace In {
	enum { T = 1024, F = 2048, I = 4096 };
	struct _IN_ {
		int fullsize;
		int size;
		int lines;
		int fullLines;
		int ignored;
		char* text;
	};

	// таблица проверки входной информации, индекс = код (W1251) символа
	// F - запрещенный символ, T - разрешенный символ, I - игнорировать (не вводить),
	//		если 0 <= значение < 256 - то вводится данное значение
	const unsigned short IN_CODE_TABLE[256] = {
	// 0     1      2      3      4      5      6      7      8      9     10     11     12     13     14      15
	'\n',  In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::I, In::T, In::T, In::T, In::T, In::T, In::T, // 1
	// 16
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 2
	// 32
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 3
	// 48
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 4
	// 64
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 5
	// 80
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 6
	// 96
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 7
	// 112
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 8
	// 128
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 9
	// 144
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 10
	// 160
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 11
	// 176
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 12
	// 192
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 13
	// 208
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 14
	// 224
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, // 15
	// 240
	In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T, In::T  // 16
	};

	_IN_ getin(LT::LexTable& lexTable, IT::IdTable& idTable, char* dir, char* outdir);
};