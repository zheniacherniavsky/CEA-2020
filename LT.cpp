#include "LT.h"
#include "Error.h"

#include <iostream>

namespace LT
{
	LexTable Create(int size)
	{
		if (size < LT_MAXSIZE)
		{
			LexTable* LT = new LexTable; // выделяем память под таблицу
			LT->maxsize = size; // указываем размер
			LT->size = 0;
			LT->table = new Entry;

			LT->head = LT->table; // ставим начало указателя

			// список будет односвязный, т.к. возвращаться к предыдущему элементу в таблице
			// лексем не имеет никакого смысла

			return *LT; // возвращаем указатель на нашу таблицу
		}
		else throw ERROR_THROW(200); // превышен размер создаваемой таблицы 
	}

	void Add(LexTable& LT, Entry entry) // добавление
	{
		if (LT.table != nullptr)
		{	
			LT.table->lexema[0] = entry.lexema[0];
			
			if (LT.size <= LT.maxsize) LT.size;
			// else throw "ЖЕКА НЕ ЗАБУДЬ ОШИБКУ ДОПИСАТЬ"

			LT.table->idxTI = entry.idxTI;
			LT.table->sn = entry.sn;
			// выделяем память и перемещаем наш лист туда
			// при этом начало списка сохраняется в LT
			LT.table->next = new Entry;
			LT.table = LT.table->next;
		}
		else throw ERROR_THROW(201); // данной таблицы не существует или её компоненты не проинициализированы.
	}

	Entry GetEntry(LexTable& LT, int n) // получаем строку по 
	{
		Entry* value = LT.head;
		int pos = 1; // первая строка
		while (value)
		{
			if (pos == n)
			{
				return *value;
			}
			else 
			{
				value = value->next;
				pos++;
			}
		}
		throw ERROR_THROW(202) // Не обнаружена лексема в данной строке
	}

	void Delete(LexTable& LT)
	{
		delete &LT;
	}

	// функция изменения лексем на идентификаторы
	char compareLexems(char* lexem) {
		const int SIZE = 17;
		const char* lexems[SIZE] = { "integer","string", "function", "declare",
							"return", "print", ";", ",", "{", "}", "(",
							")", "+", "-", "*", "/", "=" };

		char idLexems[SIZE] = { 't', 't', 'f', 'd', 'r', 'p', ';', ',', '{', '}', '(', ')', '+', '-', '*', '/', '=' };

		for (int i = 0; i < SIZE; i++) {
			if (strcmp(lexem, lexems[i]) == 0)
				return idLexems[i];

		}
		
		if (lexem[0] == *"\"" || lexem[0] == *"\'") {
			// поменять _ на пробел
			return LEX_LITERAL;
		}
		else return LEX_ID;
	}
}
