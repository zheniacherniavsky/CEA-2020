#include "IT.h"
#include "Error.h"
#include <iostream>

namespace IT
{
	IdTable Create(int size)
	{
		IdTable newTable;

		if (size <= IT_MAXSIZE)
			newTable.maxsize = size;
		else throw ERROR_THROW(0); // допилить ошибку на превышение макс размера таблы

		newTable.size = 0;
		newTable.table = new Entry(); // выделяем память
		newTable.head = nullptr; // голова пока пустая. Её буду заполнять при добавлении элементов
		
		return newTable;
	}

	void Add(IdTable& idtable, Entry* entry)
	{
		if (idtable.size < idtable.maxsize) // размер должен быть меньше максимального размера таблицы 
		{
			if (idtable.head == nullptr)
			{
				idtable.head = idtable.table; // ставим начало списка на этот элемент, т.к. он бует первым
			}
			else
			{
				idtable.table->next = new Entry(); // иначе выделяем память под следующий элемент
				idtable.table = idtable.table->next; // и переходим на него
			}

			// дальше просто заполняем данными
			for (int i = 0; i < ID_MAXSIZE; i++)
			{
				idtable.table->id[i] = entry->id[i];
			}

			idtable.table->iddatatype = entry->iddatatype;
			idtable.table->idtype = entry->idtype;
			idtable.table->idxfirstLE = entry->idxfirstLE;
			idtable.table->value.vint = entry->value.vint;
			idtable.table->value.vstr->len = entry->value.vstr->len;

			for (int i = 0; i < entry->value.vstr->len; i++)
				idtable.table->value.vstr->str[i] = entry->value.vstr->str[i];

			idtable.size++;
		}
		else throw ERROR_THROW(0) // допилить ошибку для переувеличения размера таблицы id
		
	}
}