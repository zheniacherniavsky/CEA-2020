#include "LT.h"
#include "Error.h"

#include <iostream>

namespace LT
{
	LexTable Create(int size)
	{
		if (size < LT_MAXSIZE)
		{
			LexTable* LT = new LexTable; // �������� ������ ��� �������
			LT->size = size; // ��������� �������
			LT->table = new Entry; // �������� ������ ��� ��������

			LT->head = LT->table; // ������ ������
			LT->next = nullptr; // ��������� ������� ������

			// ������ ����� �����������, �.�. ������������ � ����������� �������� � �������
			// ������ �� ����� �������� ������

			return *LT; // ���������� ��������� �� ���� �������
		}
		else throw ERROR_THROW(200); // �������� ������ ����������� ������� 
	}

	void Add(LexTable& LT, Entry entry) // ����������
	{
		if (LT.table != nullptr)
		{
			for (int i = 0; i < strlen(entry.lexema); i++)
				LT.table->lexema[i] = entry.lexema[i];

			LT.table->idxTI = entry.idxTI;
			LT.table->sn = entry.sn;
		}
		else throw ERROR_THROW(201); // ������ ������� �� ���������� ��� � ���������� �� �������������������.
	}

	Entry GetEntry(LexTable& LT, int n) // �������� ������ �� 
	{
		Entry* value = LT.table;
		int pos = 1; // ������ ������
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
		throw ERROR_THROW(202) // �� ���������� ������� � ������ ������
	}

	void Delete(LexTable& LT)
	{
		delete &LT;
	}
}
