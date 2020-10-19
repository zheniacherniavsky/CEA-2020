#pragma once

namespace PN {
	bool PolishNotation( // создание польской записи
		int lextable_pos, // позиция выражения в таблице лексем
		LT::LexTable& lexTable, // таблица лексем
		IT::IdTable& idTable, // таблица идентификаторов
		bool debug = false
	);
}
