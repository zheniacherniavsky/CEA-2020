#define _CRT_SECURE_NO_WARNINGS

// REGULAR HANDLER
// you can enable or disable the regular express handler
// false:
//		in -in you get only handled program text
// true:
//		you enable regular express handler
#define REGULAR_HANDLER true 

#define MAX_LEXEMS_LENGTH 100

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

	char* lexems(char* s) {
		// from this function we get a lexems which are used in FST
		char* str = new char[strlen(s)];
		for (int i = 0; i < strlen(s); i++)
			str[i] = s[i];
		const char* separators = " ,()";
		bool pass = false;
		for (int i = 0; i < strlen(str); i++, pass = false) {
			for (int j = 0; j < strlen(separators); j++) {
				if (str[i] == separators[j])
					pass = true;
			}
			if (pass) str[i] = '\n';
		}
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] == '\n' && str[i + 1] == '\n') {
				for (int j = i + 1; j < strlen(str); j++) {
					str[j] = str[j + 1];
				}
				str[strlen(str)] = '\0';
			}
		}
		str[strlen(str) - 4] = '\0'; // fix size lol i dont wanna find bug
		return str;
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
			for (int i = 0, spaceControl = 0; i <= strlen(buff); i++) {
				info.fullsize++;
				// debug check
				// std::cout << buff[i] << " " << i << " code: " << getAscii(buff[i]) << std::endl;

				// ----- SEPARATORS -----
				// delete { and } 
				if (getAscii(buff[i]) == 123 || getAscii(buff[i]) == 125) {
					info.ignored++;
					break;
				}

				// delete excess lines
				if (getAscii(buff[i] == 0 && i == 0)) {
					info.ignored++;
					continue;
				}

				// check spaces like a trim() function
				if ((getAscii(buff[i]) == 32 || getAscii(buff[i]) == 9) && i == spaceControl) {
					info.ignored++;
					spaceControl++;
					continue;
				}
				else if (getAscii(buff[i]) == 32 && getAscii(buff[i + 1]) == 32) {
					info.ignored++;
					continue;
				}
				// ---------------------

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
		
		char* lexemsList = lexems(info.text);
		cout << lexemsList << endl << info.text << endl;

		

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
			char* regular = strtok(lexemsList, "\n");
			
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