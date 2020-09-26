﻿#pragma once
#define LEXEMA_FIXSIZE 1 // фиксированный размер лексемы
#define LT_MAXSIZE 4096 // максимальное кол-во строк в таблице лексем
#define LT_TI_NULLIDX 0xffffffff // нет элемента в таблице идентификаторов
#define LEX_INTEGER 't' // лексема для integer
#define LEX_STRING 't' // лексема для string
#define LEX_ID 'i' // лексема для идентификатора
#define LEX_LITERAL 'l' // лексема для литерала
#define LEX_FUNCTION 'f' // лексема для функции
#define LEX_DECLARE 'd' // лексема для declare
#define LEX_RETURN 'r' // лексема для return
#define LEX_PRINT 'p' // лексема для print
#define LEX_SEMICOLON ';' // лексема для ;
#define LEX_COMMA ',' // ,
#define LEX_LEFTBRACE '{'
#define LEX_BRACELET '}'
#define LEX_LEFTHESIS '('
#define LEX_RIGHTHESIS ')'
#define LEX_PLUS '+' // +
#define LEX_MINUS '-' // - 
#define LEX_STAR '*' // *
#define LEX_DIRSLASH '/' // /

#include "IT.h"


namespace LT // таблица лексем
{

	struct Entry
	{
		char lexema[LEXEMA_FIXSIZE]; // лексема
		int sn; // номер строки в исходном тексте
		int idxTI; // индекс в таблице идентификаторов или NULL

		Entry* next = nullptr;

		int updateIndex(int i); // обновление индекса идентификатора
	};

	struct LexTable
	{
		int maxsize = LT_MAXSIZE; // емкость таблицы лексем < LT_MAXSIZE
		int size; // текущий размер таблицы лексем
		Entry* table; // массив строк таблицы лексем
		Entry* head;
	};

	LexTable Create( // создать таблицу лексем
		int size // с таким размеров
	);
	
	void Add( // добавить строку в таблицу лексем
		LexTable& lextable, // экземпляр таблицы
		Entry entry // строка таблицы лексем
	);

	Entry GetEntry( // получить строку таблицы лексем
		LexTable& lextable, // откуда , т.е. табл. лексем
		int n // номер получаемой строки
	);

	void Delete(LexTable& lextable);

	char compareLexems(char* lexem, IT::IdTable& idTable);
}

