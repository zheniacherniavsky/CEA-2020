#pragma once
#define ID_MAXSIZE		5			// ���� ����� �������� � ��������������
#define IT_MAXSIZE		4096		// ���� ����� ����� � ������� ���������������
#define IT_INT_DEFAULT	0x00000000	// �������� �������������� �� ��������� ��� ����
#define IT_STR_DEFAULT	0x00		// ����. �� ���� ��� ������
#define IT_NULL_IDX		0xffffffff	// ��� �������� � ������� ��������������
#define IT_STR_MAXSIZE	255			
namespace IT						// ������� ���������������
{
	enum IDDATATYPE {INT=1, STR=2};			// ���� ������ ���������������
	enum IDTYPE		{V=1, F=2, P=3, L=4};	// ���� ���������������: �������., �������, �����., �������

	struct Entry
	{
		int		idxfirstLE;			// ������ ������ ������ � ������� ������
		char	id[ID_MAXSIZE];		// ������������� (������������� ������������� �� ID_MAXSIZE)
		IDDATATYPE	iddatatype;		// ��� ������
		IDTYPE		idtype;			// ��� ��������������

		Entry* next;				// ����������� ������ (��������� �������)

		union
		{
			int vint;				// �������� integer
			struct
			{
				char len;						// ����� �������� � string
				char str[IT_STR_MAXSIZE - 1];	// ������� string
			} vstr[IT_STR_MAXSIZE];				// �������� string
		} value;					// �������� ��������������
	};

	struct IdTable					// ��������� ������� ���������������
	{
		int maxsize;				// ������� ������� ���������������	<	TI_MAXSIZE
		int size;					// ������� ������ ������� ���������������	<	maxsize
		Entry* table;				// ������ ����� ������� ���������������

		Entry* head;				// ���������� ������ ������
	};
	
	IdTable Create(int size);		// ������� ������� ���������������

	void Add(IdTable& idtable, Entry entry);	// ������� ������� ���������������

	Entry GetEntry(IdTable& idtable, int n);	// �������� ������ ������� ���������������

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);	// �������: ����� ������ (���� ����), TI_NULLIDX (���� ���)

	void Delete(IdTable& idtable); // ���������� ������ (������� ������� ���������������

}