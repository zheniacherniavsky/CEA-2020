#define _CRT_SECURE_NO_WARNINGS

// REGULAR HANDLER
// you can enable or disable the regular express handler
// false:
//		in -in you get only handled program text
// true:
//		you enable regular express handler
#define REGULAR_HANDLER false 

#include "In.h"
#include "Error.h"
#include "Log.h"
#include "FST/FST.h"
#include <fstream>
#include <cstring>
#include <iostream>
using namespace std;

namespace In {

	int getAscii(char a) {
		for (int i = 0; i < 256; i++)
			if ((char)i == a) return i;
		throw ERROR_THROW(0);
	}

	_IN_ getin(char* dir, char* outdir) {
		_IN_ info;
		info.fullsize = 0;
		info.size = 0;
		info.text = new char[IN_MAX_LEN_TEXT];
		info.lines = 0;
		info.fullLines = 0;
		info.ignored = 0;

		ifstream text(dir);
		char buff[IN_MAX_LEN_LINE];
		if(!text.is_open()) throw ERROR_THROW(0);
		while (!text.eof()) {
			text.getline(buff, IN_MAX_LEN_LINE);
			info.lines++;
			for (int i = 0; i <= strlen(buff); i++) {
				info.fullsize++;
				// debug check
				std::cout << buff[i] << " " << i << " code: " << getAscii(buff[i]) << std::endl;

				// check spaces
				if (getAscii(buff[i]) == 32 && getAscii(buff[i + 1] == 32)) {
					i--;
					continue;
				} else if ((getAscii(buff[i]) == 32 && i == 0))
					continue;

				if (IN_CODE_TABLE[getAscii(buff[i])] == T) {
					info.text[info.size] = buff[i];
					info.size++;
				}

				else if (IN_CODE_TABLE[getAscii(buff[i])] == F) {
					throw ERROR_THROW_IN(110, info.lines, i + 1);
				}

				else if (IN_CODE_TABLE[getAscii(buff[i])] == I) {
					info.ignored++;
					continue;
				}

				else {
					info.text[info.size] = (char)IN_CODE_TABLE[getAscii(buff[i])];
					info.size++;
				}
			}
			info.fullLines++;
		}

		info.text[info.size] = '\0';
		cout << info.text << endl;
		text.close();

		// OUT

		// a(g)⁺((b+c+d)e(g)⁺)*g⁺fe - regular expression

		// a - main
		// b - send
		// c - wait
		// d - show
		// e - ;
		// f - return
		// g - SPACE (' ')

		if (REGULAR_HANDLER) {
			ofstream out(outdir);
			char* regular = strtok(info.text, "\n");

			while (regular != NULL) {
				FST::FST fst1(
					regular,
					9,
					FST::NODE(1, FST::RELATION('a', 1)),
					FST::NODE(2, FST::RELATION('g', 2), FST::RELATION('g', 1)),

					FST::NODE(5,
						FST::RELATION('b', 3),
						FST::RELATION('c', 3),
						FST::RELATION('d', 3),
						FST::RELATION('g', 6),
						FST::RELATION('g', 5)
					),
					FST::NODE(1, FST::RELATION('e', 4)),

					FST::NODE(5,
						FST::RELATION('g', 5),
						FST::RELATION('g', 4),
						FST::RELATION('b', 3),
						FST::RELATION('c', 3),
						FST::RELATION('d', 3)
					),

					FST::NODE(2, FST::RELATION('g', 6), FST::RELATION('g', 5)),

					FST::NODE(1, FST::RELATION('f', 7)),
					FST::NODE(1, FST::RELATION('e', 8)),

					FST::NODE()
				);
				if (FST::execute(fst1))
				{
					out << "(+) Цепочка распознана:\t\t\t" << regular << "\n";
				}
				else
				{
					out << "(-) Цепочка не распознана:\t\t" << regular << "\n";
				}
				regular = strtok(NULL, "\n");
			}
			out.close();
		}
		else {
			ofstream out(outdir);
			out << info.text;
			out.close();
		}

		return info;
	}
}