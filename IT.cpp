#include "IT.h"
#include "Error.h"
#include <iostream>

namespace IT
{
	IdTable Create(int size) // создание таблицы идентификаторов
	{
		IdTable newTable; // выдел€ю пам€ть под таблицу
		
		if (size < IT_MAXSIZE) newTable.maxsize = size;
		else throw ERROR_THROW(200)

		newTable.size = 0; // текущий размер таблицы
		newTable.table = new Entry; // выдел€ю пам€ть под элемент
		newTable.head = newTable.table; // запоминаю начало списка

		return newTable;
	}

	void Add(IdTable& idtable, Entry entry) // добавление элемента в таблицу
	{
		if (idtable.table != nullptr)
		{
			for (int i = 0; i < ID_MAXSIZE; i++) // идентификатор
				idtable.table->id[i] = entry.id[i];

			if (idtable.size < idtable.maxsize) idtable.size++;
			// else throw "∆≈ ј Ќ≈ «јЅ”ƒ№ ќЎ»Ѕ ” ƒќѕ»—ј“№"

			idtable.table->iddatatype = entry.iddatatype;

			idtable.table->idtype = entry.idtype;

			idtable.table->idxfirstLE = entry.idxfirstLE;

			idtable.table->value.vint = entry.value.vint; // копирование значени€ int
			for(int i = 0; i < entry.value.vstr->len; i++) // копирование значени€ char
				idtable.table->value.vstr->str[i] = entry.value.vstr->str[i];
			
			idtable.table->next = new Entry;
			idtable.table = idtable.table->next;
			idtable.table->next = nullptr;
		}
		else throw ERROR_THROW(201);
	}

	Entry GetEntry(IdTable& idtable, int n) // получить элемент таблицы идентификаторов по номеру строки
	{
		Entry* value = idtable.head;
		int pos = 0; // перва€ строка
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
		throw ERROR_THROW(202) // Ќе обнаружена лексема в данной строке
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) // возврат номера строки (если есть) и TI_NULLIDX (если нет)
	{
		Entry* value = idtable.head;
		while (value)
		{
			if (strcmp(value->id, id) == 0) return value->idxfirstLE;
		}
		return IT_NULL_IDX;
	}

	void Delete(IdTable& idtable)
	{
		delete &idtable;
	}
}