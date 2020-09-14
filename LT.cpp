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
			LT->size = size; // указываем размерн
			LT->table = new Entry; // выделяем память под элементы

			LT->head = LT->table; // голова списка
			LT->next = nullptr; // следующий элемент списка

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
			for (int i = 0; i < strlen(entry.lexema); i++)
				LT.table->lexema[i] = entry.lexema[i];

			LT.table->idxTI = entry.idxTI;
			LT.table->sn = entry.sn;
		}
		else throw ERROR_THROW(201); // данной таблицы не существует или её компоненты не проинициализированы.
	}

	Entry GetEntry(LexTable& LT, int n) // получаем строку по 
	{
		Entry* value = LT.table;
		int pos = 1; // первая строка
		while (value)
		{
			if (pos == n)
			{
				return *value;
			}
			else {
				value = LT.next;
			}
		}
		throw ERROR_THROW(202) // Не обнаружена лексема в данной строке
	}

	void Delete(LexTable& LT)
	{
		delete &LT;
	}
}
