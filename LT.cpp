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
			LT->maxsize = size; // ��������� ������
			LT->size = 0;
			LT->table = new Entry;

			LT->head = LT->table; // ������ ������ ���������

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
			LT.table->lexema[0] = entry.lexema[0];
			
			if (LT.size <= LT.maxsize) LT.size;
			// else throw "���� �� ������ ������ ��������"

			LT.table->idxTI = entry.idxTI;
			LT.table->sn = entry.sn;
			// �������� ������ � ���������� ��� ���� ����
			// ��� ���� ������ ������ ����������� � LT
			LT.table->next = new Entry;
			LT.table = LT.table->next;
		}
		else throw ERROR_THROW(201); // ������ ������� �� ���������� ��� � ���������� �� �������������������.
	}

	Entry GetEntry(LexTable& LT, int n) // �������� ������ �� 
	{
		Entry* value = LT.head;
		int pos = 1; // ������ ������
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

	void Delete(LexTable& LT)
	{
		delete &LT;
	}

	// ������� ��������� ������ �� ��������������
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
			// �������� _ �� ������
			return LEX_LITERAL;
		}
		else return LEX_ID;
	}
}
