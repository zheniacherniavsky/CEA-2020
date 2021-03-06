# Спецификация языка программирования CEA-2020

## 1.1. Характеристика языка программирования
Язык программирования CEA-2020 является процедурным, универсальным, строготипизированным, компилируемым и не объектно-ориентированным языком.

## 1.2.	Алфавит языка
Алфавит языка CEA-2020 основан на кодировке Windows-1251, представленной на рисунке 1.1.
![1.1](http://cs50x.ru/assets/asciitable2.png)
*Рисунок 1.1 – Алфавит входных символов*
Символы, используемые на этапе выполнения: символы латинского алфавита, цифры десятичной системы счисления от 0 до 9, спецсимволы, а также непечатные символы пробела, табуляции и перевода строки.

## 1.3. Символы сепараторы
*Таблица 1.1 - Сепараторы*
| Сепаратор | Название | Область приминения |
| --------- | -------- | ------------------ |
|    ' '    |  Пробел  | Допускается везде, кроме идентификаторов и ключевых слов |
|     ;     | Точка с запятой | Разделение конструкций |
| {...}     | Фигурные скобки | Заключение программного блока |
| (...) | Круглые скобки | Приоритет операций, параметры функции |
| '...' | Одинарные кавычки | Допускается везде, кроме идентификаторов и ключевых слов |
| = | Знак равно | Присваивание значения |
| , | Запятая | Разделение параметров |
| + - * / | Знаки «плюс», «минус», «умножение», «деление» | Выражения |

## 1.4.	Применяемые кодировки
Для написания исходного кода на языке программирования CEA-2020 используется кодировка Windows-1251.

## 1.5.	Типы данных
В языке CEA-2020 реализованы два типа данных: целочисленный и строковый. Описание типов данных, предусмотренных в данным языке представлено в таблице 1.2.


*Таблица 1.2 – Типы данных языка CEA-2020*
| Тип данных | Описание типа данных |
| ---------- | -------------------- |
| Целочисленный тип данных `integer` | Фундаментальный тип данных. Используется для работы с целочисленными положительными значениями. В памяти занимает 1 байт. Максимальное значение: 256. Минимальное значение: 0. Инициализация по умолчанию: значение 0. |
| Строковый тип данных `string` | Фундаментальный тип данных. Используется для работы с символами, каждый символ в памяти занимает 1 байт. Максимальное количество символов: 255. Инициализация по умолчанию: длина 0, символ конца строки `'\0'`. |


## 1.6.	Преобразование типов
Преобразование типов данных не поддерживается, т.е. язык является строготипизированным.

## 1.7.	Идентификаторы
Имя идентификатора может содержать только малые буквы, от 1 до 5 букв; идентификатор не может совпадать с ключевыми словами; максимальное количество идентификаторов 2^16.

## 1.8.	Литералы
С помощью литералов осуществляется инициализация переменных. Существует два типа литералов: целочисленные (числа от 0 до 256, интерпретируются как integer, могут быть только rvalue) и строковые (строки, символы, заключенные в ‘’(кавычки) или “”(двойные кавычки), могут быть только rvalue).

## 1.9.	Область видимости идентификаторов
Область видимости «сверху вниз» (по принципу С++). В языке CEA-2020 требуется обязательное объявление переменной перед её инициализацией и последующим использованием. Все переменные должны находиться внутри программного блока. Имеется возможность объявления одинаковых переменных в разных блоках, т. к. переменные, объявленные в одной функции, недоступны в другой. Каждая переменная получает поле – название функции, в которой она объявлена.

## 1.10.	Инициализация данных
При объявлении переменной допускается инициализация данных. Некоторые способы инициализации переменных: declare <тип данных> <идентификатор>, declare <тип данных> <идентификатор> = <литерал> или <выражение>, автоматическая инициализация: переменные типа count инициализируются нулём, переменные типа string – пустой строкой; <идентификатор> = <значение>, присваивание переменной значения.

## 1.11.	Инструкции языка
Все возможные инструкции языка программирования CEA-2020 представлены в общем виде в таблице 1.3.


*Таблица 1.3 – Инструкции языка программирования CEA-2020*
| Инструкция | Запись на языке CEA-2020 |
| ----- | ----- |
| Объявление переменной | `let <тип данных> <идентификатор>;` |
| Присваивание | `<идентификатор>=<значение>/<идентификатор>;` |
| Объявление внешней функции | `<тип данных> function <идентификатор> (<тип данных> <идентификатор>, …)` |
| Блок инструкций | `{ ... }` |
| Возврат из подпрограммы | `return <идентификатор> / <литерал>;` |
| Вывод целочисленных данных | `printI (<идентификатор>);` |
| Вывод строковых данных | `printS (<литерал>);` |



