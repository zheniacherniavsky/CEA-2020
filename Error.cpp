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
		// 200-230 лексический анализатор
		ERROR_ENTRY(200, (char*)"Превышен максимальный размер лексической таблицы!"),
		ERROR_ENTRY(201, (char*)"Данной таблицы не существует или её компоненты не проинициализированы!"),
		ERROR_ENTRY(202, (char*)"Не обнаружена лексема в данной строке!"),
		ERROR_ENTRY(203, (char*)"Открыта или не закрыта лишняя скобка!"),
		ERROR_ENTRY(204, (char*)"Точка входа main не была найдена!"),
		ERROR_ENTRY(205, (char*)"Точка входа main может быть только одна!"),
		ERROR_ENTRY(206, (char*)"Перед инициализацией переменной должно быть слово declare"),
		ERROR_ENTRY(207, (char*)"Для типа данных integer допустимое значение 0 - 256!\nДля того, чтобы убрать ограничения и брать число постоянно по модулю, нужно написать в самом начале кода unchecked."),
		ERROR_ENTRY(208, (char*)"На языке CEA-2020 не предусмотрены глобальные переменные!"),
		ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210),ERROR_ENTRY_NODEF10(220),ERROR_ENTRY_NODEF10(230),
		ERROR_ENTRY_NODEF10(240),ERROR_ENTRY_NODEF10(250),ERROR_ENTRY_NODEF10(260),
		ERROR_ENTRY_NODEF10(270),ERROR_ENTRY_NODEF10(280),ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, (char*)"Неверная структура программы!"),
		ERROR_ENTRY(601, (char*)"Ошибочный оператор!"),
		ERROR_ENTRY(602, (char*)"Ошибка в выражении!"),
		ERROR_ENTRY(603, (char*)"Ошибка в выражении!"),
		ERROR_ENTRY(604, (char*)"Ошибка в параметрах функции!"),
		ERROR_ENTRY(605, (char*)"Ошибка в параметрах вызываемой функции!"),
		ERROR_ENTRY_NODEF(606),
		ERROR_ENTRY_NODEF(607),
		ERROR_ENTRY_NODEF(608),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610),ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640),ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660),
		ERROR_ENTRY_NODEF10(670),ERROR_ENTRY_NODEF10(680),ERROR_ENTRY_NODEF10(690),
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