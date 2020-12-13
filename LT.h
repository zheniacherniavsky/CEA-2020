#pragma once

#include "IT.h"

#define LEXEMA_FIXSIZE 1 // фиксированный размер лексемы
#define LT_MAXSIZE 4096 // максимальное кол-во строк в таблице лексем
#define LT_FUNCTION_MAX_PARMS 8 // максимальное кол-во параметров в функции

#define LT_TI_NULLIDX		0xffffffff	// нет элемента в таблице идентификаторов
#define LEX_INTEGER			't'			// лексема для integer
#define LEX_STRING			't'			// лексема для string
#define LEX_ID				'i'			// лексема для идентификатора
#define LEX_LITERAL			'l'			// лексема для литерала
#define LEX_NUMBER			'n'			// для числа
#define LEX_FUNCTION		'f'			// лексема для функции
#define LEX_DECLARE			'd'			// лексема для declare
#define LEX_RETURN			'r'			// лексема для return
#define LEX_PRINT_INT		'p'			// лексема для print
#define LEX_PRINT_STR		's'			// лексема для print
#define LEX_MAIN			'm'			// main
#define LEX_SEMICOLON		';'			// лексема для ;
#define LEX_COMMA			','			// лексема для ,
#define LEX_LEFTBRACE		'{'			// лексема для {
#define LEX_BRACELET		'}'			// лексема для }
#define LEX_LEFTHESIS		'('			// лексема для (
#define LEX_RIGHTHESIS		')'			// лексема для )
#define LEX_PLUS			'+'			// лексема для +
#define LEX_MINUS			'-'			// лексема для - 
#define LEX_STAR			'*'			// лексема для *
#define LEX_DIRSLASH		'/'			// лексема для /
#define LEX_IS				'='			// лексема для =
#define LEX_CYCLE			'w'			// лексема для repeat()
#define LEX_CONDITION		':'			// лексема для if()
#define LEX_CONVERT			'c'			// лексема для itos()
#define LEX_POW				'$'			// лексема для pow()
#define LEX_ROOT			'q'			// лексема для root()
#define LEX_PERCENT			'%'			// лексема для %
#define LEX_AND				'&'			// лексема для &
#define LEX_OR				'|'			// лексема для |
#define LEX_INVERT			'~'			// лексема для ~
#define LEX_LESS			'<'			// лексема для <
#define LEX_OVER			'>'			// лексема для >
#define LEX_EQUAL			'e'			// лексема для ==
#define LEX_DOG				'@'			// лексема для @

namespace LT // таблица лексем
{

	struct Entry
	{
		char lexema[LEXEMA_FIXSIZE]; // лексема
		int sn; // номер строки в исходном тексте
		int idxTI; // индекс в таблице идентификаторов или NULL
		int priority;

		Entry* next = nullptr;

		struct funcParms
		{
			int count = 0;
			int idx[LT_FUNCTION_MAX_PARMS];
			int memoryCount = 0;
			IT::IDDATATYPE memoryType[4] = {IT::EMPTY, IT::EMPTY, IT::EMPTY, IT::EMPTY };
		} func;

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

	Entry* GetEntry( // получить строку таблицы лексем
		LexTable& lextable, // откуда , т.е. табл. лексем
		int n // номер получаемой строки
	);

	Entry* GetEntryByIdx(LexTable& lextable, int idx);

	int getSnByPos(LexTable& lex, int pos); // получить номер строки по ноеру элемента в таблице

	void Delete(LexTable& lextable);

}