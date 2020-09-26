﻿#define _CRT_SECURE_NO_WARNINGS

// REGULAR HANDLER
// you can enable or disable the regular express handler
// false:
//		in -in you get only handled program text
// true:
//		you enable regular express handler
#define REGULAR_HANDLER true

#define MAX_LEXEMS_LENGTH 1000

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

	int getAscii(char a) { // фция получения аски кода символа
		for (int i = 0; i < 256; i++)
			if ((char)i == a) return i;
		throw ERROR_THROW(0);
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

	int* getLineNums(string code) // функция вычисления номера строки исходного кода для его лексем
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

	void makeOutWithLT(LT::LexTable& table);
	void makeOutWithIT(IT::IdTable& idTable);

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

				// проверка на пустую строку
				if (strcmp(&line[0], "\0") == 0) continue;

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
		IT::IdTable idTable = IT::Create(IT_MAXSIZE); // создаю таблицу идентификаторов
		int* lexemsPosition = getLineNums(code); // получаю массив с номерами строк лексем ( позиций )
		char* lexem = strtok(&code[0], " \n");
		
		for(int i = 0, idxIndex = 1; lexem != NULL; i++)
		{
			LT::Entry entryLexem;
			entryLexem.sn = lexemsPosition[i]; // кидаем позицию лексемы в исходном коде
			entryLexem.lexema[0] = LT::compareLexems(lexem, idTable); // кидаем туда идентификатор лексемы
			/*
				в compareLexems() сразу идёт проверка на идентификатор и его добавление в таблицу!
				нет смысла писать отдельную функцию и again проходить все лексемы
			*/
			idxIndex = entryLexem.updateIndex(idxIndex); 

			LT::Add(lexTable, entryLexem);
			// cout << lexem << "[" << lexemsPosition[i] << "] :" << LT::compareLexems(lexem) << endl; // debug
			lexem = strtok(NULL, " \n");
		}

		// debug
		makeOutWithLT(lexTable);
		makeOutWithIT(idTable);

		return info;
	}

	// debug
	void makeOutWithLT(LT::LexTable& table)
	{
		LT::Entry* element = table.head;

		int i = 0;
		cout << "\tВЫВОД ТАБЛИЦЫ ЛЕКСЕМ:" << endl;
		while (element->next != nullptr)
		{
			cout << "\n" << element->sn << '\t';
			while(i == element->sn) {
				cout << element->lexema[0];
				element = element->next;
			}
			i++;
		}
	}

	// debug
	void makeOutWithIT(IT::IdTable& idTable)
	{
		cout << "\n---=-=-=-=-=-IT TABLE DEBUG=-=-=-=--=-------" << endl;
		IT::Entry* showTable = idTable.head;
		while (showTable)
		{
			cout << "\nshowTable->id = " << showTable->id << endl;
			cout << "showTable->iddatatype = " << showTable->iddatatype << endl;
			cout << "showTable->idtype = " << showTable->idtype << endl;
			cout << "showTable->idxfirstLE = " << showTable->idxfirstLE << endl;
			cout << "showTable->value.vstr->len = " << showTable->value.vstr->len << endl;
			cout << "showTable->value.vstr->str = " << showTable->value.vstr->str << endl;
			cout << "showTable->value.vint = " << showTable->value.vint << endl;
			showTable = showTable->next;
		}
	}
}