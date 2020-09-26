#define _CRT_SECURE_NO_WARNINGS

// REGULAR HANDLER
// you can enable or disable the regular express handler
// false:
//		in -in you get only handled program text
// true:
//		you enable regular express handler
#define REGULAR_HANDLER true

#define MAX_LEXEMS_LENGTH 100

#include "In.h"
#include "Error.h"
#include "Log.h"
#include "FST/FST.h"
#include "LT.h"

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

namespace In {

	int getAscii(char a) {
		for (int i = 0; i < 256; i++)
			if ((char)i == a) return i;
		throw ERROR_THROW(0);
	}

	_IN_ getin(char* dir, char* outdir) {
		_IN_ info;
		info.fullsize = 0;
		info.size = 0;
		info.text = new char[IN_MAX_LEN_TEXT];
		info.lines = 0;
		info.fullLines = 0;
		info.ignored = 0;

		/*
			План работы на данный момент.
			1. получаем строку исходного кода
			2. выполняется функция trim() (В этой строке удаляются лишние пробелы и табы)
			3. попробовать разбить код на лексемы
			4. создать список лексем вместе с номером строки исходного кода
			пример:
			0 int
			0 function
			0 (
			0 int
			0 a
			0 )
			0 ;
			1 string
			1 value

			Сoздаётся LT и туда в односвязный список добавляются элементы

			5.  После этого лексемы можно пустить по автомату и дать ему уникальный ID
			Мы имеем лексему, а так же строку исходного кода этой лексемы, поэтому мы сможем обратно построить\
			код, который будет состоять только из уникальных идентификаторов.

		*/

		ifstream sourceCode(dir); // открываем наш файл
		char line[IN_MAX_LEN_LINE]; // делаю переменную для записи строки
		if (!sourceCode.is_open) ERROR_THROW(0); // выкидываем ошибку если не удалось открыть файл
		
		while (!sourceCode.eof()) // пока исходный код не дойдёт до конца
		{
			sourceCode.getline(line, IN_MAX_LEN_LINE); // читаю строку в переменную line
			char newLine[IN_MAX_LEN_LINE];

			for (int i = 0, j = 0, spaceControl = 0; i < strlen(line); i++) // цикл будет проходиться по всем символам
			{
				if (getAscii(line[i]) == 0 && i == 0) // если == \n и первый символ - пропуск
				{
					info.ignored++;
					continue; //
				}

				// check spaces like a trim() function
				if ((getAscii(line[i]) == 32) && i == spaceControl) {
					info.ignored++;
					spaceControl++;
					continue;
				}
				else if (getAscii(line[i]) == 32 && getAscii(line[i + 1]) == 32) {
					info.ignored++;
					continue;
				}

				if (IN_CODE_TABLE[getAscii(line[i])] == T) {
					newLine[j] = line[i];
					j++;
				} // если символ разрешен, то пропускаем его.

				else if (IN_CODE_TABLE[getAscii(line[i])] == F) {
					throw ERROR_THROW_IN(110, info.lines, i + 1);
				} // если не, то выкидываем ошибку

				else if (IN_CODE_TABLE[getAscii(line[i])] == I) {
					info.ignored++;
					continue;
				} // или игнорируем этот символ

				else {
					newLine[j] = (char)IN_CODE_TABLE[getAscii(line[i])];
					j++;
				} // или заменяем его
			}
			newLine[strlen(newLine)] = '\0';

			for (int i = 0; i < strlen(newLine); i++)
			{
				cout << newLine[i];
			}
		}
		


		return info;
	}
}