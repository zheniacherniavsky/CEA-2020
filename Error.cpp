#include "Error.h"

namespace Error {
	// Таблица ошибок
	//   0 -  99 : системные ошибки
	// 100 - 109 : ошибки параметров
	// 110 - 119 : ошибки открытия файла
	ERROR_ errors[ERRORS_MAX_SIZE] =
	{
		ERROR_ENTRY(0, (char*)"Недопустимый код ошибки"),
		ERROR_ENTRY(1, (char*)"Системный сбой"),
		ERROR_ENTRY_NODEF(2),ERROR_ENTRY_NODEF(3),ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5),ERROR_ENTRY_NODEF(6),ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70),ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, (char*)"Возможно использовать только 3 параметра [-in: , -out: , -log:]"),
		ERROR_ENTRY(101, (char*)"Параметр -in: должен быть задан"),
		ERROR_ENTRY(102, (char*)"Синтаксическая ошибка для параметра [-in:]"),
		ERROR_ENTRY(103, (char*)"Синтаксическая ошибка для параметра [-out:]"),
		ERROR_ENTRY(104, (char*)"Синтаксическая ошибка для параметра [-log:]"),
		ERROR_ENTRY(105, (char*)"Превышен лимит символов для параметра. Максимум 300"),
		ERROR_ENTRY_NODEF(106),ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, (char*)"Данный символ запрещен!"),
		ERROR_ENTRY(111, (char*)"LOG файл не может открыться. Проверьте путь к файлу."),
		ERROR_ENTRY_NODEF(112),ERROR_ENTRY_NODEF(113),
		ERROR_ENTRY_NODEF(114),ERROR_ENTRY_NODEF(115),ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117),ERROR_ENTRY_NODEF(118),ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120),ERROR_ENTRY_NODEF10(130),
		ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),ERROR_ENTRY_NODEF100(600),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),ERROR_ENTRY_NODEF100(900)
	};
	ERROR_ geterror(int id) {
		if (id >= 0 && id < ERRORS_MAX_SIZE) return errors[id];
		else return errors[0];
	}
	ERROR_ geterrorin(int id, short line = -1, short col = -1) {
		ERROR_ error;
		error = errors[id];
		// get in.file info about cols and lines
		error.inHandler.col = col;
		error.inHandler.line = line;
		if (id >= 0 && id < ERRORS_MAX_SIZE) return error;
		else return errors[0];
	}
}