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

	bool checkSeparators(string &code, char symbol)
	{
		const char* separators = ";,()+-*/{}";
		for (int i = 0; i < strlen(separators); i++) {
			if (separators[i] == symbol) 
			{
				code = code + " " + symbol + " ";
				return true;
			}
		}
		return false;
	}

	int* getLineNums(string code)
	{
		int* array = new int[MAX_LEXEMS_LENGTH];
		int size = 0;
		for (int i = 0, line = 0, pos = 0; i < code.length(); i++)
		{
			if (code[i] != ' ')
			{
				if (code[i] == '\n') {
					line++;
					continue;
				}

				array[pos] = line;
				pos++; size++;
				while (code[i] != ' ') {
					if (code[i] == '\n' || code[i] == ' ') {
						i--;
						break;
					}
					i++;
				}
			}
		}
		return array;
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
			bool _literal = false;
			for (int i = 0, spaceControl = 0; i <= line.length(); i++)
			{
				if(checkSeparators(code, line[i])) continue; // добавление пробелов для разделения между лексемами

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

		// в code находится обработанный код, теперь разбиваем всё на лексемы
		// и пихаю это всё в таблицу LexTable
		LT::LexTable lexTable = LT::Create(MAX_LEXEMS_LENGTH); // создаю таблицу лексем
		int* lexemsPosition = getLineNums(code); // получаю массив с номерами строк лексем ( позиций )
		char* lexem = strtok(&code[0], " \n");
		
		for(int i = 0; lexem != NULL; i++)
		{
			LT::Entry entryLexem;
			entryLexem.sn = lexemsPosition[i]; // кидаем позицию лексемы в исходном коде
			entryLexem.lexema[0] = LT::compareLexems(lexem); // кидаем туда идентификатор лексемы
			LT::Add(lexTable, entryLexem);
			cout << lexem << "[" << lexemsPosition[i] << "] :" << LT::compareLexems(lexem) << endl; // debug
			lexem = strtok(NULL, " \n");
		}


		cout << "вова: " << LT::GetEntry(lexTable, 1).lexema[0] << endl;


		LT::Entry entryLexem = *lexTable.head;
		for (;;)
		{
			if (&entryLexem.next == nullptr) break;
			cout << entryLexem.lexema[0] << endl;
			entryLexem = *entryLexem.next;
		}

		return info;
	}
}