#pragma once
#define ID_MAXSIZE		5			// макс колво символов в идентификаторе
#define IT_MAXSIZE		4096		// макс колво строк в таблице идентификаторов
#define IT_INT_DEFAULT	0x00000000	// значение идентификатора по умолчанию для инта
#define IT_STR_DEFAULT	0x00		// знач. по умол для строки
#define IT_NULL_IDX		0xffffffff	// нет элемента в таблице иденификаторов
#define IT_STR_MAXSIZE	255			
namespace IT						// таблица идентификаторов
{
	enum IDDATATYPE {EMPTY = 0, INT=1, STR=2};			// типы данных идентификаторов
	enum IDTYPE		{V=1, F=2, P=3, L=4};	// типы идентификаторов: перемен., функция, парам., литерал

	struct Entry
	{
		int		idxfirstLE;				// индекс первой строки в таблице лексем
		int idxTI;						// индекс в таблице идентификаторов или NULL
		char	id[ID_MAXSIZE];			// идентификатор (автоматически устанавливает до ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE		idtype;				// тип идентификатора

		bool declared;
		
		struct
		{
			char functionName[ID_MAXSIZE];
			int area;
		} visibility;
		

		Entry* next = nullptr;			// односвязный список (следующий элемент)

		union
		{
			int vint;					// значение integer
			struct
			{
				int len;						// колво символов в string
				char str[IT_STR_MAXSIZE - 1];	// символы string
			} vstr[IT_STR_MAXSIZE];				// значение string
		} value;					// значение идентификатора
	};

	struct IdTable					// экземпляр таблицы идентификаторов
	{
		int maxsize;				// емкость таблицы идентификаторов	<	TI_MAXSIZE
		int size;					// текущий размер таблицы идентификаторов	<	maxsize
		Entry* table;				// массив строк таблицы идентификаторов

		Entry* head;				// запоминаем начало списка
	};
	
	IdTable Create(int size);		// создать таблицу идентификаторов

	void Add(IdTable& idtable, Entry entry);	// создать таблицу идентификаторов

	Entry* GetEntry(IdTable& idtable, int idx);	// получить элемент таблицы идентификаторов по idx

	int IsId(IdTable& idtable, char id[ID_MAXSIZE], short visArea, char fnkName[]);	// вернёт idx идентификатора

	void Delete(IdTable& idtable); // освободить память (удалить таблицу идентификаторов
}