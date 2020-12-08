#include "IT.h"
#include "LT.h"
#include "Error.h"
#include <iostream>
#include <math.h>

namespace IT
{
	bool equal(char a[ID_MAXSIZE], char b[ID_MAXSIZE]);

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

	int convertTo10(int num8)
	{
		int num10 = 0;
		int a = 1;
		int pos = 0;
		while (true)
		{
			int check = (num8 / a) % 10;
			if (check > 7) throw Error::geterror(2);
			num10 += check * pow(8, pos);
			if (num8 % a == num8) break;
			else
			{
				a *= 10;
				pos++;
			}
		}
		return num10;
	}

	void Add(IdTable& idtable, Entry entry)
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
				idtable.table->id[i] = entry.id[i];
			}

			idtable.table->iddatatype = entry.iddatatype;
			idtable.table->idtype = entry.idtype;
			idtable.table->idxTI = entry.idxTI;
			idtable.table->idxfirstLE = entry.idxfirstLE;
			idtable.table->declared = entry.declared;
			if (entry.value.vint8 != NULL)
			{
				idtable.table->value.vint = convertTo10(entry.value.vint8);
			}
			if(entry.value.vstr != NULL) idtable.table->value.vstr->len = entry.value.vstr->len;

			idtable.table->visibility.area = entry.visibility.area;
			for (int i = 0; i < strlen(entry.visibility.functionName); i++)
				idtable.table->visibility.functionName[i] = entry.visibility.functionName[i];

			if (entry.value.vstr->len < 256)
			{
				for (int i = 0; i < entry.value.vstr->len; i++)
				{
					if (entry.value.vstr->str[i] == '_') idtable.table->value.vstr->str[i] = ' ';
					else idtable.table->value.vstr->str[i] = entry.value.vstr->str[i];
				}
			}	
			idtable.size++;
		}
		else throw ERROR_THROW(0) // допилить ошибку для переувеличения размера таблицы id	
	}

	Entry* GetEntry(IdTable& idtable, int idx)
	{
		Entry* element = new Entry();

		element = idtable.head;
		while (element)
		{
			if (element->idxTI == idx) return element;
			else element = element->next;
		}
		return NULL;
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE], short visArea, char fnkName[])
	{
		Entry* element = new Entry();

		element = idtable.head;
		while (element)
		{
			if (equal(element->id, id))
			{
				if (equal(element->visibility.functionName, fnkName)) // параметры и переменные
					return element->idxTI;
				else if (element->idtype == IT::F) // вызов функции
					return element->idxTI;		
			}	
			element = element->next;
		}
		return IT_NULL_IDX;
	}

	bool equal(char a[ID_MAXSIZE], char b[ID_MAXSIZE])
	{
		if (strlen(a) != strlen(b)) return false;
		else
		{
			for (int i = 0; i < strlen(b); i++)
				if (a[i] != b[i]) return false;
			return true;
		}
	}
}