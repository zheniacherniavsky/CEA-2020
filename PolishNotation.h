#pragma once

namespace PN {
	bool PolishNotation( // �������� �������� ������
		int lextable_pos, // ������� ��������� � ������� ������
		LT::LexTable& lexTable, // ������� ������
		IT::IdTable& idTable, // ������� ���������������
		bool debug = false
	);
}
