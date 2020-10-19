#include "LT.h"
#include "IT.h"

#include <iostream>
#include <stack>
#include "PolishNotation.h"

/*
	ISSUES:
		- ������ ������ ������� �������, ������ 
		- ������ ������ ��� ��������� ����� ������
*/

namespace PN
{
	struct polishNotation
	{
		LT::Entry* result;
		LT::Entry* resultHead;
		std::stack <LT::Entry> symbols;

		polishNotation()
		{
			result = nullptr;
			resultHead = nullptr;
		}
	};

	bool isSymbol(LT::Entry* element);
	void addRusult(polishNotation& p, LT::Entry* element);

	bool PolishNotation( // �������� �������� ������
		int lextable_pos, // ������� ��������� � ������� ������
		LT::LexTable& lexTable, // ������� ������
		IT::IdTable& idTable, // ������� ���������������
		bool debug // ����� �������� ������
	)
	{
		/*
		�������� ����������:
		1 - �������� ������: ���������;
		- �������������� ������: �������� ������;
		- ����: ������;
		- �������� ������ ��������������� ����� �������;
		- �������� ����������� � �������������� ������ � ������� �� ����������;
		- �������� ������������ � ����, ���� ���� ���� ��� � ������� ����� ����� ���������� ������;
		- �������� ����������� ��� �������� � ������� ��� ������ ����������� � �������������� ������;
		- ����������� ������ ���������� � ����;
		- ����������� ������ ����������� ��� �������� �� ����������� ������, ����� ���� ��� ������ ������������;
		- �� ����� ������� �������� ������ ��� ��������, ���������� � �����, ������������� � �������������� ������
		*/

		// 1 - ��������� �������� ������ ���������

		// i=a*(b+c)
		// 
		// a*(b+c)     
		// *(b+c)       a	
		// (b+c)		a		*
		// b+c)			a		*(
		// +c)			ab		*(
		// c)			ab		*(+
		// )			abc		*(+
		//				abc+	*
		//				abc+*
		
		polishNotation pol;
		LT::Entry* sourceStr = LT::GetEntry(lexTable, lextable_pos);
		LT::Entry* nextLine = LT::GetEntry(lexTable, lextable_pos + 1);
		LT::Entry* head = sourceStr;
		
		LT::Entry* beforePN = sourceStr;

		if (debug)
		{
			std::cout << "�� �������� ������:" << std::endl;
			while (beforePN->sn == lextable_pos)
			{
				std::cout << beforePN->lexema[0];
				beforePN = beforePN->next;
			}
			std::cout << std::endl;
		}
		
		// algorithm
		/*
		
		while(������ ��������� � ���������)
		{
			if(�� ������ ��������)
			{
				���������� � �������������� ������, ������� false � ���. �������
			}
			else // � ��� ������ ��������
			{
				if(�������� �� ������ ������) { ... } // ���������� ��������� ������ ������
				else
				{
					...  ���������� ����� ��� ���������� � �������������� ������
				}
			}
		}

		*/


		while (sourceStr != nullptr && sourceStr->sn == lextable_pos)
		{
			char s = sourceStr->lexema[0]; // lex symbol
			if (!isSymbol(sourceStr))
			{
				if (s == LEX_SEMICOLON)
				{
					while (!pol.symbols.empty())
					{
						addRusult(pol, &pol.symbols.top()); // ��������� � ������ ���� �����
						pol.symbols.pop(); // ������� ��� ����� �� �����
					}
				}
				else if (s != LEX_ID && s != LEX_IS)
				{
					if (debug) {
						std::cout << "�������� ������ ��� ���� ������� �� ���������!" << std::endl;
					}
					return false;
				}
				addRusult(pol, sourceStr); // ���� �������������, �� ������ � ������
			}
			else
			{
				if (s == LEX_RIGHTHESIS) // ���� ��������� ), ��
				{
					while (pol.symbols.top().lexema[0] != LEX_LEFTHESIS) // ���� �� �������� (
					{
						addRusult(pol, &pol.symbols.top()); // ��������� � ������ ���� �����
						pol.symbols.pop(); // ������� ��� ����� �� �����
					}
					pol.symbols.pop(); // ������� (
				}
				else
				{
					if (!pol.symbols.empty())
					{
						while ((pol.symbols.top().priority >= sourceStr->priority) && sourceStr->priority != 0)
						{
							addRusult(pol, &pol.symbols.top()); // ��������� � ������ ���� �����
							pol.symbols.pop(); // ������� ��� ����� �� �����
							if (pol.symbols.empty()) break;
						}
					}
					pol.symbols.push(*sourceStr); // ����� ��������� ������ � ����
				}
			}
			sourceStr = sourceStr->next;
		}

		if (debug)
		{
			std::cout << "\n����� �������� ������:" << std::endl;
			LT::Entry* out = pol.resultHead;
			while (out != nullptr)
			{
				std::cout << out->lexema[0];
				out = out->next;
			}
			std::cout << std::endl;
		}

		// ISSUE: ������ ������, ������� lextable

		head->idxTI = pol.resultHead->idxTI;
		head->lexema[0] = pol.resultHead->lexema[0];
		head->priority = pol.resultHead->priority;
		head->sn = pol.resultHead->sn;

		head->next = pol.resultHead->next;
		while (head->next != nullptr) head = head->next;
		head->next = nextLine;
		
		return true;
	}

	void addRusult(polishNotation& p, LT::Entry* element)
	{
		if (p.result == nullptr)
		{
			p.result = new LT::Entry();
			p.resultHead = p.result;
		}
		else
		{
			p.result->next = new LT::Entry();
			p.result = p.result->next;
		}

		p.result->idxTI = element->idxTI;
		p.result->lexema[0] = element->lexema[0];
		p.result->sn = element->sn;
	}
	
	bool isSymbol(LT::Entry* element)
	{
		const char* s = "(),+-*/"; // ISSUES: add []

		for (int i = 0; i < strlen(s); i++)
		{
			if (element->lexema[0] == s[i]) return true;
		}
		return false;
	}
}
