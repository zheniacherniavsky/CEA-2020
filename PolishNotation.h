#pragma once

namespace PN {
	void PolishNotation( // создание польской записи
		LT::LexTable& lexTable, // таблица лексем
		IT::IdTable& idTable, // таблица идентификаторов
		bool debug = false
	);
}
