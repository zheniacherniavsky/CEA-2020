#include "In.h"
#include "Error.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "FST.h"
#include "FT.h"

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

namespace In {

	int getAscii(char a) { // ф-ция получения аски кода символа
		for (int i = 0; i < 256; i++)
			if ((char)i == a) return i;
		throw ERROR_THROW(0); // символ в таблице не найден?
	}

	bool checkSeparators(string &code, char symbol) // функция проверки на лексемы отдельных символов
	{
		const char* separators = ";,()+-*/{}=";
		for (int i = 0; i < strlen(separators); i++) {
			if (separators[i] == symbol) 
			{
				code = code + " " + symbol + " ";
				return true;
			}
		}
		return false;
	}

	_IN_ getin(LT::LexTable& lexTable, IT::IdTable& idTable, char* dir, char* outdir)
	{
		_IN_ info;
		info.fullsize = 0;
		info.size = 0;
		info.text = new char[IN_MAX_LEN_TEXT];
		info.lines = 0;
		info.fullLines = 0;
		info.ignored = 0;

		/*
			....
			Сoздаётся LT и туда в односвязный список добавляются элементы

			5.  После этого лексемы можно пустить по автомату и дать ему уникальный ID
			Мы имеем лексему, а так же строку исходного кода этой лексемы, поэтому мы сможем обратно построить\
			код, который будет состоять только из уникальных идентификаторов.

		*/

		ifstream sourceCode(dir); // открываем наш файл

		if (!sourceCode.is_open()) ERROR_THROW(0); // выкидываем ошибку если не удалось открыть файл

		// ------------------------------------------------------------------
		//	Первая стадия - подготовка исходного кода для разбития на лексемы
		// ------------------------------------------------------------------

		string line; // сюда получаем строку исходного кода и обрабатываем её
		string code = ""; // полученный код

		while (getline(sourceCode, line))
		{
			if (line[0] == '.')
			{
				continue; // служебный комментарий 
			}
			bool _literal = false;
			for (int i = 0, spaceControl = 0; i <= line.length(); i++)
			{
				if (checkSeparators(code, line[i])) continue; // добавление пробелов для разделения между лексемами

				// проверка на литерал
				if (line[i] == '\"' || line[i] == '\'')
					_literal ? _literal = false : _literal = true;
				if (line[i] == ' ' && _literal)
				{
					code += '_';
					continue;
				}

				// Проверка символа на допустимость
				int a = IN_CODE_TABLE[getAscii(line[i])];

				if (a == T)
				{
					code += line[i];
					info.size++;
					continue;
				}
				else if (a == I)
				{
					info.ignored++;
					continue;
				}
				else if (a == F)
				{
					throw ERROR_THROW_IN(110, info.lines, i + 1);
				}
				else
				{
					code += (char)IN_CODE_TABLE[getAscii(line[i])];
					info.size++;
				}
			}
			info.lines++;
		}
		sourceCode.close(); // закрываем файл
		// в code находится обработанный код
		// в sCode находится исходный код
		FT::fillTables(&code[0], lexTable, idTable); // ф-ция заполнения таблиц
		return info;
	}
}