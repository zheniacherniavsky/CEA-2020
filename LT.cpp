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
			// выделяем память и перемещаем наш лист туда
			// при этом начало списка сохраняется в LT
			LT.table->next = new Entry;
			LT.table = LT.table->next;
			LT.table->next = nullptr;
		}
		else throw ERROR_THROW(201); // данной таблицы не существует или её компоненты не проинициализированы.
	}

	Entry GetEntry(LexTable& LT, int n) // получаем строку по 
	{
		Entry* value = LT.head;
		int pos = 0; // первая строка
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

	/* char compareLexems(char* lexem) // функция изменения лексем 
	{
		FST::FST fst2(lexem, 8,
		FST::NODE(1, FST::RELATION('i', 1)),
		FST::NODE(1, FST::RELATION('n', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('e', 4)),
		FST::NODE(1, FST::RELATION('g', 5)),
		FST::NODE(1, FST::RELATION('e', 6)),
		FST::NODE(1, FST::RELATION('r', 7)),
		FST::NODE());

		FST::FST fst1(lexem, 2,
			FST::NODE(1, FST::RELATION('(', 1)),
			FST::NODE());

		FST::FST fst3(lexem, 7,
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('u', 4)),
			FST::NODE(1, FST::RELATION('r', 5)),
			FST::NODE(1, FST::RELATION('n', 6)),
			FST::NODE());

		if (FST::execute(fst1) == -1 || FST::execute(fst2) == -1 || FST::execute(fst3) == -1)
		{
			std::cout << "(+) Цепочка распознана:\t\t\t" << lexem << "\n";
		}
		else
		{
			std::cout << "(-) Цепочка не распознана:\t\t" << lexem << "\n";
		}

		return LEX_ID;
		/*
		// в этой функции идёт проверка на идентификатор, и я сразу же при 
		// обнаружении идентификатора я закидываю его в таблицу
		const int SIZE = 17;
		const char* lexems[SIZE] = { "integer","string", "function", "declare",
							"return", "print", ";", ",", "{", "}", "(",
							")", "+", "-", "*", "/", "=" };

		char idLexems[SIZE] = { 't', 't', 'f', 'd', 'r', 'p', ';', ',', '{', '}', '(', ')', '+', '-', '*', '/', '=' };

		for (int i = 0; i < SIZE; i++) {
			if (strcmp(lexem, lexems[i]) == 0)
				return idLexems[i];
		}

		if (lexem[0] == '\"' || lexem[0] == '\'') {
			
			//	Тут мы понимаем, что работаем с идентификатором.
			//	Значит нужно его добавить в таблицу, но сначала создадим объект с
			//	информации об этом идентификаторе
			
			IT::Entry *elem = new IT::Entry(); // выделяю память под элемент
			// elem.id - имя идентификатора, сразу укорачивается до ID_MAXSIZE (5)
			for (int i = 0; i < strlen(lexem); i++)
			{
				if (i == ID_MAXSIZE) break;
				elem->id[i] = lexem[i];
			}

			// поменять _ на пробел в нашей лексеме, т.к. идентификатор уже указали
			for (int i = 1; i < strlen(lexem); i++) // от единицы т.к. убираю кавычку
				if (lexem[i] == '_') lexem[i] = ' ';

			elem->iddatatype = IT::STR; // у нас строковый литерал, поэтому STR
			elem->idtype = IT::V; // !!!! придумать как определить тип !!!!
			elem->idxfirstLE = 0; // пока что 0, т.к. не надо сейчас.
			elem->value.vstr->len = strlen(lexem);
			for (int i = 0; i < elem->value.vstr->len; i++) // от 1 до len-1 т.к убираем кавычки 
			{
				elem->value.vstr->str[i] = lexem[i];
			}

			IT::Add(idTable, elem);
			return LEX_LITERAL;
		}
		else
		{
			IT::Entry* elem = new IT::Entry(); // выделяю память под элемент
			// elem.id - имя идентификатора, сразу укорачивается до ID_MAXSIZE (5)
			for (int i = 0; i < strlen(lexem); i++)
			{
				if (i == ID_MAXSIZE) break;
				elem->id[i] = lexem[i];
			}

			elem->iddatatype = IT::INT; // т.к. int
			elem->idtype = IT::P; // !!!! придумать как определить тип !!!!
			elem->idxfirstLE = 0; // пока что 0, т.к. не надо сейчас.
			elem->value.vint = atoi(lexem);

			IT::Add(idTable, elem);
			return LEX_ID;
		}
		*/
}
