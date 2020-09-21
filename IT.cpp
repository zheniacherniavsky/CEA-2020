#include "IT.h"
#include "Error.h"
#include <iostream>

namespace IT
{
	IdTable Create(int size) // �������� ������� ���������������
	{
		IdTable newTable; // ������� ������ ��� �������
		
		if (size < IT_MAXSIZE) newTable.maxsize = size;
		else throw ERROR_THROW(200)

		newTable.size = 0; // ������� ������ �������
		newTable.table = new Entry; // ������� ������ ��� �������
		newTable.head = newTable.table; // ��������� ������ ������

		return newTable;
	}

	void Add(IdTable& idtable, Entry entry) // ���������� �������� � �������
	{
		if (idtable.table != nullptr)
		{
			for (int i = 0; i < ID_MAXSIZE; i++) // �������������
				idtable.table->id[i] = entry.id[i];

			if (idtable.size < idtable.maxsize) idtable.size++;
			// else throw "���� �� ������ ������ ��������"

			idtable.table->iddatatype = entry.iddatatype;

			idtable.table->idtype = entry.idtype;

			idtable.table->idxfirstLE = entry.idxfirstLE;

			idtable.table->value.vint = entry.value.vint; // ����������� �������� int
			for(int i = 0; i < entry.value.vstr->len; i++) // ����������� �������� char
				idtable.table->value.vstr->str[i] = entry.value.vstr->str[i];
			
			idtable.table->next = new Entry;
			idtable.table = idtable.table->next;
			idtable.table->next = nullptr;
		}
		else throw ERROR_THROW(201);
	}

	Entry GetEntry(IdTable& idtable, int n) // �������� ������� ������� ��������������� �� ������ ������
	{
		Entry* value = idtable.head;
		int pos = 0; // ������ ������
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
		throw ERROR_THROW(202) // �� ���������� ������� � ������ ������
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) // ������� ������ ������ (���� ����) � TI_NULLIDX (���� ���)
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