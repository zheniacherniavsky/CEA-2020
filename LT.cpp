#include "LT.h"
#include "Error.h"
#include "IT.h"
#include "FST.h"

#include <iostream>

namespace LT
{
	int Entry::updateIndex(int i)
	{
		if (lexema[0] == 'i')
		{
			idxTI = i;
			i++;
		}
		else if (lexema[0] == 'l') {
			idxTI = i;
			i++;
		}

		else idxTI = IT_NULL_IDX;
		return i;
	}

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
			
			if (LT.size <= LT.maxsize) LT.size++;
			// else throw "ЖЕКА НЕ ЗАБУДЬ ОШИБКУ ДОПИСАТЬ"

			LT.table->idxTI = entry.idxTI;
			LT.table->sn = entry.sn;
			LT.table->priority = entry.priority;
			// выделяем память и перемещаем наш лист туда
			// при этом начало списка сохраняется в LT
			LT.table->next = new Entry;
			LT.table = LT.table->next;
			LT.table->next = nullptr;
		}
		else throw ERROR_THROW(201); // данной таблицы не существует или её компоненты не проинициализированы.
	}

	Entry* GetEntry(LexTable& LT, int n) // получаем строку по номеру выражения
	{
		Entry* value = LT.head;
		int pos = 0; // первая строка
		while (value != nullptr)
		{
			if (pos == n)
			{
				return value;
			}
			else
			{
				value = value->next;
				if (value != nullptr)
					pos = value->sn;
				else return nullptr;
			}
		}
		throw ERROR_THROW(202) // Не обнаружена лексема в данной строке
	}

	int getSnByPos(LexTable& lex, int pos)
	{
		Entry* element = lex.head;
		for (int i = 0; i < pos; i++)
		{
			if (element->next != NULL)
				element = element->next;
		}
		if (element) return element->sn;
		else return 0;
	}

	void Delete(LexTable& LT)
	{
		delete &LT;
	}
}
