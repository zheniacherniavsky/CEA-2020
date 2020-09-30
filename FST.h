#pragma once
#include <cstdlib>
#include <iostream>
namespace FST {
	struct RELATION { //ребро
		char symbol;
		short nnode;  //номер состояния, после ребра 
		RELATION(char c = 0x00, short ns = NULL);
	};

	struct NODE {
		short n_relation;  //количество инцидентных ребер
		RELATION *relations;  //сами инцидентные ребра
		NODE();
		NODE(short n, RELATION rel, ...);
	};

	struct FST {//НКА Автомат 
		const char* string; //цепочка
		short position; //позиция
		short nstates; //кол-во состояний 
		NODE* nodes; //Вершины/Состояния
		short* rstates; //возможны состояния на данной позиции 
		FST(const char* s, short ns, NODE n, ...);
	};

	int execute(FST& fst);
};